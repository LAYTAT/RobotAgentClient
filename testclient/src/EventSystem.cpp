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

    // �󶨴�����
    m_msgHandler->RegisterMsg(MSGID::MSG_LOGIN_REPLIY_CLIENT, &EventSystem::OnLoginReply);

    return true;
}

void EventSystem::Uinit()
{
    m_msgHandler->Uinit();
    delete m_msgHandler;
    m_msgHandler = nullptr;
    std::cout << "EventSystem is uninitialized." << std::endl;

}

/*@Ŀ�ģ�������������صĵ�¼��Ϣ��֤
 *@���أ�����Ļ�����-1�������Ļ�����0 */
INT32 EventSystem::OnLoginReply(const MesgInfo &stHead, const char *body, const INT32 len,const INT32 connfd){
    std::cout << "\n successfully recved login reply" << count++ << std::endl;
    GameSpec::LoginRep login_reply_from_server;


    if(!login_reply_from_server.ParseFromArray(body, len))
    {
        std::cout << "\nParseFromArray player login_server failed !!!" <<std::endl;
        return -1;
    }
    if(login_reply_from_server.errcode() != GameSpec::ErrorCode::ERROR_NO_ERROR) {
        std::cout << "\nlogin server reply error" << std::endl;
        return -1;
    }
    // ��֤ͨ���Ļ���
    // ����ȡ�����ص���Ϣ
    if (!RobotAgentManager::Instance()->IsGateSet()) {
        RobotAgentManager::Instance()->SetGate(login_reply_from_server.gate_ip(),login_reply_from_server.gate_port(),login_reply_from_server.session_code());
    }

}

