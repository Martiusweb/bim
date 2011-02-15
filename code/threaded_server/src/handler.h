#ifndef _HANDLER_H_
#define _HANDLER_H_

#include "request.h"

class Handler
{
  public:
    Handler();
    virtual ~Handler();
    /**
     * @brief Handle the request
     *
     * @param request The request
     */
    virtual void handle(Request& request);

    /**
     * @brief Add a handler at the end of the handler list
     *
     * @param next_handler the handler to be added to the list
     */
    void add(Handler* next_handler);
  private:
    Handler* next_;
};

#endif

