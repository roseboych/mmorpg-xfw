/**
* dpx application
*
* @category		player's data save
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "DPXSvr.h"

#include <prolib/Pro_all.h>
#include <corelib/db/MustReturnContext.h>
#include <corelib/db/NotReturnContext.h>
#include <corelib/log/logmacro.h>

#include "dbaction/ChrUpdateInfoAction.h"

#include "DPXModule.h"

USE_PROTOCOL_NAMESPACE;

void DPXSvr::css_posrotsave_ntf( BasicProtocol* p, bool& autorelease)
{
	DPX_GETPLAYER_FROMCACHE( user, p);

	Pro_DBPosRotSave_ntf* ntf =dynamic_cast<Pro_DBPosRotSave_ntf*>(p);
	user->baseinfo_.posx_ =ntf->lastposx_;
	user->baseinfo_.posy_ =ntf->lastposy_;
	user->baseinfo_.posz_ =ntf->lastposz_;
	user->baseinfo_.facing_ =ntf->lastfacing_;

	ChrUpdateInfoAction* la =DATABASE_NEW ChrUpdateInfoAction( user->uuid_.init_timestamp_);
	la->chrid_ =user->chrid_;
	la->baseinfo_ =user->baseinfo_;

	NotReturnContext<ChrUpdateInfoAction>* rc =TASKCMD_NEW NotReturnContext<ChrUpdateInfoAction>( la);
	DPXMODULE->append_noreturntask_sort( user->uuid_.userid_, rc);
}

void DPXSvr::css_playerbaseinfo_sav( BasicProtocol* p, bool& autorelease)
{
	DPX_GETPLAYER_FROMCACHE( user, p);

	Pro_DBPlayerBaseInfo_sav* sav =dynamic_cast<Pro_DBPlayerBaseInfo_sav*>( p);
	user->baseinfo_.moneys_ =sav->baseinfo_.moneys_;
	user->baseinfo_.level_ =sav->baseinfo_.level_;
	user->baseinfo_.bagslots_ =sav->baseinfo_.bagslots_;
	user->baseinfo_.skill1_ =sav->baseinfo_.skill1_;
	user->baseinfo_.skill2_ =sav->baseinfo_.skill2_;
	user->baseinfo_.hp_ =sav->baseinfo_.hp_;
	user->baseinfo_.mp_ =sav->baseinfo_.mp_;
	user->baseinfo_.power_ =sav->baseinfo_.power_;
	user->baseinfo_.agile_ =sav->baseinfo_.agile_;
	user->baseinfo_.endurance_ =sav->baseinfo_.endurance_;
	user->baseinfo_.intellect_ =sav->baseinfo_.intellect_;
	user->baseinfo_.spirit_ =sav->baseinfo_.spirit_;
	user->baseinfo_.armor_ =sav->baseinfo_.armor_;
	user->baseinfo_.died_ =sav->baseinfo_.died_;
	user->baseinfo_.exp_ =sav->baseinfo_.exp_;

	ChrUpdateInfoAction* la =DATABASE_NEW ChrUpdateInfoAction( user->uuid_.init_timestamp_);
	la->chrid_ =user->chrid_;
	la->baseinfo_ =user->baseinfo_;

	NotReturnContext<ChrUpdateInfoAction>* rc =TASKCMD_NEW NotReturnContext<ChrUpdateInfoAction>( la);
	DPXMODULE->append_noreturntask_sort( user->uuid_.userid_, rc);
}

void DPXSvr::css_buffchange_ntf( BasicProtocol* p, bool& autorelease)
{
	DPX_GETPLAYER_FROMCACHE( user, p);

	Pro_BuffChange_ntf* ntf =dynamic_cast<Pro_BuffChange_ntf*>( p);
}
