/**
* css application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "Player.h"

#include <prolib/Pro_all.h>
#include "../BaseStoryService.h"

USE_PROTOCOL_NAMESPACE;

void Player::lnkkeep_reload()
{
	//发送玩家数据
	Pro_ChrLoad_ack *ack =PROTOCOL_NEW Pro_ChrLoad_ack();
	PRO_UUID_FILL( ack, global_index_, uuid_);
	for( PlayerData::MYITEMINFO_MAP::iterator iter =myitems_.begin(); iter != myitems_.end(); ++iter)
	{
		MyItemInfo *pinfo =iter->second;
		ack->items_.push_back( pinfo->get_itemdata());
	}

	for( PlayerData::MYPETINFO_VECTOR::iterator iter =mypets_.begin(); iter != mypets_.end(); ++iter)
	{
		MyPetInfo* ppet =(*iter);
		ack->pets_.push_back( ppet->get_petdata());
	}

	send_to_gts( ack);

	BaseStoryService* pcontext =CONTENTSERVICE_INS;

	Pro_ChrFin_NTF *ntf =PROTOCOL_NEW Pro_ChrFin_NTF();
	PRO_UUID_FILL( ntf, global_index_, uuid_);
	ntf->chrid_ =chrid_;
	ntf->baseinfo_ =baseinfo_;

	send_to_gts( ntf);

	//发送进入玩家视野数据
	pcontext->get_scenemanage()->notify_allobjs_inview( this);
}