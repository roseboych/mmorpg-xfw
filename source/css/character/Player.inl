/**
* css application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng, 2009
*/

inline 
void Player::set_position( S_FLOAT_32 x, S_FLOAT_32 y, S_FLOAT_32 z)
{
	this->baseinfo_.posx_ =x;
	this->baseinfo_.posy_ =y;
	this->baseinfo_.posz_ =z;
}

inline 
void Player::set_position( coordinate_pointer& p)
{
	this->baseinfo_.posx_ =p.x_;
	this->baseinfo_.posy_ =p.y_;
	this->baseinfo_.posz_ =p.z_;
}

inline 
coordinate_pointer Player::get_position()
{
	coordinate_pointer pos;
	pos.x_ =baseinfo_.posx_;
	pos.y_ =baseinfo_.posy_;
	pos.z_ =baseinfo_.posz_;
	
	return pos;
}

inline 
S_FLOAT_32 Player::get_positionx()
{
	return baseinfo_.posx_;
}

inline S_FLOAT_32 Player::get_positiony()
{
	return baseinfo_.posy_;
}

inline S_FLOAT_32 Player::get_positionz()
{
	return baseinfo_.posz_;
}

inline 
void Player::set_positionx( S_FLOAT_32 v)
{
	baseinfo_.posx_ =v;
}

inline 
void Player::set_positiony( S_FLOAT_32 v)
{
	baseinfo_.posy_ =v;
}

inline 
void Player::set_positionz( S_FLOAT_32 v)
{
	baseinfo_.posz_ =v;
}

inline 
void Player::set_facing( S_FLOAT_32 v)
{
	baseinfo_.facing_ =v;
}

inline 
S_FLOAT_32 Player::get_facing()
{
	return baseinfo_.facing_;
}

inline 
bool Player::is_needupdatepos( const ACE_UINT64& tnow)
{
	//副本不需要保存玩家位置
	if( is_in_inst)
		return false;

	if( t_lastupdate_ + PLAYER_UPDATEPOS_TIMESTEP*1000 > tnow)
		return false;

	if( ::abs( lastposx_ - baseinfo_.posx_) < 10.f && ::abs( lastposy_ - baseinfo_.posy_) < 10.f &&
		::abs( lastfacing_ - baseinfo_.facing_) < 5.f)
		return false;

	return true;
}
