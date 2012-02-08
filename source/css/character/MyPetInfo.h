/**
* css application
*
* @category		player pet
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __MYPETINFO__H__
#define __MYPETINFO__H__

#include "../css_pre.h"
#include <prolib/Pro_define.h>

/**
* @class MyPetInfo
* 
* @brief 玩家宠物信息
**/
class MyPetInfo
{
public:
	MyPetInfo();
	MyPetInfo( const PRO::petdata_item& pdata);

	inline int get_petid();
	inline int	get_petcode();
	inline const char* get_petname();
	inline void set_petname( const char* v);

	inline PRO::petdata_item& get_petdata();
protected:
	//宠物信息
	PRO::petdata_item	petdata_;
	//宠物配置
};

#include "MyPetInfo.inl"

#endif	//__MYPETINFO__H__
