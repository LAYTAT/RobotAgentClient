﻿// -------------------------------------------------------------------------------
// THIS FILE IS ORIGINALLY GENERATED BY THE DESIGNER.
// YOU ARE ONLY ALLOWED TO MODIFY CODE BETWEEN '///<<< BEGIN' AND '///<<< END'.
// PLEASE MODIFY AND REGENERETE IT IN THE DESIGNER FOR CLASS/MEMBERS/METHODS, ETC.
// -------------------------------------------------------------------------------

#include "RobotAgent.h"
#include <iostream>
#include <string>

///<<< BEGIN WRITING YOUR CODE FILE_INIT

///<<< END WRITING YOUR CODE

RobotAgent::RobotAgent()
{
///<<< BEGIN WRITING YOUR CODE CONSTRUCTOR

///<<< END WRITING YOUR CODE
}

RobotAgent::~RobotAgent()
{
///<<< BEGIN WRITING YOUR CODE DESTRUCTOR

///<<< END WRITING YOUR CODE
}

void RobotAgent::SayHello()
{
///<<< BEGIN WRITING YOUR CODE SayHello
    std::cout << "Hello" << std::endl;
///<<< END WRITING YOUR CODE
}


///<<< BEGIN WRITING YOUR CODE FILE_UNINIT
void RobotAgent::SetRobotEntity(RobotEntity* robot_entity)
{
    robot_entity_ = robot_entity;
}
///<<< END WRITING YOUR CODE
