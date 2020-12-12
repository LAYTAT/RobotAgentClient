//
// Created by laylei on 2020/12/11.
//

#include "RobotAgent.h"
#include "RobotAgentManager.h"
#include <iostream>
#include <unistd.h>
#include <cmath>
#include "MsgID.pb.h"
#include "MSG_PLAYER_MOVE.pb.h"

RobotAgent::RobotAgent() {
    // 连接到login server
    login_conn = new baselink();
    gate_conn = new baselink();

    this->Init();

    if(login_conn->OpenClient(9001) == -1) {
        std::cout << "open client failed" << std::endl;
    }

    // 初始化玩家状态
    character_state = RobotAgentEnum::INIT;
    pos_x = 100 + (rand() % 7);
    pos_y = 0;
    pos_z = 50 + (rand() % 7);
    pos_ry = 0;
}

bool RobotAgent::Init(){
    if (!login_conn->Init(-1)) {
        std::cout << "login sock init failed." << std::endl;
        return -1;
    }
    if (!gate_conn->Init(-1)) {
        std::cout << "gate sock init failed." << std::endl;
        return -1;
    }
}

//int32_t RobotAgent::msg_handler(ProtocalMsg *msg)
//{
//    if (msg == nullptr)
//    {
//        std::cout << "msg is null!" << std::endl;
//        return -1;
//    }
//
//    if (msg->data == nullptr)
//    {
//        std::cout << "msg->data is null!" << std::endl;
//        return -2;
//    }
//    std::cout << "msg_ID=" << msg->msg_id << "uid=" << msg->user_id << " len=" << msg->payload_size << std::endl;
//
//    if (msg->msg_id == Login)
//    {
//        if (state != LoginState)
//        {
//            std::cout << "state is not correct! state=" << state << std::endl;
//            return -2;
//        }
//        GameSpec::CtlMsgLoginRsp *login_rsp = new GameSpec::CtlMsgLoginRsp();
//        login_rsp->ParseFromArray(msg->data, msg->payload_size);
//        if (login_rsp->errcode() != (int32_t)Error_Socket_OK)
//        {
//            std::cout << "msg:error code=" << login_rsp->errcode() << std::endl;
//            return login_rsp->errcode();
//        }
//
//        conn = new TCPSocket();
//        conn->open_as_client(1023);
//        int32_t re_state = conn->connect_to(login_rsp->ip().c_str(), login_rsp->port(), true, 100);
//        if (re_state == 0)
//        {
//            state = ActiveState;
//        }
//
//        if (state == ActiveState)
//        {
//            std::cout << "state is active!" << std::endl;
////			delete login_conn;
////			login_conn = nullptr;
//        }
//        id = msg->user_id;
//        return re_state;
//    }
//    else
//    {
//        // ignore other massage
//        return 0;
//    }
//
//}

void RobotAgent::Uninit(){
    login_conn->Uinit();
    gate_conn->Uinit();
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

//INT32 RobotAgent::

INT32 RobotAgent::agent_state_update(){
    if(character_state != RobotAgentEnum::LOGGED_IN){
        std::cout << "\nPlayer state is not right, current state = " << character_state << std::endl;
    }

    MSG_PLAYER_MOVE playerMoveState = MSG_PLAYER_MOVE();
    int delta_x = (rand() % 7) - 1;
    int delta_z = (rand() % 7) - 1;

    pos_x += delta_x;
    pos_z += delta_z;

    pos_ry = (int)(atan2(delta_x, delta_z)*180/acos(-1));

    playerMoveState.set_playerid(player_id);
    playerMoveState.set_z(pos_z);
    playerMoveState.set_x(pos_x);
    playerMoveState.set_ry(pos_ry);

    int32_t pstate = 0;
    int e = rand() % 100;
    if (delta_x != 0)
    {
        pstate |= GameSpec::Up;
    }

    if (delta_z != 0)
    {
        pstate |= GameSpec::Left;
    }
    if (e >= 98)
    {
        pstate |= GameSpec::E;
    }
    playerMoveState.set_state(pstate);
    std::cout << "\nplayer current state update : x= " << pos_x
    << " z= " << pos_z
    << " ry= " << pos_ry << " state="
    << " state = " << character_state << std::endl;
    usleep(20000);
    return 0;
}

INT32 RobotAgent::agent_login(const char *username, const char *password) {

    RobotAgentManager::Instance()->connectToLoginServer(this->get_login_conn());

    if(character_state != RobotAgentEnum::INIT) {
        std::cout << "Player current state is not initialized" << std::endl;
        return -1;
    }
//    if(login_conn->ConnectToLoginServer() < 0) {
//        std::cout << "connect to login failed" << std::endl;
//        return -1;
//    }

    GameSpec::CtlMsgLoginReq login_req = GameSpec::CtlMsgLoginReq();
    login_req.set_name(username);
    login_req.set_password(password);

    MesgInfo* msginfo_to_login = new MesgInfo();
    msginfo_to_login->msgID = MSGID::MSG_PLAYER_LOGIN;
    msginfo_to_login->uID = 0; // 暂时用户id未知所以置零
    msginfo_to_login->packLen = login_req.ByteSizeLong();
    RobotAgentManager::Instance()->SendMsgToLogin(this,*msginfo_to_login, login_req);
    delete msginfo_to_login;
    character_state = RobotAgentEnum::LOGGING_IN;

    return 0;
}

baselink* RobotAgent::get_login_conn() {
    return login_conn;
}

baselink* RobotAgent::get_gate_conn() {
    return gate_conn;
}

int32_t RobotAgent::get_state()
{
    return character_state;
}

int32_t RobotAgent::close_conn()
{
    delete login_conn;
    login_conn = nullptr;
    return 0;
}
