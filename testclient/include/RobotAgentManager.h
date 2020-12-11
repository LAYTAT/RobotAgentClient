//
// Created by laylei on 2020/12/11.
//

#ifndef ROBOTCLIENT_ROBOTAGENTMANAGER_H
#define ROBOTCLIENT_ROBOTAGENTMANAGER_H

#include "Singleton.h"

class RobotAgentManager {
private:
    RobotAgentManager();
    ~RobotAgentManager();
    DECLARE_SINGLETON(RobotAgentManager);

public:
    bool Init();
    void Uinit();
    void Dojob();

};
#endif //ROBOTCLIENT_ROBOTAGENTMANAGER_H
