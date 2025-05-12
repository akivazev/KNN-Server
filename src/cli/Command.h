#ifndef AP_1_EX_4_COMMAND_H
#define AP_1_EX_4_COMMAND_H

#include <string>
#include "DefaultIO.h"
#include "../knnserver/KNN_Client_Handler.h"

using namespace std;

class Command {
public:
    virtual void execute() = 0;
    string getDescription();
protected:
    string description;
    DefaultIO& dio;
    KNN_Client_Handler& client_server;

    Command(string cmdDescription, DefaultIO& dio, KNN_Client_Handler& clientServerPtr);
};


#endif //AP_1_EX_4_COMMAND_H
