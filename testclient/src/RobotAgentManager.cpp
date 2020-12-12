//
// Created by laylei on 2020/12/11.
//

#include "RobotAgentManager.h"
#include <iostream>
#include "EventSystem.h"

INSTANCE_SINGLETON(RobotAgentManager);

RobotAgentManager::RobotAgentManager(){
    this->m_ListenSock_Login = new baselink();
    this->m_ListenSock_Gate = new baselink();
    this->m_msg_head = new MesgHead();
}
RobotAgentManager::~RobotAgentManager(){
    delete m_ListenSock_Login;
    delete m_ListenSock_Gate;
    delete m_msg_head;
}


bool RobotAgentManager::Init(){
    if (!m_ListenSock_Login->Init(-1)) {
        std::cout << "login sock init failed." << std::endl;
        return -1;
    }
    if (!m_ListenSock_Gate->Init(-1)) {
        std::cout << "gate sock init failed." << std::endl;
        return -1;
    }
    return true;
}

void RobotAgentManager::Uinit(){
    m_epoll.Uinit();
    m_msg_head->Uinit();
    m_ListenSock_Gate->Uinit();
    m_ListenSock_Login->Uinit();
}

void RobotAgentManager::Dojob(){

}

/*
 * @目的：新建连接到login server
 * @返回：-1代表失败，0代表成功。*/
INT32 RobotAgentManager::connectToLoginServer(INT32 client_port){

    if(m_ListenSock_Login->OpenClient(client_port) == -1) {
        std::cout << "Open client for login server failed." << std::endl;
        return -1;
    }

    INT32 login_ret = this->m_ListenSock_Login->ConnectServer(LOGIN_SERVER_PORT,LOGIN_SERVER_IP_ADDR);
    if(login_ret < 0){
        std::cout << "connect to login server failed.";
    }
    m_login_fd = m_ListenSock_Login->GetFD();
    m_epoll.EpollAdd(m_login_fd);
    return 0;
}

/*
 * @目的：新建连接到gate server
 * @返回：-1代表失败，0代表成功。*/
INT32 RobotAgentManager::connectToGateServer(INT32 client_port){

    if(m_ListenSock_Gate->OpenClient(client_port) == -1) {
        std::cout << "Open client for gate server failed." << std::endl;
        return -1;
    }

    INT32 gate_ret = this->m_ListenSock_Gate->ConnectServer(GATE_SERVER_PORT,GATE_SERVER_IP_ADDR);
    if(gate_ret < 0){
        std::cout << "connect to gate server failed.";
    }
    m_gate_fd = m_ListenSock_Gate->GetFD();
    m_epoll.EpollAdd(m_gate_fd);
    return 0;
}