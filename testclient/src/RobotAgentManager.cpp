//
// Created by laylei on 2020/12/11.
//

#include "RobotAgentManager.h"
#include <iostream>
#include "EventSystem.h"

#define NUMER_OF_ROBOTS 10

INSTANCE_SINGLETON(RobotAgentManager);

RobotAgentManager::RobotAgentManager(){
    //初始化epoll
    if(!m_epoll.Init())
    {
        std::cout << "RobotAgentManager error: Epoll add ev failed!" << std::endl;
        return;
    }
    INT32 agents_num = NUMER_OF_ROBOTS;
    if(agents_num <= 0) {
        std::cout << "Constructing RobotAgentManager failed." << std::endl;
        return;
    }
    socketfd_player = new std::unordered_map<INT32, RobotAgent*>();
    tcp_map = new std::unordered_map<INT32, baselink*>();
//    this->m_ListenSock_Login = new baselink();
//    this->m_ListenSock_Gate = new baselink();
    this->m_msg_head = new MesgHead();

    RobotAgent* robot_agent_temp = nullptr;
    for(int i = 0 ; i < agents_num; i++) {
        robot_agent_temp = new RobotAgent();
        m_epoll.EpollAdd(robot_agent_temp->get_login_conn()->GetFD());
        socketfd_player->insert({ robot_agent_temp->get_login_conn()->GetFD(), robot_agent_temp });
    }
//    m_epoll.EpollAdd()
}
RobotAgentManager::~RobotAgentManager(){
//    delete m_ListenSock_Login;
//    delete m_ListenSock_Gate;
    if (socketfd_player != nullptr) {
        delete socketfd_player;
        socketfd_player = nullptr;
    }
    if (tcp_map != nullptr) {
        delete tcp_map;
        tcp_map = nullptr;
    }
    delete m_msg_head;
}


bool RobotAgentManager::Init(){
//    m_epoll.EpollAdd()
//    if (!m_ListenSock_Login->Init(-1)) {
//        std::cout << "login sock init failed." << std::endl;
//        return -1;
//    }
//    if (!m_ListenSock_Gate->Init(-1)) {
//        std::cout << "gate sock init failed." << std::endl;
//        return -1;
//    }
    return true;
}

void RobotAgentManager::Uinit(){
    m_epoll.Uinit();
    m_msg_head->Uinit();
//    m_ListenSock_Gate->Uinit();
//    m_ListenSock_Login->Uinit();
}

/*
 * @目的：新建连接到 server
 * @返回：-1代表失败，0代表成功。*/
INT32 RobotAgentManager::connectToLoginServer(baselink* which_server_connection){

//    if(which_server_connection->OpenClient(client_port) == -1) {
//        std::cout << "Open client for server failed." << std::endl;
//        return -1;
//    }

    INT32 login_ret = which_server_connection->ConnectServer(LOGIN_SERVER_PORT,LOGIN_SERVER_IP_ADDR);
    if(login_ret < 0){
        std::cout << "connect to server failed.";
    }

    m_epoll.EpollAdd(which_server_connection->GetFD());
//    if(m_epoll.EpollAdd(m_login_fd) == -1 ) {
//        std::cout << "\nEpoll add fd:" << m_login_fd << " failed" << std::endl;
//        perror("perror:");
//    }

    return 0;
}

///*
// * @目的：新建连接到gate server
// * @返回：-1代表失败，0代表成功。*/
//INT32 RobotAgentManager::connectToGateServer(INT32 client_port){
//
//    if(m_ListenSock_Gate->OpenClient(client_port) == -1) {
//        std::cout << "Open client for gate server failed." << std::endl;
//        return -1;
//    }
//
//    INT32 gate_ret = this->m_ListenSock_Gate->ConnectServer(GATE_SERVER_PORT,GATE_SERVER_IP_ADDR);
//    if(gate_ret < 0){
//        std::cout << "connect to gate server failed.";
//    }
//    m_gate_fd = m_ListenSock_Gate->GetFD();
//    m_epoll.EpollAdd(m_gate_fd);
//    return 0;
//}

[[noreturn]] void RobotAgentManager::Working() {
    INT32 ready_size = 0;
    while (true) {
        ready_size = m_epoll.EpollWait(100);
        std::unordered_map<INT32, RobotAgent*>* socketfd_player_map = get_socketfd_player();
        auto fd_player_itr = socketfd_player_map->begin();
        int len = socketfd_player_map->size();
        for (int i = 0 ; i < len ; ++i) {
            //对于初始状态的robot
            if(fd_player_itr -> second -> get_state() == RobotAgentEnum::INIT){
                std::string name = std::string("jack"+std::to_string(i+1));
                std::string pwd = name; // 目前的数据库中id与pwd一致
                fd_player_itr->second->agent_login(name.c_str(), pwd.c_str());
            } else if (fd_player_itr -> second ->get_state() == RobotAgentEnum::LOGGED_IN){
                fd_player_itr -> second ->agent_state_update();
            }
            fd_player_itr++;
        }

//        RobotAgent* temp_player_agent;
//        for (int i = 0; i < ready_size; i++)
//        {
//            struct epoll_event* event = get_epoll()->get_event(i);
//
//            if (sockfd_player_map->find(event->data.fd) != sockfd_player_map->end())
//            {
//                temp_player_agent = sockfd_player_map->at(event->data.fd);
//                TCPSocket* soc;
//                if (temp_player_agent->get_state() == LoginState || temp_player_agent->get_state() == WaitState)
//                {
//                    soc = temp_player_agent->get_login_conn();
//                }
//                else if (temp_player_agent->get_state() == ActiveState)
//                {
//                    soc = temp_player_agent->get_conn();
//                }
//                else
//                {
//                    std::cout << "state is not correct!" << std::endl;
//                }
//
//                int rec_len;
//                ProtocalMsg * msg;
//                int times = 0;
//                while (true)
//                {
//                    rec_len = soc->recv_data();
//                    SocketBuffer * buff = soc->get_socketbuff();
//                    std::cout << "return size:" << rec_len << "  readable len: " << buff->get_readable_size() << " start:"
//                              << buff->get_start() << " end:" << buff->get_end() << std::endl;
//
//                    if (buff->get_readable_size() >= sizeof(int32_t) * 3)
//                    {
//                        msg = buff->decode();
//                        if (buff->get_readable_size() >= (sizeof(int32_t) * 3 + msg->payload_size))
//                        {
//                            buff->get_msg_payload(msg);
//                            buff->set_start_by_len((sizeof(int32_t) * 3 + msg->payload_size));
//                            temp_player_agent->msg_handler(msg);
//
//                            if (temp_player_agent->get_login_conn() == soc)
//                            {
//                                int32_t fd = soc->get_socket_fd();
//                                TCPSocket * temp_con = temp_player_agent->get_conn();
//                                epoll->epoll_add(temp_con->get_socket_fd());
//                                sockfd_player_map->insert({ temp_con->get_socket_fd(), temp_player_agent});
//                                epoll->epoll_del(temp_player_agent->get_login_conn()->get_socket_fd());
//                                temp_player_agent->close_login();
//                                sockfd_player_map->erase(fd);
//                            }
//
//
//                            break;
//                        }
//                        if (msg != nullptr)
//                        {
//                            delete msg;
//                            msg = nullptr;
//                        }
//                    }
//                    times++;
//                    if (times > 100)
//                    {
//                        std::cout << "Network is bad!" << std::endl;
//                        break;
//                    }
//
//                }
//
//
//            }
//        }
    }
    return;
}

void RobotAgentManager::Dojob()
{
    std::cout << "Robot Agent Manager: Dojob start!" << std::endl;
    INT32 ready_size = 0;
    while (true)
    {

        // 改变robot agent的状态
        std::unordered_map<INT32, RobotAgent*>* socketfd_player_map = get_socketfd_player();
        auto fd_player_itr = socketfd_player_map->begin();
        int len = socketfd_player_map->size();
        for (int i = 0 ; i < len ; ++i) {
            //对于初始状态的robot
            if(fd_player_itr -> second -> get_state() == RobotAgentEnum::INIT){
                std::string name = std::string("jack"+std::to_string(i+1));
                std::string pwd = name; // 目前的数据库中id与pwd一致
                fd_player_itr->second->agent_login(name.c_str(), pwd.c_str());
            } else if (fd_player_itr -> second ->get_state() == RobotAgentEnum::LOGGED_IN){
                fd_player_itr -> second ->agent_state_update();
            }
            fd_player_itr++;
        }

        // 网络部分
        ready_size = m_epoll.EpollWait(-1);
        if(ready_size < 0){
            std::cout << "ready_size < 0, errno:"<< errno << std::endl;
            perror("epollwait");
            break;
        }

        for (int i = 0; i < ready_size; i++)
        {
            struct epoll_event* t_epev = m_epoll.GetEvent(i);
            INT32 rdyfd = t_epev->data.fd;

            baselink* t_linker = m_epoll.GetLinkerByfd(rdyfd);
            if (t_linker == nullptr)
                {
                    std::cout << "Can't find client socket in linkmap, connfd = "<< rdyfd << std::endl;
                    continue;
                }
            INT32 ret = t_linker->RecvData();
            if (ret == 0)
                {
                    continue;
                }
            else if (ret == -1)
                {
                    m_epoll.EpollRemove(rdyfd);
                    // 前面已经确认过connfd一定存在
                    continue;
                }

            INT32 t_packlen = t_linker->GetPackLens();
            while ( t_packlen != -1 )
                {
                    //获得一整个包，注意这里才改变buffer的头指针
                    char *str = t_linker->GetPack(t_packlen);
                    //判断第一个包头的信息
                    const MesgInfo t_msginfo =t_linker->GetMsginfo();

                    //获得msgID绑定的函数，然后来处理对应的MSG
                    auto func = EventSystem::Instance()->GetMsgHandler()->GetMsgFunc(t_msginfo.msgID);
                    if (NULL != func)
                    {
                        //调用处理函数
                        (EventSystem::Instance()->*func)(t_msginfo, str, t_msginfo.packLen, rdyfd);
                    }
                    t_packlen = t_linker->GetPackLens();
                }

        }
    }
}