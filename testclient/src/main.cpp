#include <cstdio>
#include <iostream>
#include <string>
#include "behaviac/behaviac.h"
#include "internal/RobotAgent.h"
#include "robot_entity.h"
#include "asio.hpp"
#include <thread>

using asio::ip::tcp;

std::list<std::thread* > threadpool;
volatile bool main_threawd_exit = false;

void InitBehavic()
{
    behaviac::Workspace::GetInstance()->SetFilePath("exported");
    behaviac::Workspace::GetInstance()->SetFileFormat(behaviac::Workspace::EFileFormat::EFF_xml);
}

void InitPlayer(
    const std::string& tree_name, 
    const std::string& robot_name,
    uint32_t start_index,
    uint32_t end_index,
    asio::io_service& pio_service,
    uint32_t heart_tick)
{
    for (uint32_t i = start_index; i < end_index; i++)
    {
        RobotEntity* new_robot = new RobotEntity(i, robot_name + std::to_string(i), pio_service, heart_tick);
        if (!new_robot)
        {
            continue;
        }

        if (new_robot->Init(tree_name))
        {
            RobotEntity::robot_list_.push_back(new_robot);
        }
    }
}

void UnInitPlayer()
{
    auto iter = RobotEntity::robot_list_.begin();
    while (iter != RobotEntity::robot_list_.end())
    {
        auto* robot_entity = *iter;
        if (robot_entity)
        {
            robot_entity->UnInit();
            delete robot_entity;
        }
        iter = RobotEntity::robot_list_.erase(iter);
    }
}

void UnInitBehaviac()
{
    behaviac::Workspace::GetInstance()->Cleanup();
}

void asio_signal_handler(const asio::error_code& err, int sig)
{
    main_threawd_exit = true;
}

int main(int argc, char* argv[])
{
    // 定义行为树基本数据（可通过读XML或Json等配置文件实现）
    std::string tree_name = "SayHello";
    std::string robot_name = "TestRobot";
    uint32_t start_index = 0;
    uint32_t end_index = 2;
    uint32_t heart_tick = 1000;

    // 初始化行为树
    InitBehavic();

    //启动asio
    asio::io_service* pio_service = new asio::io_service();
    asio::signal_set sigset(*pio_service, SIGINT, SIGTERM);
    sigset.async_wait(asio_signal_handler);

    // 初始化机器人
    InitPlayer(tree_name, robot_name, start_index, end_index, *pio_service, heart_tick);

    std::thread* t = new std::thread([pio_service]()
        {
            pio_service->run();
        }
    );
    threadpool.push_back(t);

    while (!main_threawd_exit)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    pio_service->stop();

    for (auto* t : threadpool)
    {
        t->join();
    }

    // 销毁机器人
    UnInitPlayer();

    // 销毁行为树
    UnInitBehaviac();

    return 0;
}
