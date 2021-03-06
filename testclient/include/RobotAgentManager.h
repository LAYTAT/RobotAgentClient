//
// Created by laylei on 2020/12/11.
//

#ifndef ROBOTCLIENT_ROBOTAGENTMANAGER_H
#define ROBOTCLIENT_ROBOTAGENTMANAGER_H

#include "Singleton.h"
#include "Epoll.h"
#include "baselink.h"
#include "MesgHead.h"
#include "RobotAgent.h"
#include "unordered_map"

class RobotAgentManager {
private:
    RobotAgentManager();
    ~RobotAgentManager();
    DECLARE_SINGLETON(RobotAgentManager);

public:
    bool Init();
    void Uinit();

    [[noreturn]] void Working();
    void Dojob();
    inline Epoll* get_epoll() {return &m_epoll;}
    inline std::unordered_map<INT32, baselink*>* get_tcp_map() {return tcp_map;}
    inline std::unordered_map<INT32, RobotAgent*>* get_socketfd_player() {return socketfd_player;}

    INT32 connectToLoginServer(baselink* which_server_connection);
//    INT32 connectToGateServer(INT32 client_port);
    inline void SendMsg(const MesgInfo& msg_info, Message& msg, const INT32 connfd) {m_epoll.SendMsg(msg_info, msg, connfd);}
    inline void SendMsgToLogin(RobotAgent* robot_agent, const MesgInfo& msg_info, Message& msg) {
//        std::cout << "\nrobot_agent->get_login_conn()->GetFD() = " << robot_agent->get_login_conn()->GetFD()<<std::endl;
        if(robot_agent->get_login_conn()->GetFD() > 0) m_epoll.SendMsg(msg_info, msg, robot_agent->get_login_conn()->GetFD());
    }
    inline void SendMsgToGate(RobotAgent* robot_agent, const MesgInfo& msg_info, Message& msg) {
        if(robot_agent->get_gate_conn()->GetFD() > 0) m_epoll.SendMsg(msg_info, msg, robot_agent->get_gate_conn()->GetFD());
    }
    inline bool IsGateSet(){return is_gate_set;}
    inline void SetGate(string ip, INT32 port, INT32 session_code){gate_ip=ip;gate_port=port;gate_session_code=session_code;is_gate_set=true;}

private:
//    INT32 m_login_fd;
//    INT32 m_gate_fd;
    Epoll m_epoll;
    MesgHead* m_msg_head;
    std::unordered_map<INT32, baselink*>* tcp_map;
    std::unordered_map<INT32, RobotAgent*> * socketfd_player;
    bool is_gate_set=false;
    string gate_ip;
    INT32 gate_port;
    INT32 gate_session_code;
//    baselink* m_ListenSock_Login;
//    baselink* m_ListenSock_Gate;
//
};
#endif //ROBOTCLIENT_ROBOTAGENTMANAGER_H
