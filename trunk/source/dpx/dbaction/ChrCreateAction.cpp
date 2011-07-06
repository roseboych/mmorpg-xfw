/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "ChrCreateAction.h"

#include <prolib/Pro_all.h>
#include <corelib/corelibdef.h>
#include <corelib/log/logmacro.h>
#include <reslib/deploy/GlobalConfig.h>

#include "../player/Player.h"
#include "../DPXModule.h"

USE_PROTOCOL_NAMESPACE;

ChrCreateAction::ChrCreateAction( S_TIMESTAMP s):inherit( DBACTION_CHRADDINFO, s),
player_( 0), param_(0), ret_( 0), chrinfo_( 0)
{
}

ChrCreateAction::~ChrCreateAction()
{
	if( chrinfo_)
		delete chrinfo_;
	chrinfo_ =0;
}

bool ChrCreateAction::validate()
{
	if( player_ == 0 || !player_->is_samesession( uuid_))
		return false;

	return true;
}

void ChrCreateAction::run_indbpool( otl_connect *pconn)
{
	try
	{
		pconn->auto_commit_off();
#ifdef OTL_ODBC
		pconn->set_transaction_isolation_level( otl_tran_serializable);
#endif
	
		otl_stream cur( 50, DBPROC_CHRADDINFO, *pconn, otl_implicit_select);

		//TODO:增加初试坐标
		S_INT_16 sv =param_->sex_;
		S_INT_16 rc =param_->race_;
		S_INT_16 pf =param_->profession_;
		S_INT_16 bgs =50;
		cur << param_->uuid_.userid_ << (short)GLOBALCONFIG_INS->get_regionsn()
			<< param_->name_.c_str() << rc << pf << sv << 1000 << 1000 << 0 << 1000000
			<< param_->posx_ << param_->posy_ << param_->posz_ << param_->facing_
			<< 0 << 1 << 2 << 3 << 4 << 5 << bgs << 0x01000003 << 0;

		if( !cur.eof())
		{
			cur >> ret_;
			if( ret_ == -1)
				ret_ =4;
			else if( ret_ == -2)
				ret_ =2;
			else if( ret_ == -3)
				ret_ =3;
			else
			{
				chrinfo_ =FRAMEWK_NEW ChrListInfo();
				cur >> chrinfo_->chrid_ >> chrinfo_->moneys_ >> chrinfo_->level_ 
					>> chrinfo_->petid_;

				chrinfo_->name_ =param_->name_;
				chrinfo_->sex_ =param_->sex_;
				chrinfo_->race_ =param_->race_;
				chrinfo_->profession_ =param_->profession_;
				chrinfo_->lastposx_ =param_->posx_;
				chrinfo_->lastposy_ =param_->posy_;
				chrinfo_->lastposz_ =param_->posz_;
				chrinfo_->lastfacing_ =param_->facing_;
				chrinfo_->hp_ =1000;
				chrinfo_->mp_ =1000;
				chrinfo_->exp_ =0;
			}
		}
		else
			ret_ =3;

		cur.close();

		if( ret_ == 0)
			pconn->commit();
		else
			pconn->rollback();
	}
	catch( otl_exception& e){
		pconn->rollback();
		ret_ =3;
		MODULE_LOG_ERROR( MODULE_DATABASE, "ChrCreateAction::run_indbpool exception code:%d msg:%s ", e.code, e.msg);
	}
}

void ChrCreateAction::run_inmainpool()
{
	Pro_ChrCreate_ack* ack =PROTOCOL_NEW Pro_ChrCreate_ack();
	PRO_UUID_FILL( ack, player_->global_index_, player_->uuid_);
	ack->result_ =ret_;
	ack->equiplen_ =CHRITEMS_MAX;

	if( ret_ == 0)
	{
		ack->chrid_ =chrinfo_->chrid_;
		ack->name_ =chrinfo_->name_;
		ack->race_ =chrinfo_->race_;
		ack->profession_ =chrinfo_->profession_;
		ack->sex_ =chrinfo_->sex_;
		ack->hp_ =chrinfo_->hp_;
		ack->mp_ =chrinfo_->mp_;
		ack->exp_ =chrinfo_->exp_;
		ack->level_ =chrinfo_->level_;
		ack->moneys_ =chrinfo_->moneys_;
		ack->petid_ =chrinfo_->petid_;
		ack->petcode_ =chrinfo_->petcode_;
		ack->petname_ =chrinfo_->petname_;
		ack->equiplen_ =CHRITEMS_MAX;
		memset( &ack->equips_[0], 0, sizeof( S_INT_32)*CHRITEMS_MAX);

		player_->chrs_.push_back( chrinfo_);
		chrinfo_ =0;
	}

	DPXMODULE->send_to_cts( ack);
}
