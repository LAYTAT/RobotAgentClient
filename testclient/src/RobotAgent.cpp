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

    if(login_conn->OpenClient() == -1) {
        std::cout << "open client failed" << std::endl;
    }

    // 初始化玩家状态
    state = RobotAgentEnum::INIT;
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

INT32 RobotAgent::msg_handler(const MesgInfo* msg_info, char * str)
{
    if (msg_info == nullptr)
    {
        std::cout << "Error: msg head is null!" << std::endl;
        return -1;
    }

    if (str == nullptr)
    {
        std::cout << "Error: msg data is null!" << std::endl;
        return -2;
    }
    std::cout
    << " msg_ID="<< msg_info->msgID
    << " uid=" << msg_info->uID
    << " len=" << msg_info->packLen << std::endl;

    if (msg_info->msgID == MSGID::MSG_LOGIN_REPLIY_CLIENT)
    {
        if (state != RobotAgentEnum::LOGGING_IN)
        {
            std::cout << "Error: state is not correct! state=" << state << std::endl;
            return -2;
        }
        GameSpec::LoginRep *login_rsp = new GameSpec::LoginRep;
        login_rsp->ParseFromArray(str, msg_info->packLen);
        if (login_rsp->errcode() != GameSpec::ErrorCode::ERROR_NO_ERROR)
        {
            std::cout << "errno： msg error code=" << login_rsp->errcode() << std::endl;
            return login_rsp->errcode();
        }

        gate_conn = new baselink();
        // gate open_as_client
        if(gate_conn->OpenClient() == -1) {
            std::cout << "open client failed" << std::endl;
        }
        int32_t re_state = gate_conn->ConnectServer(login_rsp->gate_port(), login_rsp->gate_ip().c_str());
        if (re_state == 0)
        {
            state = RobotAgentEnum::LOGGED_IN;
        }

        if (state == RobotAgentEnum::LOGGED_IN)
        {
            std::cout << "state is active!" << std::endl;
//			delete login_conn;
//			login_conn = nullptr;
        }
        player_id = msg_info->uID;
        return re_state;
    }
    else
    {
        // ignore other massage
        return 0;
    }

}

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
    state = RobotAgentEnum::OTHER_STATE;
}

//INT32 RobotAgent::

INT32 RobotAgent::agent_state_update(){
    if(state != RobotAgentEnum::LOGGED_IN){
        std::cout << "\nPlayer state is not right, current state = " << state << std::endl;
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
              << " state = " << state << std::endl;
    usleep(20000);
    return 0;
}

INT32 RobotAgent::agent_login(const char *username, const char *password) {

    RobotAgentManager::Instance()->connectToLoginServer(this->get_login_conn());

    if(state != RobotAgentEnum::INIT) {
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
    state = RobotAgentEnum::LOGGING_IN;

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
    return state;
}

int32_t RobotAgent::close_login()
{
    delete login_conn;
    login_conn = nullptr;
    return 0;
}
