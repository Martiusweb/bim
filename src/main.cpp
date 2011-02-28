#include <iostream>
#include "Server.hpp"
#include "EventDispatcher.hpp"

using namespace bim;
using namespace std;

int main(int argc, char** argv)
{
    Server server(7000, 10000);
    EventDispatcher dispatcher(1024, 16);

    cout << "Welcome" << endl;

    dispatcher.init();
    server.init();
    server.registerEventDispatcher(dispatcher);

    return 0;
}
