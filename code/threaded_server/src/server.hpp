/// Author : Paul ADENOT <paul@paul.cx>
/// License : WTFPL <http://sam.zoy.org/wtfpl/>


#ifndef __H_SERVEUR_H__
#define __H_SERVEUR_H__

#define BUFFER_SIZE 4096
#define SIZE  256
#define SIZE_PATH 256
#define SIZE_URL 256

struct server_infos
{
  int fd_socket;
  int port;
  int log;

  char hostname[SIZE_URL];
  char document_root[SIZE_URL];
};

struct client_info
{
  int fd_client;

  char document_root[SIZE_URL];
};

void server_init(struct server_infos *infos);
void new_client(int fd, struct server_infos *infos, struct sockaddr_in* client);
void usage(void);
void *thread_function(void *infosClient);
void start(struct server_infos *infos);
void send_content(const char *name, const char* requested,int fd_socket);
int get_URL(const char *requete, char *url, char* requested);
void list_directory(const char *name, const char* requested, int fd_socket);
int string_compare(const void* str1, const void *str2);
int arguments(int argc, char **argv, struct server_infos *infos);

#endif
