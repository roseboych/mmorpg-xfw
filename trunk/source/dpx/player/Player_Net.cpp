/**
* dpx application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "Player.h"

#include <prolib/Pro_all.h>

USE_PROTOCOL_NAMESPACE;

BasicProtocol* Player::construct_chrlistpro()
{
	Pro_ChrList_ack* ack =PROTOCOL_NEW Pro_ChrList_ack();
	PRO_UUID_FILL( ack, global_index_, uuid_)

	for( size_t ii =0; ii < chrs_.size(); ++ii)
	{
		ChrListInfo* pc =chrs_[ii];

		Pro_ChrList_ack::chrinfo cr;
		cr.chrid_ =pc->chrid_;
		cr.name_ =pc->name_;
		cr.race_ =pc->race_;
		cr.profession_ =pc->profession_;
		cr.sex_ =pc->sex_;
		cr.hp_ =pc->hp_;
		cr.mp_ =pc->mp_;
		cr.exp_ =pc->exp_;
		cr.level_ =pc->level_;
		cr.moneys_ =pc->moneys_;
		cr.petid_ =pc->petid_;
		cr.petcode_ =pc->petcode_;
		cr.petname_ =pc->petname_;
		cr.equiplen_ =pc->equiplen_;
		for( int ind =0; ind < CHRITEMS_MAX; ++ind)
			cr.equips_[ind] =pc->equips_[ind];

		ack->chrs_.push_back( cr);
	}

	return ack;
}
