//
// Created by laylei on 2020/12/11.
//

#include "RobotAgentManager.h"
#include <iostream>

INSTANCE_SINGLETON(RobotAgentManager);

RobotAgentManager::RobotAgentManager(){
    std::cout << "RobotAgentManager() is called." << std::endl;
}
RobotAgentManager::~RobotAgentManager(){
    std::cout << "~RobotAgentManager() is called." << std::endl;
}


bool RobotAgentManager::Init(){
    std::cout << "RobotAgentManager is initialized." << std::endl;
    return true;
}

void RobotAgentManager::Uinit(){
    std::cout << "RobotAgentManager is uninitialized." << std::endl;
}

void RobotAgentManager::Dojob(){

}