/**
* dpx application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "ChrUpdateInfoAction.h"

#include <corelib/corelibdef.h>
#include <corelib/log/logmacro.h>
#include <reslib/deploy/GlobalConfig.h>

ChrUpdateInfoAction::ChrUpdateInfoAction( S_TIMESTAMP s):
inherit( DBACTION_CHRUPDATEINFO, s),
ret_( 0)
{
}

ChrUpdateInfoAction::~ChrUpdateInfoAction()
{
}

void ChrUpdateInfoAction::run_indbpool( otl_connect *pconn)
{
	try
	{
		pconn->auto_commit_off();
#ifdef OTL_ODBC
		pconn->set_transaction_isolation_level( otl_tran_serializable);
#endif
	
		otl_stream cur( 50, DBPROC_CHRUPDATE, *pconn, otl_implicit_select);

		S_INT_16 dv =baseinfo_.died_;
		S_INT_16 bs =baseinfo_.bagslots_;
		cur << chrid_ << baseinfo_.moneys_ << baseinfo_.level_ << bs << baseinfo_.skill_[0] << baseinfo_.skill_[1]
			<< baseinfo_.hp_ << baseinfo_.mp_ << baseinfo_.posx_ << baseinfo_.posy_ << baseinfo_.posz_ << baseinfo_.facing_
			<< dv << baseinfo_.exp_;

		cur.close();

		pconn->commit();
	}
	catch( otl_exception& e){
		pconn->rollback();

		MODULE_LOG_ERROR( MODULE_DATABASE, "ChrUpdateInfoAction::run_indbpool exception code:%d msg:%s ", e.code, e.msg);
	}
}

void ChrUpdateInfoAction::run_inmainpool()
{
}
