#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
namespace bim
{
  /**
   * @brief The context of the webserver.
   */
class Context
{
  public:
    std::string get_document_root();
    void set_document_root(const std::string& document_root);
  private:
    std::string document_root_;
};
}

#endif
