/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "ChrDelAction.h"

#include <corelib/corelibdef.h>
#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "../player/Player.h"
#include "../DPXModule.h"

USE_PROTOCOL_NAMESPACE;

ChrDelAction::ChrDelAction( S_TIMESTAMP s):inherit( DBACTION_CHRDEL, s),
player_( 0), ret_( 0)
{

}

bool ChrDelAction::validate()
{
	if( player_ == 0 || !player_->is_samesession( uuid_))
		return false;

	return true;
}

void ChrDelAction::run_indbpool( otl_connect *pconn)
{
	try
	{
		pconn->auto_commit_off();
#ifdef OTL_ODBC
		pconn->set_transaction_isolation_level( otl_tran_serializable);
#endif
	
		otl_stream cur( 50, DBPROC_CHRDEL, *pconn, otl_implicit_select);

		cur << chrid_;

		if( !cur.eof())
		{
			cur >> ret_;
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
		MODULE_LOG_ERROR( MODULE_DATABASE, "ChrDelAction::run_indbpool exception code:%d msg:%s ", e.code, e.msg);
	}
}

void ChrDelAction::run_inmainpool()
{
	if( ret_ == -1)
		ret_ =3;

	if( ret_ == 0)
	{
		player_->rm_chrinlist( chrid_);
		if( player_->chrid_ == chrid_)
			player_->set_curchrid( NO_INITVALUE);
	}

	Pro_ChrDel_ack* ack =PROTOCOL_NEW Pro_ChrDel_ack();
	PRO_UUID_FILL( ack, player_->global_index_, player_->uuid_);
	ack->result_ =ret_;
	ack->chrid_ =chrid_;

	DPXMODULE->send_to_cts( ack);
}
