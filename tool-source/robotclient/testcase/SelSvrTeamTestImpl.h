#pragma once

#include <prolib/core_type.h>
#include <prolib/Pro_all.h>

#include "../BasicImpl.h"

class TestCase;

class SelSvrTeamTestImpl : public BasicImpl
{
public:
	SelSvrTeamTestImpl( TestCase *p);
	virtual ~SelSvrTeamTestImpl(void);
	
	virtual void logon();

	virtual void do_lgs( BasicProtocol *p);
	virtual void do_gts( BasicProtocol *p);

	virtual void status_check( ULONGLONG now);

public:
	S_TIMESTAMP	reconn_token_;
	S_INT_32		reconn_userid_;

	ULONGLONG	tttmp_;
};
