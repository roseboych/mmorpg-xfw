/**
* css application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng, 2010
*/
inline 
void PlayerData::set_facing( float& face)
{
	baseinfo_.facing_ =face;
}

inline 
float PlayerData::get_facing()
{
	return baseinfo_.facing_;
}

inline 
char PlayerData::get_sex()
{
	return baseinfo_.sex_;
}

inline
char PlayerData::get_race()
{
	return baseinfo_.race_;
}

inline
char PlayerData::get_profession()
{
	return baseinfo_.profession_;
}

inline
int PlayerData::get_money()
{
	return baseinfo_.moneys_;
}

inline
void PlayerData::set_money( int v)
{
	baseinfo_.moneys_ =v;
}

inline
int PlayerData::get_level()
{
	return baseinfo_.level_;
}

inline
char PlayerData::get_bagslots()
{
	return baseinfo_.bagslots_;
}

inline
char PlayerData::get_died()
{
	return baseinfo_.died_;
}

inline
void PlayerData::set_died( char v)
{
	this->baseinfo_.died_ =v;
}

inline 
int PlayerData::get_hp()
{
	return baseinfo_.hp_;
}

inline 
void PlayerData::set_hp( int v)
{
	baseinfo_.hp_ =v;
}

inline 
int PlayerData::inc_hp( int v)
{
	baseinfo_.hp_ += v;
	return baseinfo_.hp_;
}

inline 
int PlayerData::get_mp()
{
	return baseinfo_.mp_;
}

inline 
void PlayerData::set_mp( int v)
{
	baseinfo_.mp_ =v;
}

inline 
int PlayerData::inc_mp( int v)
{
	baseinfo_.mp_ += v;
	return baseinfo_.mp_;
}

inline 
int PlayerData::get_exp()
{
	return baseinfo_.exp_;
}

inline
void PlayerData::set_exp( int v)
{
	baseinfo_.exp_ =v;
}

inline
int PlayerData::inc_exp( int v)
{
	baseinfo_.exp_ += v;
	return baseinfo_.exp_;
}

inline 
int PlayerData::get_force()
{
	return prop_force_;
}

inline 
void PlayerData::set_force( int v)
{
	prop_force_ =v;
}

inline 
int PlayerData::inc_force( int v)
{
	prop_force_ += v;
	return prop_force_;
}

inline 
int PlayerData::get_agile()
{
	return prop_agile_;
}

inline 
void PlayerData::set_agile( int v)
{
	prop_agile_ =v;
}

inline 
int PlayerData::inc_agile( int v)
{
	prop_agile_ += v;
	return prop_agile_;
}

inline 
int PlayerData::get_endurance()
{
	return prop_endurance_;
}

inline 
void PlayerData::set_endurance( int v)
{
	prop_endurance_ =v;
}

inline 
int PlayerData::inc_endurance( int v)
{
	prop_endurance_ += v;
	return prop_endurance_;
}

inline 
int PlayerData::get_intellect()
{
	return prop_intellect_;
}

inline 
void PlayerData::set_intellect( int v)
{
	prop_intellect_ =v;
}

inline 
int PlayerData::inc_intellect( int v)
{
	prop_intellect_ += v;
	return prop_intellect_;
}

inline 
int PlayerData::get_spirit()
{
	return prop_spirit_;
}

inline 
void PlayerData::set_spirit( int v)
{
	prop_spirit_ =v;
}

inline 
int PlayerData::inc_spirit( int v)
{
	prop_spirit_ += v;
	return prop_spirit_;
}

inline 
int PlayerData::get_armor()
{
	return prop_armor_;
}

inline 
void PlayerData::set_armor( int v)
{
	prop_armor_ =v;
}

inline 
int PlayerData::inc_armor( int v)
{
	prop_armor_ += v;
	return prop_armor_;
}
