#include "robot_entity.h"
#include <iostream>

std::list<RobotEntity*> RobotEntity::robot_list_;

RobotEntity::RobotEntity(
    uint32_t id, 
    const std::string& name, 
    asio::io_service& pio_service, 
    uint32_t timer_elapse_milliseconds) :
    id_(id),
    name_(name),
    timer_(pio_service),
    timer_elapse_milliseconds_(timer_elapse_milliseconds)
{
    //启动定时器
    timer_.expires_from_now(std::chrono::milliseconds(0));
    timer_.async_wait([this](const asio::error_code& error) 
        {
            this->OnTimer(error); 
        }
    );
}

bool RobotEntity::Init(const std::string& bt_name)
{
    // 创建机器人实体
    p_robot_bt_ = behaviac::Agent::Create<RobotAgent>(name_.c_str());
    if (!p_robot_bt_)
    {
        return false;
    }

    // 导入行为树
    bool bRet = p_robot_bt_->btload(bt_name.c_str());
    if (!bRet)
    {
        return false;
    }

    // 指定当前准备执行的行为树
    p_robot_bt_->btsetcurrent(bt_name.c_str());

    p_robot_bt_->SetRobotEntity(this);

    return true;
}

bool RobotEntity::UnInit()
{
    if (!p_robot_bt_)
    {
        return true;
    }

    behaviac::Agent::Destroy(p_robot_bt_);

    return true;
}

void RobotEntity::OnTimer(const asio::error_code& error)
{
    //每一帧调用
    if (!error)
    {
        //每一帧处理
        OnUpdate();

        //自己触发下一帧 
        timer_.expires_from_now(std::chrono::milliseconds(timer_elapse_milliseconds_));
        timer_.async_wait([this](const asio::error_code& error) {this->OnTimer(error); });
    }
}

void RobotEntity::OnUpdate()
{
    //执行行为树
    if (p_robot_bt_)
    {
        std::cout << "Robot name: " + name_ << ", id: " << id_ << ", tick time: " << ++tick_time_ << std::endl;
        p_robot_bt_->btexec();
    }
}