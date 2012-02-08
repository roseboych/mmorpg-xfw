/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "ChrGetListAction.h"

#include <prolib/Pro_all.h>
#include <corelib/corelibdef.h>
#include <corelib/log/logmacro.h>
#include <reslib/deploy/GlobalConfig.h>

#include "../player/Player.h"
#include "../DPXModule.h"

ChrGetListAction::ChrGetListAction( S_TIMESTAMP s):inherit( DBACTION_CHRGETLIST, s),
player_( 0)
{
}

ChrGetListAction::~ChrGetListAction()
{
	release();
}

void ChrGetListAction::release()
{
	for( size_t ii =0; ii < chrs_.size(); ++ii)
		delete chrs_[ii];
	chrs_.clear();
}

ChrListInfo* ChrGetListAction::get_chrinfo( int cid)
{
	for( size_t ii =0; ii < chrs_.size(); ++ii)
	{
		if( chrs_[ii]->chrid_ == cid)
			return chrs_[ii];
	}

	return 0;
}

bool ChrGetListAction::validate()
{
	if( player_ == 0 || !player_->is_samesession( uuid_))
		return false;

	return true;
}

void ChrGetListAction::run_indbpool( otl_connect *pconn)
{
	try
	{
		pconn->auto_commit_off();
#ifdef OTL_ODBC
		pconn->set_transaction_isolation_level( otl_tran_serializable);
#endif
	
		otl_stream cur( 50, DBPROC_CHRGETLIST, *pconn, otl_implicit_select);

		cur << uuid_.userid_ << (short)GLOBALCONFIG_INS->get_regionsn();
		
		//获取角色列表
		while( !cur.eof())
		{
			short sv, rc, pf;
			NS_STL::string strname, petname;
			ChrListInfo* chr =FRAMEWK_NEW ChrListInfo();
			cur >> chr->chrid_ >> strname >> rc >> pf >> sv >> chr->lastposx_ >> chr->lastposy_ >> chr->lastposz_
				>> chr->lastfacing_ >> chr->hp_ >> chr->mp_ >> chr->exp_ >> chr->level_ >> chr->moneys_
				>> chr->petid_ >> chr->petcode_ >> petname;

			chr->name_ =strname;
			chr->petname_ =petname;
			chr->race_ =rc;
			chr->sex_ =sv;
			chr->profession_ =pf;
			chr->equiplen_ =CHRITEMS_MAX;

			chrs_.push_back( chr);
		}

		cur.close();

		//获取avatar列表
		otl_stream cur2( 50, DBPROC_CHRGETAVLIST, *pconn, otl_implicit_select);

		cur2 << uuid_.userid_ << (short)GLOBALCONFIG_INS->get_regionsn();

		int cid, icode;
		short iav, inp;

		while( !cur2.eof())
		{
			cur2 >> cid >> iav >> icode >> inp;

			ChrListInfo* pc =get_chrinfo( cid);
			if( pc == 0)
				continue;
			if( inp < 0 || inp >= CHRITEMS_MAX)
				continue;
			pc->equips_[inp] =icode;
		}

		pconn->commit();
	}
	catch( otl_exception& e){
		pconn->rollback();
		MODULE_LOG_ERROR( MODULE_DATABASE, "ChrGetListAction::run_indbpool exception code:%d msg:%s ", e.code, e.msg);
	}
}

void ChrGetListAction::run_inmainpool()
{
	//保存到玩家缓存数据
	NS_STL::copy( chrs_.begin(), chrs_.end(), NS_STL::back_inserter( player_->chrs_));
	chrs_.clear();
	player_->chrs_init_ =true;

	//发送角色列表
	DPXMODULE->send_to_cts( player_->construct_chrlistpro());
}
