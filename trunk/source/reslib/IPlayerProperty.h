/**
* reslib application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __IPLAYERPROPERTY__H__
#define __IPLAYERPROPERTY__H__

/**
* @class IPlayerProperty
* 
* @brief interface of the player property
**/
class IPlayerProperty
{
public:
	//生命值hp
	virtual int get_hp() =0;
	virtual void set_hp( int v) =0;
	virtual int inc_hp( int v) =0;
	//法力值mp
	virtual int get_mp() =0;
	virtual void set_mp( int v) =0;
	virtual int inc_mp( int v) =0;
	//力量force
	virtual int get_force() =0;
	virtual void set_force( int v) =0;
	virtual int inc_force( int v) =0;
	//敏捷agile
	virtual int get_agile() =0;
	virtual void set_agile( int v) =0;
	virtual int inc_agile( int v) =0;
	//耐力endurance
	virtual int get_endurance() =0;
	virtual void set_endurance( int v) =0;
	virtual int inc_endurance( int v) =0;
	//智力intellect
	virtual int get_intellect() =0;
	virtual void set_intellect( int v) =0;
	virtual int inc_intellect( int v) =0;
	//精神spirit
	virtual int get_spirit() =0;
	virtual void set_spirit( int v) =0;
	virtual int inc_spirit( int v) =0;
	//护甲armor
	virtual int get_armor() =0;
	virtual void set_armor( int v) =0;
	virtual int inc_armor( int v) =0;
};

#endif	//__IPLAYERPROPERTY__H__
