/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>


#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <libgen.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "server.hpp"
#include "socket.hpp"
#include "macros.hpp"
#include "scoped_timer.h"


/**
 * This is dispayed when the user sets bad arguments.
 */
const char HELP_MESSAGE[] = "Arguments list :\n"
                            "\t-h \thostname\n"
                            "\t-p \tport number\n"
                            "\t-d \troot directory\n"
                            "\t-l \tlog mode\n";

/**
 * String returned when a 404 error shall occur.
 */
const char ERROR_404[] = "HTTP/1.1 404 NOT FOUND\r\n\r\n"
                         "<html>"
                         "<head>"
                         "<title>404 Not Found</title>"
                         "</head>"
                         "<body>"
                         "<h1>404 NOT FOUND</h1>"
                         "<body>"
                         "</html>\r\n";

/**
 * String returned when a 501 error shall occur (method not implemented).
 */
const char ERROR_501[] = "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
                         "<html><head>"
                         "<title>501 Method Not Implemented</title>"
                         "</head><body>"
                         "<h1>Method Not Implemented</h1>"
                         "<p>akenrlanre to /index.html not supported.<br />"
                         "</p>"
                         "<hr>"
                         "</body></html>";
/**
 * String returned when a 400 error occur (malformed request).
 * TODO
 */
const char ERROR_400[] = "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
                         "<html><head>"
                         "<title>400 Bad Request</title>"
                         "</head><body>"
                         "<h1>Bad Request</h1>"
                         "<p>Your browser sent a request that this server could not understand.<br />"
                         "</p>"
                         "<hr>"
                         "</body></html>";

/**
 * Header for a normal response.
 */
const char HEADER_200[] = "HTTP/1.1 200 OK\r\n\r\n";

/**
 * Begin of a file listing.
 */
const char BEGIN_LIST_FILES[] = "HTTP/1.1 200 OK\r\n"
                                "\r\n"
                                "<html>"
                                "<h1>Index of %s</h1>"
                                "<br><hr>"
                                "<ul>";

const char GET_REQUEST[] = "GET %s HTTP/1.1\r\n";

/**
 * Element of a file listing.
 */
const char LIST_ELEMENT[] = "<li><a href=\"%s\">%s</a></li>";


/**
 * End of a file listing.
 */
const char END_LIST_FILES[]   = "</ul>"
                                "<hr>"
                                "</html>";

const unsigned int LISTEN_BACKLOG = 100;

/**
 * Get the path from the document root from an HTTP request
 * @param request The HTTP request
 * @param path The document_root, to be appended.
 * @return -1 if bad request, else 0
 */
int get_URL(const char *request, char *path, char *requested)
{

  if(sscanf(request, GET_REQUEST, requested) != 1)
    return -1;

  strcat(path, "/");
  strcat(path, requested);

  return 0;
}

void* thread_function(void *client_infos)
{
  // scoped_timer timer(std::cerr);
  struct client_info *infos = (struct client_info *) client_infos;
  // The number of bytes read
  int count = 0;
  // The path of the file
  char path[SIZE_URL];
  // The path the user requested
  char requested[SIZE_URL];
  // The http request
  char request[BUFFER_SIZE];

  if( (count = recv(infos->fd_client, request, BUFFER_SIZE, 0)) > 0)
  {
    request[count] = 0;

    strcpy(path, infos->document_root);

    if(get_URL(request, path, requested))
    {
      send_socket(infos->fd_client, ERROR_501, strlen(ERROR_501));
      goto clean;
    }

    send_content(path, requested, infos->fd_client);
  }

clean:
  close_socket(infos->fd_client);
  delete infos;
  pthread_exit(0);
}

/**
 * This functions decides what to send back, from the HTTP header
 * @param name The name of the item (directory, file) to open
 * @param requested The name of the file, as requested by the client
 * @param fd_socket The socket to write to.
 */
void send_content(const char *name, const char* requested, int fd_socket)
{
  int count;
  char content[BUFFER_SIZE];
  DIR* handle;



  // Try to open a directory named <name>
  if((handle = opendir(name)) != NULL)
  {
    char temp_path[BUFFER_SIZE];
    int fd;

    sprintf(temp_path, "%s/index.html", name);

    if( (fd = open(temp_path, O_RDONLY)) != -1)
    {
      // Request listing subdirectory : recursive call
      TEST_FAILURE(close(fd));
      send_content(temp_path, requested,fd_socket);
    }
    else
    {
      list_directory(name, requested,fd_socket);
    }
    TEST_FAILURE(closedir(handle));
  }
  // Then, we try to open it as a file
  else
  {
    if(errno == ENOTDIR || errno == ENOENT)
    {
      int fd;
      
      if( (fd = open(name, O_RDONLY)) == -1)
      {
        send_socket(fd_socket, ERROR_404, strlen(ERROR_404));
      }
      else
      {
        send_socket(fd_socket, HEADER_200, strlen(HEADER_200));

        while( (count = read(fd, content, BUFFER_SIZE)) > 0)
        {
          send_socket(fd_socket, content, count);
        }

        TEST_FAILURE(close(fd));
      }
    }
    else
    {
      perror("Opening directory failed.");
      exit(1);
    }
  }
}

/**
 * Comparation function for qsort
 * @param str1 The first string to compare
 * @param str2 The second string to compare
 * @return Same as the return of strcmp
 */
int string_compare(const void* str1, const void *str2)
{
  const char* const * a = static_cast<const char* const*>(str1);
  const char* const * b = static_cast<const char* const*>(str2);

  return (strcmp(*a, *b));
}

/**
 * @brief List the content of a directory
 *
 * @todo Stat the flie to determine type and permissions, and to be able to
 * give 403 error.
 *
 * @param name the name of the file
 * @param fd_socket the socket to write to
 */
void list_directory(const char *name, const char* requested,int fd_socket)
{
  DIR *rep;
  char *file_list[SIZE];
  int i, j;

  if ( (rep = opendir(name)) != NULL)
  {
    struct dirent *ent;
    char begin_message[SIZE_PATH];
    sprintf(begin_message,BEGIN_LIST_FILES,requested);

    send_socket(fd_socket, begin_message, strlen(begin_message));

    for(i = 0; ((ent = readdir (rep)) != NULL); i++)
    {
        file_list[i] = new char[strlen(ent->d_name) + 2];
        sprintf(file_list[i],"%s", ent->d_name);

        if(ent->d_type == DT_DIR)
          strcat(file_list[i],"/");
    }

    qsort(file_list, i, sizeof *file_list, string_compare);

    for(j = 0; j < i; j++)
    {
      char link[SIZE_URL];

      sprintf(link, LIST_ELEMENT, file_list[j], file_list[j]);
      send_socket(fd_socket, link, strlen(link));

      delete [] file_list[j];
    }

    send_socket(fd_socket, END_LIST_FILES, strlen(END_LIST_FILES));

    TEST_FAILURE(closedir (rep));
  }
  else
  {
    std::cerr << __FILE__ << ":" << __LINE__;
    perror("opening directory");
    exit(1);
  }
}

void new_client(int fd, struct server_infos *infos, struct sockaddr_in* client)
{
  struct client_info *infos_client = new client_info;
  pthread_t fd_thread;

  // LOG << "Address : " << inet_ntoa(client->sin_addr) << std::endl;

 infos_client->fd_client = fd;
  strcpy(infos_client->document_root, infos->document_root);

  TEST_FAILURE(pthread_create(&fd_thread, NULL, thread_function, infos_client) < 0);

  TEST_FAILURE(pthread_detach(fd_thread));
}

void usage(void)
{
  fprintf(stderr, HELP_MESSAGE);
  exit(1);
}

void server_init(struct server_infos *infos)
{
  TEST_FAILURE( (infos->fd_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1);

  int optval = 1;
  TEST_FAILURE(setsockopt(infos->fd_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)));

  bind_socket(infos->fd_socket, infos->hostname, infos->port);

  TEST_FAILURE(listen(infos->fd_socket, LISTEN_BACKLOG) == -1);
}

void start(struct server_infos *infos)
{
  int fd;
  sockaddr_in client;
  socklen_t socklen = sizeof(client);
  while((fd = accept(infos->fd_socket, reinterpret_cast<sockaddr*>(&client), &socklen)) != 0)
  {
    TEST_FAILURE(fd == -1);
    new_client(fd, infos, &client);
  }

  close_socket(infos->fd_socket);
}

int arguments(int argc, char **argv, struct server_infos *infos)
{
  int c, err = 0, cflag = 0;

  while ((c = getopt(argc , argv, "h:p:d:g")) != -1)
  {
    switch (c)
    {
      case 'h':
        cflag++;
        strcpy(infos->hostname, optarg);
        break;
      case 'p':
        cflag++;
        if(sscanf(optarg, "%d", &(infos->port)) != 1)
        {
          fprintf(stderr, "Erreur dans le numero du port\n");
          err++;
        }
        break;
      case 'd':
        cflag++;
        strcpy(infos->document_root, optarg);
        break;
      case '?':
        err++;
        break;
      case 'l':
        cflag++;
        infos->log = 1;
        break;
    }

    TEST_FAILURE(err);
  }
  return 0;
}
