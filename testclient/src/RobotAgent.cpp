//
// Created by laylei on 2020/12/11.
//

#include "RobotAgent.h"
#include <iostream>
#include <unistd.h>
#include <cmath>
#include "MsgID.pb.h"

RobotAgent::RobotAgent() {
    // 连接到login server
    login_conn = new baselink();
    if(login_conn->OpenClient(8888) < 0) {
        std::cout << "open client failed" << std::endl;
    }

    // 初始化玩家状态
    character_state = RobotAgentEnum::INIT;
    pos_x = 100 + (rand() % 7);
    pos_y = 0;
    pos_z = 50 + (rand() % 7);
    pos_ry = 0;
}

RobotAgent::~RobotAgent() {
    if (login_conn != nullptr)
    {
        delete login_conn;
        login_conn = nullptr;
    }

    if (gate_conn != nullptr)
    {
        delete gate_conn;
        gate_conn = nullptr;
    }
    character_state = RobotAgentEnum::OTHER_STATE;
}

baselink* RobotAgent::get_login_conn() {
    return login_conn;
}

baselink* RobotAgent::get_gate_conn() {
    return gate_conn;
}


INT32 RobotAgent::agent_login(const char *username, const char *password) {
    if(character_state != RobotAgentEnum::INIT) {
        std::cout << "Player current state is not initialized" << std::endl;
        return -1;
    }
    if(login_conn->ConnectToLoginServer() < 0) {
        std::cout << "connect to login failed" << std::endl;
        return -1;
    }

    char data[1024];
    GameSpec::CtlMsgLoginReq login_req = GameSpec::CtlMsgLoginReq();
    login_req.set_name(username);
    login_req.set_password(password);

    MesgInfo* msginfo_to_login = new MesgInfo();
    msginfo_to_login->msgID = MSGID::MSG_PLAYER_LOGIN;
    msginfo_to_login->uID = 0; // 暂时用户id未知所以置零
    msginfo_to_login->packLen = login_req.ByteSizeLong();
//    SocketServer::Instance()->SendMsgToLoginServer();
//
//    SocketBuffer::encode(data, 9999, -1, login_req.ByteSize());
//    login_req.SerializeToArray(data + sizeof(int32_t) * 3, login_req.ByteSize());
//    login_conn->send_data(data, login_req.ByteSize() + sizeof(int32_t) * 3);
//    state = LoginState;

    return 0;
}