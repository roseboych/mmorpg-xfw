#pragma once

#include <prolib/ProtocolFactory.h>
#include <prolib/Pro_all.h>

#define MAX_PACK_LEN 16*1024
#define SINGLE_PACK_LEN 1024*8

typedef enum TEST_STATE
{
	TS_NOLOGON = 0,
	TS_LOGONNING,
	TS_LOGON_SUCCEED,
	TS_LOGON_FAILED,
	TS_RELOGONING,
	TS_RELOGONING_FAILED,
	TS_LOGOUTING,
	TS_SVRTEAM_SELECTING,
	TS_SELSVRTEAM_SUCCEED,
	TS_SELSVRTEAM_FAILED,
	TS_CONNECTGTS_SUCCEED,
	TS_CONNECTGTS_FAILED,
	TS_LOGONGTS_SUCCEED,
	TS_LOGONGTS_FAILED,
	TS_SVRTEAM_QUITING,
	TS_SVRTEAMQUIT_SUCCESS,
	TS_SVRTEAMQUIT_FAILED,
	TS_ROLE_LISTING,
	TS_ROLELIST_SUCCEED,
	TS_ROLELIST_FAILED,
	TS_ROLE_SELECTING,
	TS_SELROLE_SUCCEED,
	TS_SELROLE_FAILED,
};

class BasicImpl;

class TestCase
{
public:
	TestCase();
	virtual ~TestCase(void);

	void run_it( ULONGLONG now);
	void start_case();
	void stop_case();

	void send_lgs_protocol( BasicProtocol *p);
	void recv_lgs_protocol();

	void send_gts_protocol( BasicProtocol *p);
	void recv_gts_protocol();

	bool force_close();

	void set_userid( int id);

	void set_cancel(){ cancel_ =true;};

	void set_delay( ULONGLONG t){ delay_connect_ =t;};

	bool connect_2_lgs( int& err);
	bool connect_2_gts( int& err);

	void disconnect_2_lgs();
	void disconnect_2_gts();

	void reconnect_2_lgs();

	void wait_socket_close();

protected:
	bool	cancel_;

	int		userid_;

	SOCKET	lgs_socket_;
	char	lgs_recv_buffer_[MAX_PACK_LEN];
	int		lgs_data_len_;

	SOCKET	gts_socket_;
	char	gts_recv_buffer_[MAX_PACK_LEN];
	int		gts_data_len_;

	bool	need_connect_2_lgs_;
	ULONGLONG	start_connect_t_;
	ULONGLONG	delay_connect_;

	BasicImpl	*impl_;
	ProtocolFactory *data_;
};
