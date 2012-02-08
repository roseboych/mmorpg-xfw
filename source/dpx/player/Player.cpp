/**
* dpx application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "Player.h"

Player::Player()
{
	reset();
}

void Player::regist( uuid_session& id)
{
	reset();

	uuid_ =id;
}

void Player::reset()
{
	uuid_.reset();

	clear_chrlist();
	clear_itemlist();
	clear_petlist();
	clear_bufferlist();

	chrid_ =NO_INITVALUE;
	is_chrload_ =false;
	css_svr_ =0;
}

void Player::send_to_css( BasicProtocol* p)
{
	if( css_svr_ == 0)
		delete p;
	else
		css_svr_->send_protocol( p);
}

void Player::set_curchrid( int cid)
{
	chrid_ =cid;
	is_chrload_ =false;
}
