/**
* Client Net Module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "DemoGame_Impl.h"

#include "prolib/Pro_all.h"

#include "../testcasedef.h"
#include "../NetModule.h"

//-----------------------------------------DemoGame_Impl-----------------------------------------------
DemoGame_Impl::DemoGame_Impl(void)
{
}

DemoGame_Impl::~DemoGame_Impl(void)
{
}


std::string DemoGame_Impl::get_name()
{
	return TESTCASE_DEMO;
}

void DemoGame_Impl::deal_echo( const BasicProtocol *pb)
{
	if( pb == 0)
		return;

	if( pb->iid_ == GAME_LOGIN_REQ)
	{
		//登陆请求
		PRO::Pro_Login_req* req =(PRO::Pro_Login_req*)(pb);
		this->username_ =req->token_.c_str();

		PRO::Pro_Login_ack* ack =new PRO::Pro_Login_ack();
		ack->result_ =0;

		NetModule::instance().offline_sendtoclient( ack);
	}
}

void DemoGame_Impl::case_tick()
{

}

