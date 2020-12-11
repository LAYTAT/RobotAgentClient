//
// Created by laylei on 2020/12/5.
//

#include "EventSystem.h"
#include "MsgID.pb.h"
#include "MSG_PLAYER_MOVE.pb.h"
#include "MSG_PLAYER_REGISTER.pb.h"
#include "SocketServer.h"
#include "GameSpec.pb.h"
#include "Msg_To_And_From_DB.pb.h"
#include "MsgServerID.pb.h"

using namespace google::protobuf;

INSTANCE_SINGLETON(EventSystem)

EventSystem::EventSystem()
{
    std::cout << "EventSystem() is called." << std::endl;
    m_msgHandler = nullptr;
    m_msgHandler = new MsgHandler<EventSystem>();
}

EventSystem::~EventSystem()
{
    std::cout << "~EventSystem() is called." << std::endl;

}

bool EventSystem::Init()
{
    std::cout << "EventSystem is initialized." << std::endl;

    // 绑定处理函数
//    m_msgHandler->RegisterMsg(MSGID::MSG_TEST_ID, &EventSystem::PlayerRegister);

    return true;
}

void EventSystem::Uinit()
{
    m_msgHandler->Uinit();
    delete m_msgHandler;
    m_msgHandler = nullptr;
    std::cout << "EventSystem is uninitialized." << std::endl;

}


