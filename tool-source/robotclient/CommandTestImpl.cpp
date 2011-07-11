#include "StdAfx.h"

#include "CommandTestImpl.h"
#include "logmacro.h"

USE_PROTOCOL_NAMESPACE;

S_TIMESTAMP CommandTestImpl::tmptime_ =0;

CommandTestImpl::CommandTestImpl():
hthd( 0),
dwThdId( 0),
parent_wnd_( 0),
data_len_( 0),
gts_data_len_( 0),
socket_( INVALID_SOCKET),
socket2_( INVALID_SOCKET)
{
	data_ =new ProtocolFactory();
	context_.init_scriptcontext();
	InitScriptBind();

	//init
	userid_ =NO_INITVALUE;
	chrid_ =NO_INITVALUE;
}

CommandTestImpl::~CommandTestImpl(void)
{
    stopThread();

	if( socket_ != INVALID_SOCKET)
	{
		::closesocket( socket_);
		socket_ =INVALID_SOCKET;
	}
	if( socket2_ != INVALID_SOCKET)
	{
		::closesocket( socket2_);
		socket2_ =INVALID_SOCKET;
	}

	context_.uninit_scriptcontext();   
	if( data_)
		delete data_;
}

int CommandTestImpl::RunLuaShellCmd( std::string& cmd)
{
	return context_.run_script( cmd.c_str()) == 0;
}

bool CommandTestImpl::IsScriptInit()
{
	return context_.get_luastate() != 0;
}

void CommandTestImpl::startThread()
{
	if( hthd)
		return;

	hthd=CreateThread(NULL,NULL, (LPTHREAD_START_ROUTINE)Thread, this,0,&dwThdId);
}

void CommandTestImpl::stopThread()
{
	if( hthd > 0)
	{
		TerminateThread( hthd, dwThdId);
		CloseHandle( hthd);
	}

	hthd =0;
	dwThdId =0;
}

bool CommandTestImpl::send_to_lgs( BasicProtocol *p)
{
	if( socket_ == INVALID_SOCKET)
		return false;

	char buf[SINGLE_PACK_LEN] ={0};
	char *pbuf =buf + sizeof( S_INT_32);

	S_INT_32 len =0;
	p->data( pbuf, SINGLE_PACK_LEN, len, (void*)0, false);
	*((S_INT_32*)buf) =len;

	int totlen =len + sizeof( S_INT_32);
	int len2 =0;
	pbuf =buf;

	while( ( len2 =::send( socket_, pbuf, totlen, 0)) != SOCKET_ERROR)
	{
		totlen -= len2;
		if( totlen <= 0)
			break;
		pbuf =pbuf + len2;
	}

	if( len2 == SOCKET_ERROR)
	{
		if( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::closesocket( socket_);
			socket_ =INVALID_SOCKET;
			//reset recvbuf
			data_len_ =0;

			return false;
		}
	}
	return true;
}

BasicProtocol* CommandTestImpl::recv_from_lgs()
{
	if( socket_ == INVALID_SOCKET)
		return 0;

	while( 1)
	{
		::Sleep( 1);

		char *pbuf =recv_buffer_+data_len_;
		int len =MAX_PACK_LEN - data_len_;

		int rlen =::recv( socket_, pbuf, len, 0);

		if( rlen == SOCKET_ERROR)
		{
			if( WSAGetLastError() != WSAEWOULDBLOCK)
			{
				::closesocket( socket_);
				socket_ =INVALID_SOCKET;
				//reset recvbuf
				data_len_ =0;

				CString *pstr =new CString();
				pstr->Format( "和lgs的socket连接断开\r\n");
				PostMessage( parent_wnd_, WM_USER+0x200, (WPARAM)pstr, 0);

				return false;
			}

			if( data_len_ < sizeof( S_INT_32))
				continue;
		}

		if( rlen >= 0)
			data_len_ += rlen;

		//分析现有数据
		while( 1)
		{
			if( data_len_ < sizeof( S_INT_32))
				break;
			pbuf =recv_buffer_;
			S_INT_32 dlen =*((S_INT_32*)pbuf);
			if( dlen > data_len_ - sizeof( S_INT_32))
				break;

			pbuf += sizeof( S_INT_32);
			BasicProtocol *ret =data_->data_protocol( pbuf, dlen, 0, false);
			bool bexit =false;
			if( ret)
			{
				//记录收到的协议
				//MODULE_LOG_DEBUG((MODULE_TEMP, "command test recv protocol:%d", ret->iid_));
				bexit =true;
			}

			//移动内存
			data_len_ -= sizeof( S_INT_32);
			data_len_ -= dlen;
			pbuf += dlen;
			memmove( &(recv_buffer_[0]), pbuf, data_len_);

			if( bexit)
				return ret;
		}
	}

	return 0;
}

void CommandTestImpl::disconnect_to_lgs()
{
	if( socket_ == INVALID_SOCKET)
		return;

	::closesocket( socket_);
	socket_ =INVALID_SOCKET;
}

void CommandTestImpl::disconnect_to_gts()
{
	if( socket2_ == INVALID_SOCKET)
		return;

	::closesocket( socket2_);
	socket2_ =INVALID_SOCKET;
}

bool CommandTestImpl::connect_to_lgs()
{
	socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if( socket_ == INVALID_SOCKET)
		return false;

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( lgs_ip_.c_str());
	serv_addr.sin_port			= htons( lgs_port_);

	if( ::connect( socket_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		::closesocket( socket_);
		socket_ =INVALID_SOCKET;
		return false;
	}
	else
	{        
		u_long ll =1;
		ioctlsocket( socket_, FIONBIO, &ll);
		ll =1024*8;
		ioctlsocket( socket_, FIONREAD, &ll);        
		return true;
	}
}


bool CommandTestImpl::send_to_gts( BasicProtocol *p)
{
	if( socket2_ == INVALID_SOCKET)
		return false;

	char buf[SINGLE_PACK_LEN] ={0};
	char *pbuf =buf + sizeof( S_INT_32);

	S_INT_32 len =0;
	p->data( pbuf, SINGLE_PACK_LEN, len, (void*)0, false);
	*((S_INT_32*)buf) =len;

	int totlen =len + sizeof( S_INT_32);
	int len2 =0;
	pbuf =buf;

	while( ( len2 =::send( socket2_, pbuf, totlen, 0)) != SOCKET_ERROR)
	{
		totlen -= len2;
		if( totlen <= 0)
			break;
		pbuf =pbuf + len2;
	}

	if( len2 == SOCKET_ERROR)
	{
		if( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			::closesocket( socket2_);
			socket2_ =INVALID_SOCKET;
			//reset recvbuf
			gts_data_len_ =0;

			return false;
		}
	}

	return true;
}

BasicProtocol* CommandTestImpl::recv_from_gts()
{
	if( socket2_ == INVALID_SOCKET)
		return 0;

	while( 1)
	{
		::Sleep( 1);

		char *pbuf =gts_recv_buffer_+gts_data_len_;
		int len =MAX_PACK_LEN - gts_data_len_;

		int rlen =::recv( socket2_, pbuf, len, 0);

		if( rlen == SOCKET_ERROR)
		{
			if( WSAGetLastError() != WSAEWOULDBLOCK)
			{
				::closesocket( socket2_);
				socket2_ =INVALID_SOCKET;
				gts_data_len_ =0;

				CString *pstr =new CString();
				pstr->Format( "和gts的socket连接断开\r\n");
				PostMessage( parent_wnd_, WM_USER+0x200, (WPARAM)pstr, 0);

				return false;
			}

			if( gts_data_len_ < sizeof( S_INT_32))
				continue;
		}

		if( rlen >= 0)
			gts_data_len_ += rlen;

		//分析现有数据
		while( 1)
		{
			if( gts_data_len_ < sizeof( S_INT_32))
				break;
			pbuf =gts_recv_buffer_;
			S_INT_32 dlen =*((S_INT_32*)pbuf);
			if( dlen > gts_data_len_ - sizeof( S_INT_32))
				break;

			pbuf += sizeof( S_INT_32);
			BasicProtocol *ret =data_->data_protocol( pbuf, dlen, 0, false);
			bool bexit =false;
			if( ret)
			{
				//记录收到的协议
				//MODULE_LOG_DEBUG((MODULE_TEMP, "command test recv protocol:%d", ret->iid_));
				bexit =true;
			}

			//移动内存
			gts_data_len_ -= sizeof( S_INT_32);
			gts_data_len_ -= dlen;
			pbuf += dlen;
			memmove( &(gts_recv_buffer_[0]), pbuf, gts_data_len_);

			if( bexit)
				return ret;
		}
	}

	return 0;
}

bool CommandTestImpl::connect_to_gts()
{
	socket2_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if( socket2_ == INVALID_SOCKET)
		return false;

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= inet_addr( gts_ip_.c_str());
	serv_addr.sin_port			= htons( gts_port_);

	if( ::connect( socket2_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
	{
		::closesocket( socket2_);
		socket2_ =INVALID_SOCKET;
		return false;
	}
	else
	{        
		u_long ll =1;
		ioctlsocket( socket2_, FIONBIO, &ll);
		ll =1024*8;
		ioctlsocket( socket2_, FIONREAD, &ll);        
		return true;
	}
}


DWORD  WINAPI CommandTestImpl::Thread(LPVOID  lparam)
{
	CommandTestImpl *pthis =reinterpret_cast<CommandTestImpl*>(lparam);
	while( 1)
	{
		::Sleep( 1);

		if( pthis->thread_step_ & 1)
			pthis->lgs_linkdo();
		if( pthis->thread_step_ & 2)
			pthis->gts_linkdo();
	}

	return 1;
}

void CommandTestImpl::InitScriptBind()
{
    luabind::module( context_.get_luastate())
	[
		luabind::class_< CommandTestImpl >( "CommandTest")
			.def( luabind::constructor<>())
			.def("logon", (void (CommandTestImpl::*)(const char*,const char*))&CommandTestImpl::logon)
			.def("logout", &CommandTestImpl::logout)
			.def("selsvrteam", (void (CommandTestImpl::*)(int))&CommandTestImpl::selsvrteam)
			.def("quitteam", (void (CommandTestImpl::*)(void))&CommandTestImpl::quitteam)
			.def("getchrlist", (void (CommandTestImpl::*)(void))&CommandTestImpl::getchrlist)
			.def("addonechr", (void (CommandTestImpl::*)(const char*, short, short, short))&CommandTestImpl::addonechr)
			.def("selchr", (void (CommandTestImpl::*)(int))&CommandTestImpl::selchr)
			.def("delchr", (void (CommandTestImpl::*)(int))&CommandTestImpl::delchr)
			.def("mapchat", (void (CommandTestImpl::*)(const char*))&CommandTestImpl::mapchat)
			.def("teleport", (void (CommandTestImpl::*)(int,int))&CommandTestImpl::teleport)
			.def("moveto", (void (CommandTestImpl::*)(int,int,int))&CommandTestImpl::moveto)
			.def("enterinst", (void (CommandTestImpl::*)(int))&CommandTestImpl::enterinst)
			.def("quitinst", (void (CommandTestImpl::*)(void))&CommandTestImpl::quitinst)
			.def("petfollow", (void (CommandTestImpl::*)(int))&CommandTestImpl::petfollow)
			.def("petback", (void (CommandTestImpl::*)(void))&CommandTestImpl::petback)

	];

	context_.regist_2_context<CommandTestImpl>( "run_cmd", this);
}

void CommandTestImpl::lgs_linkdo()
{
	if( socket_ == INVALID_SOCKET)
		return;

	BasicProtocol *recv =recv_from_lgs();
	if( recv == 0)
		return;

	std::auto_ptr<BasicProtocol> p(recv);

	CString *pstr =new CString();
	CString fm;
	switch( recv->iid_)
	{
	case GAME_LOGIN_ACK:
		{
			Pro_Login_ack *lack =dynamic_cast<Pro_Login_ack*>(recv);
			if( lack->result_ == 0)
				this->userid_ =lack->userid_;

			fm.Format( TEXT("登陆返回 ret:%d userid:%d\r\n"), lack->result_, lack->userid_);
			*pstr =fm;

			if( lack->result_ == 0 && lack->is_lnkkeep_ == 1)
			{
				this->gts_ip_ =lack->gts_ip_.c_str();
				this->gts_port_ =lack->gts_port_;
				this->gts_token_ =lack->token_;
				this->svr_index_ =lack->proxy_index_;

				fm.Format( "断线保持，重连模式\r\n");
				*pstr = fm + *pstr;

				//发起到gts的连接
				//开始连接gts
				if( !connect_to_gts())
				{
					fm.Format("连接gts失败\r\n");
					*pstr = fm + *pstr;
				}
				else
				{
					fm.Format("连接gts成功.................\r\n");
					*pstr = fm + *pstr;

					thread_step_ =3;
					//发送连接请求
					Pro_SvrSelTeamConfirm_req* req =new Pro_SvrSelTeamConfirm_req();
					req->proxy_index_ =svr_index_;
					req->token_ =gts_token_;

					if( !send_to_gts( req))
						*pstr = "发送Pro_SvrSelTeamConfirm_req 失败\r\n" + *pstr;
				}
			}
			break;
		}
	case SVR_SVRTEAMINFO_NTF:
		{
			Pro_SvrTeamInfo_NTF* ntf =dynamic_cast<Pro_SvrTeamInfo_NTF*>(recv);

			for( Pro_SvrTeamInfo_NTF::SVRTEAMINF_LIST::iterator iter=ntf->teams_.begin(); iter != ntf->teams_.end(); ++iter)
			{
				Pro_SvrTeamInfo_NTF::svrteaminfo& sinfo =(*iter);
				fm.Format( "服务器组[%d] validate[%d] maxuser[%d] curuser[%d]\r\n", sinfo.server_index_, sinfo.is_validate_, sinfo.maxuser_, sinfo.curuser_);
				*pstr += fm;
			}

			break;
		}
	case SVR_SELSERVER_ACK:
		{
			//选服返回
			Pro_SvrSelTeam_ack* lack =dynamic_cast<Pro_SvrSelTeam_ack*>(recv);
			if( lack->result_ == 0)
			{
				//保存服务器组
				svr_index_ =lack->proxy_index_;
				gts_ip_ =lack->gts_ip_.c_str();
				gts_port_ =lack->gts_port_;
				gts_token_ =lack->token_;
			}

			fm.Format( "选服请求返回[%d] ip:%s port:%d token:%u\r\n", lack->result_, lack->gts_ip_.c_str(), 
				lack->gts_port_, lack->token_);

			*pstr = fm;

			if( lack->result_ == 0)
			{
				//开始连接gts
				if( !connect_to_gts())
				{
					fm.Format("连接gts失败\r\n");
					*pstr += fm;
				}
				else
				{
					fm.Format("连接gts成功.................\r\n");
					*pstr += fm;

					thread_step_ =3;
					//发送连接请求
					Pro_SvrSelTeamConfirm_req* req =new Pro_SvrSelTeamConfirm_req();
					req->proxy_index_ =svr_index_;
					req->token_ =gts_token_;

					if( !send_to_gts( req))
						*pstr += "发送Pro_SvrSelTeamConfirm_req 失败\r\n";
				}

			}

			break;
		}
	case SVR_SELSERVERCONFIRM_ACK:
		{
			//选服确认
			Pro_SvrSelTeamConfirm_ack* lack =dynamic_cast<Pro_SvrSelTeamConfirm_ack*>(recv);
			if( lack->result_ == 0)
			{
				//选服成功
				thread_step_ =2;
				disconnect_to_lgs();
			}

			fm.Format( "lgs 选服确认返回[%d]\r\n", lack->result_);
			*pstr =fm;

			break;
		}
	case SVR_SELTEAMTIMEOUT_NTF:
		{
			fm.Format( "选服超时\r\n");
			*pstr =fm;

			break;
		}
	case SVR_QUITSERVERRECONN_ACK:
		{
			Pro_SvrQuitTeamReconn_ack* ack =dynamic_cast<Pro_SvrQuitTeamReconn_ack*>(recv);
			fm.Format( "退服重连请求返回[%d] \r\n", ack->result_);

			*pstr = fm;
			break;
		}
	default:
		{
			if( recv->iid_ !=0)
			{
				fm.Format( "未处理消息：%d\r\n",recv->iid_);
				*pstr +=fm;
			}         
			break;
		}
	}

	if ( pstr->GetLength() > 0)
		PostMessage( parent_wnd_, WM_USER+0x200, (WPARAM)pstr, 0);
	else
		delete pstr;
}

void CommandTestImpl::gts_linkdo()
{
	if( socket2_ == INVALID_SOCKET)
		return;

	BasicProtocol *recv =recv_from_gts();
	if( recv == 0)
		return;

	std::auto_ptr<BasicProtocol> p(recv);

	CString *pstr =new CString();
	CString fm;
	switch( recv->iid_)
	{
	case SVR_SELSERVERCONFIRM_ACK:
		{
			Pro_SvrSelTeamConfirm_ack *lack =dynamic_cast<Pro_SvrSelTeamConfirm_ack*>(recv);

			fm.Format( "gts 选服确认返回[%d]\r\n", lack->result_);
			*pstr =fm;
			break;
		}
	case GAME_CHRLIST_ACK:
		{
			Pro_ChrList_ack* lack =dynamic_cast<Pro_ChrList_ack*>(recv);
			
			fm.Format( "获取角色列表[%d]\r\n", lack->chrs_.size());
			*pstr =fm;

			for( Pro_ChrList_ack::CHR_LIST::iterator iter =lack->chrs_.begin(); iter != lack->chrs_.end(); ++iter)
			{
				Pro_ChrList_ack::chrinfo& c =(*iter);

				fm.Format( "角色[%d] name:%s level:%d race:%d profession:%d \r\n", c.chrid_, c.name_.c_str(), c.level_, c.race_, c.profession_);
				*pstr +=fm;
			}

			break;
		}
	case GAME_CHRCREATE_ACK:
		{
			Pro_ChrCreate_ack* lack =dynamic_cast<Pro_ChrCreate_ack*>(recv);

			fm.Format( "创建角色返回[%d]\r\n", lack->result_);
			*pstr =fm;

			if( lack->result_ == 0)
			{
				fm.Format("创建的角色信息[%d] name:%s level:%d race:%d profession:%d \r\n", lack->chrid_, lack->name_.c_str(), 
					lack->level_, lack->race_, lack->profession_);
				*pstr +=fm;
			}

			break;
		}
	case GAME_CHRSEL_ACK:
		{
			Pro_ChrSel_ack* lack =dynamic_cast<Pro_ChrSel_ack*>(recv);

			if( lack->result_ == 0)
				chrid_ =lack->chrid_;

			fm.Format( "选择角色返回[%d] 当前角色[%d] race:%d profession:%d\r\n", lack->result_, lack->chrid_, lack->race_, lack->profession_);
			*pstr =fm;

			break;
		}
	case GAME_CHRDEL_ACK:
		{
			Pro_ChrDel_ack* lack =dynamic_cast<Pro_ChrDel_ack*>(recv);

			if( lack->result_ == 0 && chrid_ == lack->chrid_)
				chrid_ =NO_INITVALUE;

			fm.Format( "删除角色返回[%d] 删除角色[%d] 当前角色[%d]\r\n", lack->result_, lack->chrid_, chrid_);
			*pstr =fm;

			break;
		}
	case AOI_TELEPORT_ACK:
		{
			Pro_AppTeleport_ack* ack =dynamic_cast<Pro_AppTeleport_ack*>(recv);
			fm.Format( "地图切换返回[%d]\r\n", ack->result_);
			*pstr =fm;

			break;
		}
	case GAME_LOGOUT_NTF:
		{
			Pro_Logout_ntf *ntf =dynamic_cast<Pro_Logout_ntf*>(recv);
			fm.Format( "玩家[%d]断线了\r\n", ntf->chrid_);
			*pstr =fm;

			break;
		}
	case SVR_QUITSERVER_ACK:
		{
			Pro_SvrQuitTeam_ack *ack =dynamic_cast<Pro_SvrQuitTeam_ack*>(recv);

			fm.Format( "退服返回[%d]\r\n", ack->result_);
			*pstr =fm;

			if( ack->result_ == 0)
			{
				//退服成功
				thread_step_ =1;

				this->reconn_token_ =ack->token_;

				this->disconnect_to_gts();

				this->chrid_ =NO_INITVALUE;
				this->username_ ="";
				
				if( !this->connect_to_lgs())
				{
					fm.Format( "连接到lgs服务器失败\r\n");
					*pstr += fm;
				}
				else
				{
					fm.Format( "重新连接到lgs服务器\r\n");
					*pstr += fm;

					Pro_SvrQuitTeamReconn_req* req =new Pro_SvrQuitTeamReconn_req();
					req->userid_ =userid_;
					req->token_ =reconn_token_;

					send_to_lgs( req);
				}
			}

			break;
		}
	case GAME_CHRLOAD_ACK:
		{
			Pro_ChrLoad_ack* ack =dynamic_cast<Pro_ChrLoad_ack*>(recv);
			fm.Format( "选线后返回玩家数据\r\n");
			*pstr =fm;

			fm.Format( "玩家背包数据\r\n");
			*pstr +=fm;
			for( Pro_ChrLoad_ack::EQUIPITEM_LIST::iterator iter =ack->items_.begin(); iter != ack->items_.end(); ++iter)
			{
				equipment_item& it =(*iter);
				fm.Format( "物品[%d] code:%d inavatar_:%d inpart_:%d \r\n", it.itemid_, it.itemcode_, it.inavatar_, it.inpart_);
				*pstr += fm;
			}

			fm.Format( "玩家宠物数据\r\n");
			*pstr +=fm;
			for( Pro_ChrLoad_ack::PETDATA_LIST::iterator iter =ack->pets_.begin(); iter != ack->pets_.end(); ++iter)
			{
				petdata_item& it =(*iter);
				fm.Format( "宠物[%d] code:%d name:%s \r\n", it.petid_, it.petcode_, it.petname_.c_str());
				*pstr += fm;
			}

			break;
		}
	case GAME_CHRFIN_NTF:
		{
			Pro_ChrFin_NTF* ack =dynamic_cast<Pro_ChrFin_NTF*>(recv);
			this->x_ =ack->baseinfo_.posx_;
			this->y_ =ack->baseinfo_.posy_;
			this->z_ =ack->baseinfo_.posz_;

			fm.Format( "选线后返回玩家数据-结束 pos(x:%f,y:%f,z:%f) race:%d profession:%d\r\n", 
				ack->baseinfo_.posx_, ack->baseinfo_.posy_, ack->baseinfo_.posz_, ack->baseinfo_.race_, ack->baseinfo_.profession_);
			*pstr =fm;

			break;
		}
	case AOI_UNITENTER_NTF:
		{
			Pro_AppUnitEnter_ntf* ntf =dynamic_cast<Pro_AppUnitEnter_ntf*>(recv);
			for( std::list< Pro_AppUnitEnter_ntf::player_info>::iterator iter =ntf->players_.begin(); iter != ntf->players_.end(); ++iter)
			{
				Pro_AppUnitEnter_ntf::player_info& pinfo =(*iter);
				fm.Format( "[%d][%s]进入我的视野 race:%d profession:%d\r\n", pinfo.chrid_, pinfo.nickname_.c_str(), pinfo.race_, pinfo.profession_);

				*pstr += fm;
			}

			break;
		}
	case AOI_UNITLEAVE_NTF:
		{
			Pro_AppUnitLeave_ntf* ntf =dynamic_cast<Pro_AppUnitLeave_ntf*>(recv);
			for( std::list< Pro_AppUnitLeave_ntf::player_info>::iterator iter =ntf->players_.begin(); iter != ntf->players_.end(); ++iter)
			{
				Pro_AppUnitLeave_ntf::player_info& pinfo =(*iter);
				fm.Format( "[%d]离开我的视野\r\n", pinfo.chrid_);

				*pstr += fm;
			}

			break;
		}
	case AOI_ENTERINS_ACK:
		{
			Pro_AppEnterIns_ack* ack =dynamic_cast<Pro_AppEnterIns_ack*>( recv);

			fm.Format( "进入副本回复 result:%d\r\n", ack->result_);
			*pstr += fm;

			break;
		}
	case AOI_QUITINS_ACK:
		{
			Pro_AppQuitInst_ack* ack =dynamic_cast<Pro_AppQuitInst_ack*>( recv);

			fm.Format( "退出副本回复 result:%d\r\n", ack->result_);
			*pstr += fm;

			break;
		}
	case APP_PLAYERMOVE_NTF:
		{
			Pro_PlayerMove_ntf* ntf =dynamic_cast<Pro_PlayerMove_ntf*>(recv);
			fm.Format( "玩家[%d][%d]移动到[x:%f,y:%f,z:%f] 状态[%d]\r\n", ntf->chrid_,
				ntf->locationx_, ntf->locationy_, ntf->locationz_, ntf->curstate_);
			*pstr =fm;
			break;
		}
		/*
	case AOI_UNITMOVE_ACK:
		{
			Pro_AppUnitMove_ack* ack =dynamic_cast<Pro_AppUnitMove_ack*>(recv);
			fm.Format( "本人移动到[x:%f,y:%f,z:%f] 失败\r\n", ack->locationx_, ack->locationy_, ack->locationz_);
			*pstr =fm;

			break;
		}
		*/
	case APP_PETSUMMON_ACK:
		{
			Pro_PetSummon_ack* ack =dynamic_cast<Pro_PetSummon_ack*>(recv);
			fm.Format( "宠物:%d 召唤返回[ret:%d]\r\n", ack->petid_, ack->result_);
			*pstr =fm;

			break;
		}
	case APP_PETSUMMON_NTF:
		{
			Pro_PetSummon_ntf* ntf =dynamic_cast<Pro_PetSummon_ntf*>(recv);

			fm.Format( "玩家[%d] 的宠物 %s[%d] 被召唤\r\n", ntf->chrid_, ntf->petname_.c_str(), ntf->petcode_);
			*pstr =fm;

			break;
		}
	case APP_TEAMCHAT_NTF:
		{
			Pro_TeamChat_ntf* ntf =dynamic_cast<Pro_TeamChat_ntf*>(recv);

			fm.Format( "收到玩家[%d][%s] 的聊天消息:%s\r\n", ntf->chrid_, ntf->nick_.c_str(), ntf->msg_.c_str());
			*pstr =fm;
			break;
		}
	default:
		{
			if( recv->iid_ !=0)
			{
				fm.Format( "未处理消息：%d\r\n",recv->iid_);
				*pstr +=fm;
			}         
			break;
		}
	}

	if ( pstr->GetLength() > 0)
		PostMessage( parent_wnd_, WM_USER+0x200, (WPARAM)pstr, 0);
	else
		delete pstr;
}

std::string CommandTestImpl::get_status()
{
	std::string ret ="";
	if( userid_ == NO_INITVALUE)
	{
		ret ="未登陆\r\n";
		return ret;
	}

	CString fm;
	fm.Format( "用户[%s / %d] chrid[%d] 已经登陆\r\n", username_.c_str(), userid_, chrid_);
	ret =(LPCTSTR)fm;

	return ret;
}

bool CommandTestImpl::islogon()
{
	return userid_ != NO_INITVALUE;
}

bool CommandTestImpl::isselsvrteam()
{
	return chrid_ != NO_INITVALUE;
}

void CommandTestImpl::logon( const char* name, const char* pwd)
{
	ret_desc_ ="";

	if( socket_ == INVALID_SOCKET)
	{
		if( !connect_to_lgs())
		{
			ret_desc_ ="lgs socket打开错误，服务器可能没有运行！\r\n";
			return;
		}
	}

	username_ =name;
	//deal lgs
	thread_step_ =1;

	startThread();

	Pro_Login_req *req =new Pro_Login_req();
	std::string str =name;
	str +=":";
	str +=pwd;
	req->token_ =str;

	if( !send_to_lgs( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::logout()
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_Logout_req *req =new Pro_Logout_req();
	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::selsvrteam( int index)
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_SvrSelTeam_req* req =new Pro_SvrSelTeam_req();
	req->team_index_ =index;

	if( !send_to_lgs( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::quitteam()
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_SvrQuitTeam_req* req =new Pro_SvrQuitTeam_req();

	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::getchrlist()
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_ChrList_req* req =new Pro_ChrList_req();
	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::addonechr( const char* n, short s, short race, short profession)
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_ChrCreate_req* req =new Pro_ChrCreate_req();
	req->name_ =n;
	req->sex_ =s;
	req->race_ =race;
	req->profession_ =profession;

	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::selchr( int cid)
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_ChrSel_req* req =new Pro_ChrSel_req();
	req->chrid_ =cid;

	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::delchr( int cid)
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_ChrDel_req* req =new Pro_ChrDel_req();
	req->chrid_ =cid;

	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::teleport( int v1, int v2)
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_AppTeleport_req* req =new Pro_AppTeleport_req();
	req->telid_ =v1;
	req->teltoid_ =v2;

	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::moveto( int x, int y, int z)
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	x_ += x;
	y_ += y;
	z_ += z;

	Pro_PlayerMove_req* req =new Pro_PlayerMove_req();
	req->locationx_ =x_;
	req->locationy_ =y_;
	req->locationz_ =z_;
	req->facing_ =0.f;

	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::enterinst( int v1)
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_AppEnterIns_req* req =new Pro_AppEnterIns_req();
	req->telid_ =v1;

	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::quitinst()
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_AppQuitInst_req* req =new Pro_AppQuitInst_req();

	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::petfollow( int petid)
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_PetSummon_req* req =new Pro_PetSummon_req();
	req->petid_ =petid;

	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::petback()
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_PetSummon_req* req =new Pro_PetSummon_req();
	req->petid_ =NO_INITVALUE;

	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}

void CommandTestImpl::mapchat( const char* msg)
{
	ret_desc_ ="";
	if( !islogon())
	{
		ret_desc_ ="用户未登陆\r\n";
		return;
	}

	Pro_TeamChat_req* req =new Pro_TeamChat_req();
	req->msg_ =msg;
	req->chattype_ =1;

	if( !send_to_gts( req))
	{
		ret_desc_ ="发送协议失败\r\n";
		return;
	}
}