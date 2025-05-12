//
// Created by akiva on 1/16/23.
//

#ifndef AP_1_EX_4_EXIT_H
#define AP_1_EX_4_EXIT_H

#include "../knnserver/KNN_Client_Handler.h"
#include "Command.h"

class Exit : public Command {
public:
    Exit(KNN_Client_Handler& clientServerPtr, DefaultIO& dio, bool& noExit);
    void execute() override;
private:
    bool& noExit;
};



#endif //AP_1_EX_4_EXIT_H
