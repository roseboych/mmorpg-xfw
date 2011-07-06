#pragma once

#include <prolib/core_type.h>
#include <prolib/BasicProtocol.h>

//robot
#define ROBOT_BASE	20000

//取得机器人配置 c to s
#define RBT_CONFIG_REQ			(ROBOT_BASE + 0)
//取得机器人配置回复 s to c
#define RBT_CONFIG_ACK			(ROBOT_BASE + 1)
//开始机器人登录 s to c
#define RBT_START_ACK			(ROBOT_BASE + 2)
//停止机器人运行 s to c
#define RBT_STOP_ACK			(ROBOT_BASE + 3)
//取得机器人状态 c to s
#define RBT_STATE_RPT			(ROBOT_BASE + 4)

#define ROBOT_VERSION 2

typedef enum TEST_TYPE
{
	T_TYPE_UNKNOW = 0,					//未知
	T_LOGIN_LOGOUT = 1,					//正常登录,退出
	T_LOGIN_SELSVRTEAM,					//正常选择服务器组
	T_LOGIN_SELROLE,					//正常选择角色
	T_LOGIN_SVRTEAM_MOVE,				//地图内移动
};

namespace ROBOT_PROTOCOL
{
	class RobotProFactory
	{
	public:
		RobotProFactory(void);
		virtual ~RobotProFactory(void);

	public:
		static BasicProtocol* data_protocol( char *pdata, S_INT_32 len, void* ext =0);
	};

	class Robot_Config_Req : public AppProtocol<Robot_Config_Req>
	{
		typedef AppProtocol<Robot_Config_Req> inherit;
	public:
		Robot_Config_Req();

	protected:
		BEGIN_LOAD_PRO( pdata, totlen, ext)
			LOAD_INT32_PRO( pdata, totlen, version_)
			LOAD_STR_PRO( pdata, totlen, robotname_)
			LOAD_STR_PRO( pdata, totlen, robotpwd_)
		END_LOAD_PRO()

		BEGIN_SAVE_PRO( pdata, buflen, len, ext)
			SAVE_INT32_PRO( pdata, buflen, len, version_)
			SAVE_STR_PRO( pdata, buflen, len, robotname_)
			SAVE_STR_PRO( pdata, buflen, len, robotpwd_)
		END_SAVE_PRO()

		BEGIN_CLONE_PRO( Robot_Config_Req, proo)
			CLONE_VAR_PRO( proo, version_)
			CLONE_VAR_PRO( proo, robotname_)
			CLONE_VAR_PRO( proo, robotpwd_)
		END_CLONE_PRO()

	public:
		//机器人协议版本
		int version_;	
		NS_STL::string robotname_;
		NS_STL::string robotpwd_;
	};


	class Robot_Config_Ack : public AppProtocol<Robot_Config_Ack>
	{
		typedef AppProtocol<Robot_Config_Ack> inherit;
	public:
		Robot_Config_Ack();
		
	protected:
		BEGIN_LOAD_PRO( pdata, totlen, ext)
			LOAD_INT32_PRO( pdata, totlen, version_)
			LOAD_INT32_PRO( pdata, totlen, ret_)
			LOAD_INT32_PRO( pdata, totlen, robotid_)
			LOAD_INT32_PRO( pdata, totlen, startuserid_)
			LOAD_INT32_PRO( pdata, totlen, users_)
			LOAD_STR_PRO( pdata, totlen, gatewayip_)
			LOAD_INT32_PRO( pdata, totlen, gatewayport_)
		END_LOAD_PRO()

		BEGIN_SAVE_PRO( pdata, buflen, len, ext)
			SAVE_INT32_PRO( pdata, buflen, len, version_)
			SAVE_INT32_PRO( pdata, buflen, len, ret_)
			SAVE_INT32_PRO( pdata, buflen, len, robotid_)
			SAVE_INT32_PRO( pdata, buflen, len, startuserid_)
			SAVE_INT32_PRO( pdata, buflen, len, users_)
			SAVE_STR_PRO( pdata, buflen, len, gatewayip_)
			SAVE_INT32_PRO( pdata, buflen, len, gatewayport_)
		END_SAVE_PRO()

		BEGIN_CLONE_PRO( Robot_Config_Ack, proo)
			CLONE_VAR_PRO( proo, version_)
			CLONE_VAR_PRO( proo, ret_)
			CLONE_VAR_PRO( proo, robotid_)
			CLONE_VAR_PRO( proo, startuserid_)
			CLONE_VAR_PRO( proo, users_)
			CLONE_VAR_PRO( proo, gatewayip_)
			CLONE_VAR_PRO( proo, gatewayport_)
		END_CLONE_PRO()

	public:
		//机器人协议版本
		int version_;		
		//0 成功,-1没有此机器人,-2密码错,-3其他错误, -4版本错误
		int ret_;
		//只有ret == 0 时,才有下面选项
		//机器人在服务器的唯一标识
		int robotid_;
		int startuserid_;
		int users_;
		NS_STL::string gatewayip_;
		int gatewayport_;	
	};


	class Robot_Start_Ack : public AppProtocol<Robot_Start_Ack>
	{
		typedef AppProtocol<Robot_Start_Ack> inherit;
	public:
		Robot_Start_Ack();
		
	protected:
		BEGIN_LOAD_PRO( pdata, totlen, ext)
			LOAD_INT32_PRO( pdata, totlen, startusernum_)
			LOAD_INT32_PRO( pdata, totlen, logininterval_)
			LOAD_INT32_PRO( pdata, totlen, testtype_)
		END_LOAD_PRO()

		BEGIN_SAVE_PRO( pdata, buflen, len, ext)
			SAVE_INT32_PRO( pdata, buflen, len, startusernum_)
			SAVE_INT32_PRO( pdata, buflen, len, logininterval_)
			SAVE_INT32_PRO( pdata, buflen, len, testtype_)
		END_SAVE_PRO()
	
		BEGIN_CLONE_PRO( Robot_Start_Ack, proo)
			CLONE_VAR_PRO( proo, startusernum_)
			CLONE_VAR_PRO( proo, logininterval_)
			CLONE_VAR_PRO( proo, testtype_)
		END_CLONE_PRO()

	public:
		//开始的用户数,不能大于配置里的 users
		int startusernum_;
		//两个用户间登录时间间隔,单位(毫秒)
		int logininterval_;
		//机器人类型,参照 TEST_TYPE
		int testtype_;
	};


	class Robot_Stop_Ack : public AppProtocol<Robot_Stop_Ack>
	{
		typedef AppProtocol<Robot_Stop_Ack> inherit;
	public:
		Robot_Stop_Ack();

	protected:
		BEGIN_LOAD_PRO( pdata, totlen, ext)
		END_LOAD_PRO()

		BEGIN_SAVE_PRO( pdata, buflen, len, ext)
		END_SAVE_PRO()

		BEGIN_CLONE_PRO( Robot_Stop_Ack, proo)
		END_CLONE_PRO()

	public:
	};


	class Robot_State_Rpt : public AppProtocol<Robot_State_Rpt>
	{
		typedef AppProtocol<Robot_State_Rpt> inherit;
	public:
		Robot_State_Rpt();

		typedef struct ACTION_GROUP
		{
			unsigned int actions_;		//动作总数
			unsigned int succeeds_;		//操作成功总数
			unsigned int faileds_;		//操作失败总数
			unsigned int timeouts_;		//操作超时总数
			unsigned int timeinterval_;	//操作超时时间,单位毫秒
		};

		typedef struct STAT_DATA {
			//登录
			ACTION_GROUP logon_;
			ACTION_GROUP getrole_;
			ACTION_GROUP selrole_;
			ACTION_GROUP getchannel_;
			ACTION_GROUP selchannel_;
			ACTION_GROUP getroom_;
			ACTION_GROUP loginroom_;
			ACTION_GROUP logoutroom_;
			ACTION_GROUP logoutchannel_;
			ACTION_GROUP logout_;
			STAT_DATA()
			{
				memset( this, 0, sizeof(STAT_DATA));
			}
		};

	protected:
		BEGIN_LOAD_PRO( pdata, totlen, ext)
			LOAD_INT32_PRO( pdata, totlen, robotid_)
		END_LOAD_PRO()

		BEGIN_SAVE_PRO( pdata, buflen, len, ext)
			SAVE_INT32_PRO( pdata, buflen, len, robotid_)
		END_SAVE_PRO()

		BEGIN_CLONE_PRO( Robot_State_Rpt, proo)
			CLONE_VAR_PRO( proo, robotid_)
		END_CLONE_PRO()

	public:
		//被回复的用户ID
		int robotid_;
		//测试统计数据
		STAT_DATA stat_data_;
	};

	class Robot_State_Ack : public AppProtocol<Robot_State_Ack>
	{
		typedef AppProtocol<Robot_State_Ack> inherit;
	public:
		Robot_State_Ack();
		
	protected:
		BEGIN_LOAD_PRO( pdata, totlen, ext)
			LOAD_INT32_PRO( pdata, totlen, userid_)
		END_LOAD_PRO()

		BEGIN_SAVE_PRO( pdata, buflen, len, ext)
			SAVE_INT32_PRO( pdata, buflen, len, userid_)
		END_SAVE_PRO()

		BEGIN_CLONE_PRO( Robot_State_Ack, proo)
			CLONE_VAR_PRO( proo, userid_)
		END_CLONE_PRO()

	public:
		//回复的用户ID
		int userid_;

	};
}
