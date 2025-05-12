//
// Created by akiva on 1/16/23.
//

#include "Exit.h"

const string cmdDescription{"exit client thread"};

Exit::Exit(KNN_Client_Handler& clientServerPtr, DefaultIO& dio, bool& noExit) :
        Command(cmdDescription, dio, clientServerPtr), noExit(noExit) {}

void Exit::execute() {
    noExit = false;
    // hold until download thread finishes
}