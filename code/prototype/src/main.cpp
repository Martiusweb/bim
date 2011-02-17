/**
 * This file is part of Bim, the web server which goes fast.
 * © Martin Richard & Paul Adenot
 *
 * Bim is free software : you can redistribute it and/or modify it under the
 * terms of the license.
 */

#include "server.hpp"

#include <iostream>

using namespace bim;
using namespace std;

int main(int argc, char** argv)
{
    server server;
    if(server.open(7000, 1000))
        cout << "Listening on 7000 !" << endl;

    server.close();
    cout << "Bye !" << endl;
}
