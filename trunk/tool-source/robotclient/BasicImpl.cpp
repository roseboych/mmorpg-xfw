#include "StdAfx.h"

#include "basicimpl.h"

#include <prolib/Pro_all.h>

USE_PROTOCOL_NAMESPACE;

BasicImpl::BasicImpl( TestCase *pcase)
:pcase_( pcase),
userid_( NO_INITVALUE),
error_code_( NO_INITVALUE),
test_state_(TS_NOLOGON),
record_time_( 0)
{
}

BasicImpl::~BasicImpl(void)
{
}

void BasicImpl::logon()
{
	if( test_state_ != TS_NOLOGON)
		return;

	Pro_Login_req *req =new Pro_Login_req();
	std::string str =(LPCTSTR)user_name_;
	str +=":";
	str +=(LPCTSTR)user_pwd_;
	req->token_ =str;

	send_lgs_protocol( req);

	test_state_ = TS_LOGONNING;

	record_time_ =GetMillisecondTime();
}

void BasicImpl::reconnect_2_lgs()
{
	test_state_ =TS_NOLOGON;
	this->userid_ =NO_INITVALUE;

	pcase_->disconnect_2_lgs();
	pcase_->reconnect_2_lgs();
}

ULONGLONG BasicImpl::GetMillisecondTime()
{
	FILETIME stime={0};
	GetSystemTimeAsFileTime( &stime);
	ULONGLONG ll =*((PULONGLONG)&stime);
	return ll/10000;
}

void BasicImpl::clear()
{
	userid_ =NO_INITVALUE;
}

void BasicImpl::recv_logonack( BasicProtocol *p)
{
	error_code_ =NO_INITVALUE;

	Pro_Login_ack *ack =dynamic_cast<Pro_Login_ack*>(p);
	if( ack->result_ == 0)
	{
		this->userid_ =ack->userid_;
		this->validatetime_ =ack->validatetime_;
		this->lastregion_ =ack->lastregion_;
		this->lastlogout_ =ack->lastlogout_;
		this->is_lnkkeep_ =ack->is_lnkkeep_;
		this->gts_ip_ =ack->gts_ip_.c_str();
		this->gts_port_ =ack->gts_port_;
		this->token_ =ack->token_;
		this->proxy_index_ =ack->proxy_index_;
		this->session_ =ack->session_;
	}
	else
	{
		this->userid_ =NO_INITVALUE;

		error_code_ =ack->result_;
	}
}

void BasicImpl::recv_logoutack( BasicProtocol *p)
{
	error_code_ =NO_INITVALUE;

	/*
	Pro_UserLogout_ack *ack =dynamic_cast<Pro_UserLogout_ack*>(p);
	if( ack->ret_ == 0)
	{
		clear();
	}
	else
	{
		error_code_ =ack->ret_;
	}
	*/
}
