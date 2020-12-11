
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
    std::cout << "你好" << std::endl;
    DestroySingletons();
    return 1;
}
