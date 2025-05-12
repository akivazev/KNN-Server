//
// Created by akiva on 1/12/23.
//

#include "Command.h"

Command::Command(std::string cmdDescription, DefaultIO& dio, KNN_Client_Handler& clientServerPtr)
    : description(cmdDescription), dio(dio), client_server(clientServerPtr) {}

string Command::getDescription() {
    return description;
}