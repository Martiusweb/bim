#include "context.h"

namespace bim
{
  std::string Context::get_document_root()
  {
      return document_root_;
  }

  void Context::set_document_root(const std::string& document_root)
  {
    document_root_=document_root;
  }
}

