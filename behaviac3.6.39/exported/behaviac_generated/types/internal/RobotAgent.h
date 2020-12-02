﻿// -------------------------------------------------------------------------------
// THIS FILE IS ORIGINALLY GENERATED BY THE DESIGNER.
// YOU ARE ONLY ALLOWED TO MODIFY CODE BETWEEN '///<<< BEGIN' AND '///<<< END'.
// PLEASE MODIFY AND REGENERETE IT IN THE DESIGNER FOR CLASS/MEMBERS/METHODS, ETC.
// -------------------------------------------------------------------------------

#ifndef _BEHAVIAC_ROBOTAGENT_H_
#define _BEHAVIAC_ROBOTAGENT_H_

#include "behaviac_headers.h"

///<<< BEGIN WRITING YOUR CODE FILE_INIT
class RobotEntity;
///<<< END WRITING YOUR CODE

class RobotAgent : public behaviac::Agent
///<<< BEGIN WRITING YOUR CODE RobotAgent
///<<< END WRITING YOUR CODE
{
public:
	RobotAgent();
	virtual ~RobotAgent();

	BEHAVIAC_DECLARE_AGENTTYPE(RobotAgent, behaviac::Agent)

	public: void SayHello();

///<<< BEGIN WRITING YOUR CODE CLASS_PART
public:
    void SetRobotEntity(RobotEntity* robot_entity);
private:
	RobotEntity* robot_entity_ = nullptr;
///<<< END WRITING YOUR CODE
};

///<<< BEGIN WRITING YOUR CODE FILE_UNINIT

///<<< END WRITING YOUR CODE

#endif
