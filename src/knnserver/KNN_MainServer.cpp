//
// Created by akiva on 1/16/23.
//

#include "KNN_MainServer.h"

[[noreturn]] void KNN_MainServer::run(int argc, char **argv) {
    KNN_MainServer server = initServer(argc, argv);

    while(true) {
        int clientSock = server.connectClient();

        thread clientServer{KNN_Client_Handler::runClientHandler, clientSock};
    }
}

[[noreturn]] void KNN_MainServer::runAlone(int argc, char **argv) {
//    KNN_MainServer server = initServer(argc, argv);

    while(true) {
//        int clientSock = server.connectClient();

        thread clientServer{KNN_Client_Handler::runClientHandler, 5};
        clientServer.join(); // remove when using with clients
    }

}

KNN_MainServer::KNN_MainServer(int socket) : mainSock(socket) {}

KNN_MainServer KNN_MainServer::initServer(int argc, char **argv) {
    if (argc != 2) {
        cerr << "server must receive 1 argument only" << endl;
        exit(EXIT_FAILURE);
    }

    int portNum = AbstractPeer::validatePort(argv[1]);
    int socketNum = createSocket(portNum);

    KNN_MainServer newServer(socketNum);

    return newServer;
}

int KNN_MainServer::createSocket(const int port) {

    // socket creation
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
        exit(1);
    }

    // bind
    struct sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error binding socket");
        exit(1);
    }

    // listen
    if (listen(sock, 5) < 0) {
        perror("error listening to a socket");
        exit(1);
    }

    // returns descriptor for successfully connected socket
    return sock;
}

int KNN_MainServer::connectClient() {
    // accept
    struct sockaddr_in client_in{};
    unsigned int addr_len = sizeof(client_in);

    // returns socket for client from listen() queue or if it's empty, holds until a client enters
    int clientSock = accept(mainSock, (struct sockaddr *) &client_in, &addr_len);

    return clientSock;
}