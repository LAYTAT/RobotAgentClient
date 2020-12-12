//
// Created by laylei on 2020/12/11.
//

#ifndef ROBOTCLIENT_ROBOTAGENTMANAGER_H
#define ROBOTCLIENT_ROBOTAGENTMANAGER_H

#include "Singleton.h"
#include "Epoll.h"
#include "baselink.h"
#include "MesgHead.h"

class RobotAgentManager {
private:
    RobotAgentManager();
    ~RobotAgentManager();
    DECLARE_SINGLETON(RobotAgentManager);

public:
    bool Init();
    void Uinit();
    void Dojob();
    INT32 connectToLoginServer(INT32 client_port);
    INT32 connectToGateServer(INT32 client_port);
    inline void SendMsg(const MesgInfo& msg_info, Message& msg, const INT32 connfd) {m_epoll.SendMsg(msg_info, msg, connfd);}
private:
    INT32 m_login_fd;
    INT32 m_gate_fd;
    Epoll m_epoll;
    MesgHead* m_msg_head;
    baselink* m_ListenSock_Login;
    baselink* m_ListenSock_Gate;



};
#endif //ROBOTCLIENT_ROBOTAGENTMANAGER_H
