//
// Created by akiva on 1/16/23.
//

#include "AlgoSettings.h"

const string cmdDescription{"set algorithm settings"};

AlgoSettings::AlgoSettings(KNN_Client_Handler& clientServerPtr, DefaultIO& dio) :
    Command(cmdDescription, dio, clientServerPtr) {}

void AlgoSettings::execute() {
    std::stringstream s;

    s << "The current KNN parameters are: K = " << client_server.getKVal() <<
    ", distance metric = " << client_server.getDistanceMethod();

    dio.write(s.str());

    string input = dio.read();

    client_server.operateOnInput(input, dio);
}

