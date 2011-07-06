/**
* css application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng, 2010
*/
inline 
void PlayerData::set_facing( S_FLOAT_32& face)
{
	baseinfo_.facing_ =face;
}

inline 
S_FLOAT_32 PlayerData::get_facing()
{
	return baseinfo_.facing_;
}

inline 
S_INT_8 PlayerData::get_sex()
{
	return baseinfo_.sex_;
}

inline
S_INT_8 PlayerData::get_race()
{
	return baseinfo_.race_;
}

inline
S_INT_8 PlayerData::get_profession()
{
	return baseinfo_.profession_;
}

inline
S_INT_32 PlayerData::get_money()
{
	return baseinfo_.moneys_;
}

inline
void PlayerData::set_money( S_INT_32 v)
{
	baseinfo_.moneys_ =v;
}

inline
S_INT_32 PlayerData::get_level()
{
	return baseinfo_.level_;
}

inline
S_INT_8 PlayerData::get_bagslots()
{
	return baseinfo_.bagslots_;
}

inline
S_INT_8 PlayerData::get_died()
{
	return baseinfo_.died_;
}

inline
void PlayerData::set_died( S_INT_8 v)
{
	this->baseinfo_.died_ =v;
}

inline 
S_INT_32 PlayerData::get_hp()
{
	return baseinfo_.hp_;
}

inline 
void PlayerData::set_hp( S_INT_32 v)
{
	baseinfo_.hp_ =v;
}

inline 
S_INT_32 PlayerData::inc_hp( S_INT_32 v)
{
	baseinfo_.hp_ += v;
	return baseinfo_.hp_;
}

inline 
S_INT_32 PlayerData::get_mp()
{
	return baseinfo_.mp_;
}

inline 
void PlayerData::set_mp( S_INT_32 v)
{
	baseinfo_.mp_ =v;
}

inline 
S_INT_32 PlayerData::inc_mp( S_INT_32 v)
{
	baseinfo_.mp_ += v;
	return baseinfo_.mp_;
}

inline 
S_INT_32 PlayerData::get_force()
{
	return prop_force_;
}

inline 
void PlayerData::set_force( S_INT_32 v)
{
	prop_force_ =v;
}

inline 
S_INT_32 PlayerData::inc_force( S_INT_32 v)
{
	prop_force_ += v;
	return prop_force_;
}

inline 
S_INT_32 PlayerData::get_agile()
{
	return prop_agile_;
}

inline 
void PlayerData::set_agile( S_INT_32 v)
{
	prop_agile_ =v;
}

inline 
S_INT_32 PlayerData::inc_agile( S_INT_32 v)
{
	prop_agile_ += v;
	return prop_agile_;
}

inline 
S_INT_32 PlayerData::get_endurance()
{
	return prop_endurance_;
}

inline 
void PlayerData::set_endurance( S_INT_32 v)
{
	prop_endurance_ =v;
}

inline 
S_INT_32 PlayerData::inc_endurance( S_INT_32 v)
{
	prop_endurance_ += v;
	return prop_endurance_;
}

inline 
S_INT_32 PlayerData::get_intellect()
{
	return prop_intellect_;
}

inline 
void PlayerData::set_intellect( S_INT_32 v)
{
	prop_intellect_ =v;
}

inline 
S_INT_32 PlayerData::inc_intellect( S_INT_32 v)
{
	prop_intellect_ += v;
	return prop_intellect_;
}

inline 
S_INT_32 PlayerData::get_spirit()
{
	return prop_spirit_;
}

inline 
void PlayerData::set_spirit( S_INT_32 v)
{
	prop_spirit_ =v;
}

inline 
S_INT_32 PlayerData::inc_spirit( S_INT_32 v)
{
	prop_spirit_ += v;
	return prop_spirit_;
}

inline 
S_INT_32 PlayerData::get_armor()
{
	return prop_armor_;
}

inline 
void PlayerData::set_armor( S_INT_32 v)
{
	prop_armor_ =v;
}

inline 
S_INT_32 PlayerData::inc_armor( S_INT_32 v)
{
	prop_armor_ += v;
	return prop_armor_;
}
