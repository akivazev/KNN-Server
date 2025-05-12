//
// Created by akiva on 1/16/23.
//

#ifndef AP_1_EX_4_KNN_MAINSERVER_H
#define AP_1_EX_4_KNN_MAINSERVER_H

#include <sys/socket.h>
#include <cstdio>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
#include "KNN_Client_Handler.h"
#include "AbstractPeer.h"

using namespace std;

class KNN_MainServer {
public:
    [[noreturn]] static void run(int argc, char *argv[]);

    [[noreturn]] static void runAlone(int argc, char *argv[]);


private:
    int mainSock;

    KNN_MainServer(int socket);

    static KNN_MainServer initServer(int argc, char *argv[]);

    /**
     * Gets port number, creates a TCP socket, and returns it's descriptor
     * @param port number between 0 and 65536
     * @return socket descriptor
     */
    static int createSocket(const int port);

    /**
     * Connect a new client and return it's socket descriptor
     * @param sock server socket
     * @return client socket
     */
    int connectClient();
};


#endif //AP_1_EX_4_KNN_MAINSERVER_H
