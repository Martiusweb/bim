#ifndef _REQUEST_H_
#define _REQUEST_H_

#include <string>

class Request
{
  public:
    /**
     * @brief Constructor for an HTTP request
     *
     * @param http_request The content of the request
     * @param fd the file descriptor of the client(s socket
     * @param context The context (document_root, etc.)
     */
    Request(const std::string& http_request, const int fd, const Context& context);
    /**
     * @brief Get the method for the request
     *
     * @return empty in case of error, the method otherwise
     */
    std::string& get_method();
    /**
     * @brief Get the url of this request
     *
     * @return empty in case of error, the url otherwise
     */
    std::string& get_url();
    /**
     * @brief Get the path for this request
     * @todo get document_root from context
     * @return empty in case of error, the path otherwise.
     */
    std::string& get_path();

    /**
     * @brief Get the fd back.
     *
     * @return the fd for this request.
     */
    int get_fd();

  private:
    std::string path_;
    std::string url_;
    std::string method_;
    std::string raw_;
    int fd_;
};

#endif
