//
// Created by akiva on 1/16/23.
//

#ifndef AP_1_EX_4_DISPLAYRESULTS_H
#define AP_1_EX_4_DISPLAYRESULTS_H

#include "../knnserver/KNN_Client_Handler.h"
#include "Command.h"

class DisplayResults : public Command{
public:
    DisplayResults(KNN_Client_Handler& clientServerPtr, DefaultIO& dio);
    void execute() override;
private:

};

#endif //AP_1_EX_4_DISPLAYRESULTS_H
