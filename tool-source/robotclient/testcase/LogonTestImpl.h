#pragma once

#include <prolib/core_type.h>
#include <prolib/Pro_all.h>

#include "../BasicImpl.h"

class TestCase;

class LogonTestImpl : public BasicImpl
{
public:
	LogonTestImpl( TestCase *p);
	virtual ~LogonTestImpl(void);
	
	virtual void do_lgs( BasicProtocol *p);
	virtual void do_gts( BasicProtocol *p);

	virtual void status_check( ULONGLONG now);

public:
	ULONGLONG	reconn_time_;
};
