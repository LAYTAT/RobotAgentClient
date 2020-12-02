#pragma once
#include <internal/RobotAgent.h>
#include <string>
#include <list>
#include "asio.hpp"

class RobotEntity
{
public:
    /**
     * @brief 机器人构造函数
     * @param name 机器人名字
     */
    RobotEntity(uint32_t id, const std::string& name, asio::io_service& pio_service, uint32_t timer_elapse_milliseconds);

    /**
     * @brief 机器人初始化
     * @param bt_name 行为树名
     * @return 是否成功
     */
    bool Init(const std::string& bt_name);

    /**
     * @brief 机器人销毁函数
     * @return 是否成功
    */
    bool UnInit();

    void OnTimer(const asio::error_code& error);

    void OnUpdate();

    /// 所有机器人
    static std::list<RobotEntity*> robot_list_;
private:
    /// 行为树实体
    RobotAgent* p_robot_bt_;
    /// 机器人Id
    uint32_t id_;
    /// 机器人名字
    std::string name_;
    /// 定时器
    asio::steady_timer timer_;
    /// 机器人心跳间隔
    uint32_t timer_elapse_milliseconds_;
    /// Update次数
    uint32_t tick_time_ = 0;
};