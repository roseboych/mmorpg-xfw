/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "ItemDelAction.h"

#include <corelib/corelibdef.h>
#include <prolib/Pro_all.h>
#include <corelib/log/logmacro.h>

#include "../player/Player.h"
#include "../DPXModule.h"

ItemDelAction::ItemDelAction( S_TIMESTAMP s):inherit( DBACTION_ITEMDEL, s),
player_( 0), ret_( 0)
{

}

bool ItemDelAction::validate()
{
	if( player_ == 0 || !player_->is_samesession( uuid_))
		return false;

	return true;
}

void ItemDelAction::run_indbpool( otl_connect *pconn)
{
	try
	{
		pconn->auto_commit_off();
#ifdef OTL_ODBC
		pconn->set_transaction_isolation_level( otl_tran_serializable);
#endif
	
		otl_stream cur( 50, DBPROC_CHRGETLIST, *pconn, otl_implicit_select);

		cur << userid_;
		
		if( !cur.eof())
		{
		}
		else
			ret_ =7;

		cur.close();

		pconn->commit();

		if( ret_ == 0)
		{
		}
	}
	catch( otl_exception& e){
		pconn->rollback();
		ret_ =6;
		MODULE_LOG_ERROR( MODULE_DATABASE, "ChrCreateAction::run_indbpool exception code:%d msg:%s ", e.code, e.msg);
	}
}

void ItemDelAction::run_inmainpool()
{
	
}
