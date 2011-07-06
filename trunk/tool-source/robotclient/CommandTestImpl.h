#pragma once

#include <prolib/ProtocolFactory.h>
#include <prolib/Pro_all.h>
#include <sharelib/ScriptContext.h>

#include <string>

USE_PROTOCOL_NAMESPACE

#define MAX_PACK_LEN 16*1024
#define SINGLE_PACK_LEN 1024*8

class CommandTestImpl
{
public:
	CommandTestImpl( );
	virtual ~CommandTestImpl(void);

    void set_wnd( HWND h){ parent_wnd_ =h;};

public:
	int RunLuaShellCmd( std::string& cmd);
    bool IsScriptInit();

	//lgs socket
	bool send_to_lgs( BasicProtocol *p);
	BasicProtocol* recv_from_lgs();
	bool connect_to_lgs();
	void disconnect_to_lgs();

	//gts socket
	bool send_to_gts( BasicProtocol* p);
	BasicProtocol* recv_from_gts();
	bool connect_to_gts();
	void disconnect_to_gts();

	std::string get_status();

	bool islogon();
	bool isselsvrteam();

	//实现的脚本功能
    void logon( const char* name, const char* pwd);
    void logout();
	//选服
	void selsvrteam( int index);
	void quitteam();
	//角色处理
	void getchrlist();
	void addonechr( const char* n, short s, short race, short profession);
	void selchr( int cid);
	void delchr( int cid);
	//选线
	void teleport( int v1, int v2);
	void moveto( int x, int y, int z);
	void petfollow( int petid);
	void petback();
	void mapchat( const char* msg);
	
public:
	//状态信息
	CString		ret_desc_;

	//连接信息
	std::string lgs_ip_;
	int			lgs_port_;

	S_INT_32	svr_index_;
	std::string gts_ip_;
	int			gts_port_;
	S_TIMESTAMP	gts_token_;

	S_TIMESTAMP	reconn_token_;

public:
	//1:lgs 2:gts 3:lgs+gts
	S_INT_8		thread_step_;

	NS_STL::string username_;
	S_INT_32	userid_;
	S_INT_32	chrid_;
	S_FLOAT_32	x_, y_, z_;

private:
    void InitScriptBind();
    void startThread();
    void stopThread();
    
    static DWORD  WINAPI Thread(LPVOID  lparam);

	void lgs_linkdo();
	void gts_linkdo();

private:
	ScriptContext context_;

	ProtocolFactory *data_;
    HANDLE	hthd;
    DWORD	dwThdId;
    //bool    lock_;
    HWND    parent_wnd_;

	//lgs socket
	SOCKET	socket_;
	char recv_buffer_[MAX_PACK_LEN];
	int	 data_len_;

	//gts
	SOCKET	socket2_;
	char gts_recv_buffer_[MAX_PACK_LEN];
	int	 gts_data_len_;

	//测试用时间戳
	static S_TIMESTAMP tmptime_;
};
