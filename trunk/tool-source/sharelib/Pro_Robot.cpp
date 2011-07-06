#include "pro_robot.h"

namespace ROBOT_PROTOCOL
{
	RobotProFactory::RobotProFactory(void)
	{
	}

	RobotProFactory::~RobotProFactory(void)
	{
	}

	BasicProtocol* RobotProFactory::data_protocol( char *pdata, S_INT_32 len, void* ext)
	{
		S_INT_32 iid = *(S_INT_32 *)pdata;
		BasicProtocol* ret = 0;

		switch( iid) 
		{
		case RBT_CONFIG_REQ:
			ret = new Robot_Config_Req();
			break;

		case RBT_CONFIG_ACK:
			ret = new Robot_Config_Ack();
			break;

		case RBT_START_ACK:
			ret = new Robot_Start_Ack();
			break;

		case RBT_STOP_ACK:
			ret = new Robot_Stop_Ack();
			break;

		case RBT_STATE_RPT:
			ret = new Robot_State_Rpt();
			break;

		default:
			break;
		}

		if ( ret)
		{
			try
			{
				ret->format( pdata, len, (void*)0, false);
			}
			catch (...)
			{
				return 0;
			}
		}

		return ret;
	}

	//-------------------------------------------- Robot_Config_Req begin ----------------------------------------
	Robot_Config_Req::Robot_Config_Req():inherit( RBT_CONFIG_REQ)
	{
	}

	//-------------------------------------------- Robot_Config_Req end   ----------------------------------------

	//-------------------------------------------- Robot_Config_Ack begin ----------------------------------------
	Robot_Config_Ack::Robot_Config_Ack():inherit( RBT_CONFIG_ACK)
	{
	}

	//-------------------------------------------- Robot_Config_Ack end   ----------------------------------------

	//-------------------------------------------- Robot_Start_Ack begin ----------------------------------------
	Robot_Start_Ack::Robot_Start_Ack():inherit( RBT_START_ACK)
	{
	}

	//-------------------------------------------- Robot_Start_Ack end   ----------------------------------------

	//-------------------------------------------- Robot_Stop_Ack begin ----------------------------------------
	Robot_Stop_Ack::Robot_Stop_Ack():inherit( RBT_STOP_ACK)
	{
	}

	//-------------------------------------------- Robot_Stop_Ack end   ----------------------------------------

	//-------------------------------------------- Robot_State_Rpt begin ----------------------------------------
	Robot_State_Rpt::Robot_State_Rpt():inherit( RBT_STATE_RPT)
	{
	}

	//-------------------------------------------- Robot_State_Rpt end   ----------------------------------------
}
