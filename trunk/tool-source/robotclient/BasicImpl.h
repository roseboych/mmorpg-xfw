#pragma once

#include <prolib/core_type.h>
#include <prolib/BasicProtocol.h>

#include "TestCase.h"
#include <map>
#include <string>

#include "MsgNotify.h"

class BasicImpl
{
public:
	BasicImpl( TestCase *pcase);
	virtual ~BasicImpl(void);

public:
	static ULONGLONG GetMillisecondTime();

public:
	virtual bool islogon(){ return userid_ != NO_INITVALUE;}
	virtual void logon();
	virtual void do_lgs( BasicProtocol *p) =0;
	virtual void do_gts( BasicProtocol *p) =0;

	virtual void status_check( ULONGLONG now) =0;

	virtual bool send_lgs_protocol( BasicProtocol *p){
		if( p == 0)
			return false;

		pcase_->send_lgs_protocol( p);
		delete p;

		return true;
	}

	virtual bool send_gts_protocol( BasicProtocol *p){
		if( p == 0)
			return false;

		pcase_->send_gts_protocol( p);
		delete p;

		return true;
	}

	void set_namepwd( int nid){
		user_name_.Format( "%d", nid);
		user_pwd_ =user_name_;
	}

	void clear();
	bool is_error(){ return error_code_ != NO_INITVALUE;};
	S_INT_32 get_errorcode(){ return error_code_;};

	void recv_logonack( BasicProtocol *p);
	void recv_logoutack( BasicProtocol *p);

	void reconnect_2_lgs();

protected:
	virtual bool is_force_close(){ return false; };

public:
	S_INT_32	userid_;
	S_INT_32	chrid_;
	S_INT_32	validatetime_;
	S_INT_16	lastregion_;
	S_INT_32	lastlogout_;
	S_INT_8		is_lnkkeep_;
	NS_STL::string gts_ip_;
	S_INT_16	gts_port_;
	S_TIMESTAMP	token_;
	S_INT_32	proxy_index_;
	S_TIMESTAMP	session_;

	ULONGLONG	record_time_;

protected:
	TestCase	*pcase_;
	S_INT_32	error_code_;

	CString		user_name_;
	CString		user_pwd_;

	TEST_STATE test_state_;
};

extern void post_error( MsgNotify *msg);
