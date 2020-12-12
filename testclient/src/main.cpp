
#include <iostream>
#include "SocketServer.h"
#include "EventSystem.h"
#include "RobotAgentManager.h"
// 顺序建立单例类
void CreatSingletons(){
//    CREATE_SINGLETON(SocketServer)
    CREATE_SINGLETON(EventSystem)
    CREATE_SINGLETON(RobotAgentManager)
}

// 逆序清除单例类
void DestroySingletons(){
    DESTORY_SINGLETON(RobotAgentManager)
    DESTORY_SINGLETON(EventSystem)
//    DESTORY_SINGLETON(SocketServer)
}

int main()
{
    CreatSingletons();
    // RobotAgentManager::Instance()->Dojob();
    // 连接到login server
    RobotAgentManager::Instance()->connectToLoginServer(9001);
    // 连接到gate server
    RobotAgentManager::Instance()->connectToGateServer(9002);
    DestroySingletons();
    return 1;
}
