//
// Created by akiva on 1/16/23.
//

#ifndef AP_1_EX_4_CLASSIFYDATA_H
#define AP_1_EX_4_CLASSIFYDATA_H

#include "../knnserver/KNN_Client_Handler.h"
#include "Command.h"

class ClassifyData : public Command {
public:
    ClassifyData(KNN_Client_Handler& clientServerPtr, DefaultIO& dio);
    void execute() override;
};




#endif //AP_1_EX_4_CLASSIFYDATA_H
