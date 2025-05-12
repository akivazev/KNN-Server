//
// Created by akiva on 1/16/23.
//

#ifndef AP_1_EX_4_DOWNLOADRESULTS_H
#define AP_1_EX_4_DOWNLOADRESULTS_H

#include <fstream>
#include <thread>
#include "../knnserver/KNN_Client_Handler.h"
#include "Command.h"

using namespace std;

class DownloadResults : public Command{
public:
    DownloadResults(KNN_Client_Handler& clientServerPtr, DefaultIO& dio);
    void execute() override;
private:
};



#endif //AP_1_EX_4_DOWNLOADRESULTS_H
