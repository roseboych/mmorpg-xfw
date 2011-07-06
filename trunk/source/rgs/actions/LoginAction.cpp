/**
* rgs application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "LoginAction.h"

#include <corelib/corelibdef.h>
#include <corelib/util/ShareUtil.h>
#include <reslib/deploy/GlobalConfig.h>
#include <reslib/deploy/SvrTeamOption.h>
#include <reslib/deploy/GTSOption.h>
#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "../PlayerInfo.h"
#include "../RGSModule.h"

LoginAction::LoginAction( S_TIMESTAMP s):inherit( DBACTION_LOGIN, s),
player_( 0), ret_( 0)
{

}

bool LoginAction::validate()
{
	if( player_ == 0 || !player_->is_samesession( uuid_))
		return false;

	return true;
}

void LoginAction::run_indbpool( otl_connect *pconn)
{
	NS_STL::vector< NS_STL::string> ss;
	ShareUtil::splitstr2str( token_.c_str(), ":", ss);
	if( ss.size() != 2)
	{
		ret_ =6;
		return;
	}

	try
	{
		pconn->auto_commit_off();
#ifdef OTL_ODBC
		pconn->set_transaction_isolation_level( otl_tran_serializable);
#endif
	
		otl_stream cur( 50, DBPROC_USERLOGIN, *pconn, otl_implicit_select);

		cur << ss[0];
		
		NS_STL::string pwd;
		if( !cur.eof())
		{
			cur >> uid_ >> pwd >> validatetime_ >> lastregion_ >> lastlogout_ >> types_ >> isdel_;
		}
		else
			ret_ =7;

		cur.close();

		pconn->commit();

		if( ret_ == 0)
		{
			if( isdel_ != 0)
				ret_ =7;
			else if( pwd != ss[1])
				ret_ =1;
		}
	}
	catch( otl_exception& e){
		MODULE_LOG_ERROR( MODULE_DATABASE, "LoginAction::run_indbpool exception code:%d msg:%s ", e.code, e.msg);
		pconn->rollback();
		ret_ =6;
	}
}

void LoginAction::run_inmainpool()
{
	//检查玩家是否存在
	PlayerInfo* pexist =0;
	if( ret_ == 0)
	{
		//断线踢人
		pexist =RGSMODULE->find_byuserid( uid_);
	}

	PRO::Pro_Login_ack* ack =PROTOCOL_NEW PRO::Pro_Login_ack();
	PROTOCOL_UUID_FILL( ack, player_->server_index_, player_->uuid_)

	ack->result_ =ret_;
	ack->userid_ =uid_;
	ack->validatetime_ =this->validatetime_;
	ack->lastregion_ =this->lastregion_;
	ack->lastlogout_ =this->lastlogout_;
	//设置断线保持需要的数据
	if( ret_ == 0 && pexist && pexist->is_lnkkeep())
	{
		ack->is_lnkkeep_ =1;
		ack->token_ =pexist->qt_token_;
		ack->session_ =pexist->uuid_.init_timestamp_;
		//获取ip,port
		SvrTeamOption* svr =GLOBALCONFIG_INS->get_svrteamoption( pexist->lnkkeep_teamid_);
		GTSOption* gts =svr->get_gtsoption( pexist->lnkkeep_gtsid_);
		ack->gts_ip_ =gts->ip_;
		ack->gts_port_ =gts->port_;
		ack->proxy_index_ =pexist->lnkkeep_proxyindex_;
	}

	player_->send_to_lgs( ack);

	//处理结果
	if( ret_ == 0)
	{
		//断线保持处理
		if( pexist)
		{
			if( pexist->is_lnkkeep())
			{
				pexist->login_succ_of_lnkkeep( player_->server_index_, player_->lgs_link_);
				//断线保持
				RGSMODULE->free_emptyplayer( player_);
			}
			else
			{
				//如果已经登陆先踢人
				RGSMODULE->fore_playerlogout( pexist);
				player_->uuid_.set_userid( uid_);
				RGSMODULE->regist_player( player_);
			}
		}
		else
		{
			//注册
			player_->uuid_.set_userid( uid_);
			RGSMODULE->regist_player( player_);
		}
	}
	else
	{
		//释放占用的player资源
		RGSMODULE->free_emptyplayer( player_);
	}
}
