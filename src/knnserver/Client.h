#ifndef AP_1_EX_2_CLIENT_H
#define AP_1_EX_2_CLIENT_H

#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "AbstractPeer.h"
#include "KNN_Client_Handler.h"

using namespace std;

class Client : public AbstractPeer{
public:
    /**
 * Initiates client socket and connects with server using given program arguments.
 * @param argc number of args
 * @param argv vector with args
 * @param is input stream
 * @param os output stream
 * @return Client object
 */
    static Client initClient(int argc, char* argv[], istream &is, ostream &os);

    /**
 * Calls the correspond method that sends and receives data from the server
 * as long as the connection is live.
 * Once the connection with the server is closed, the loop breaks and the socket is closed.
 */
    void run();
private:
    // socket descriptor
    int sock;

    // input stream
    istream& is;

    // output stream
    ostream& os;

    // true - keep connection alive. false - closes socket
    bool noExit;

    /**
 * Constructor
 * @param socketNum descriptor of socket
 * @param is input stream
 * @param os output stream
 */
    Client(int socketNum, istream &is, ostream &os);

    /**
 * Validate ip address.
 * @param ip given ip address
 * @param ipAddr ip address will be saved here
 */
    static void validateIP(const char* ip, char* ipAddr);

    /**
 * Receive data from the given socket.
 * @param serverSock socket connected to server
 * @return string containing the received message
 */
    string receiveFrom(int serverSock);

    /**
 * Send data to the given socket.
 * @param serverSock socket connected to server
 * @param message message
 */
    void sendTo(int serverSock, const string &message);

    /**
 * Send and receive data to and from the given socket.
 * A line is read from the member input stream and then checked for proper formatting. If it is correctly formatted,
 * it is sent as a message to the server using sendTo method. If the input is "-1", the global exit condition is set
 * and the function returns without receiving a message from the server. Otherwise, a message is received from the
 * server and then printed to the member ostream.
 * @param serverSock socket connected to server
 */
    void correspond(int serverSock);
};

int main(int argc, char* argv[]);

#endif //AP_1_EX_2_CLIENT_H
