
#include <iostream>
#include "SocketServer.h"
#include "EventSystem.h"
#include "RobotAgentManager.h"
#include <signal.h>

// SIGPIPE ignore handler
void sigpipe_ignore_handler(int ignore){
    perror("sigpipe ignored ! 忽略了一次SIGPIPE");
}

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

//    signal(SIGPIPE, sigpipe_ignore_handler);

    CreatSingletons();

    RobotAgent* ra = new RobotAgent();


    ra->agent_login("jack89","123456");

    RobotAgentManager::Instance()->Dojob();

    DestroySingletons();
    return 1;
}
