//
// Created by laylei on 2020/12/11.
//

#ifndef ROBOTCLIENT_ROBOTAGENT_H
#define ROBOTCLIENT_ROBOTAGENT_H

#include "Config.h"
#include "SocketServer.h"
#include "GameSpec.pb.h"
#include "RobotAgentEnum.h"
#include "MesgHead.h"

class RobotAgent{
private:
    INT32 player_id;
    INT32 pos_x;
    INT32 pos_y;
    INT32 pos_z;
    INT32 pos_ry;
    baselink* gate_conn;
    baselink* login_conn;
    RobotAgentEnum character_state;

public:
    RobotAgent();
    virtual ~RobotAgent();

    bool Init();
    void Uninit();

    baselink* get_gate_conn();
    baselink* get_login_conn();
    INT32 agent_login(const char* username, const char* password);
    INT32 agent_state_update();
    INT32 agent_msghandler(MesgHead* msg);
    INT32 get_character_state();
    INT32 close_conn();
};


#endif //ROBOTCLIENT_ROBOTAGENT_H
