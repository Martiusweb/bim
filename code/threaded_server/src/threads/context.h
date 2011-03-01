#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
namespace bim
{
  /**
   * @brief The context of the webserver.
   * For now, a struct, maybe a class in the future.
   */
struct Context
{
  std::string document_root_;
};
}

#endif
