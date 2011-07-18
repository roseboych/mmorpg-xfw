/**
* css
*
* @category		enter inst
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __ENTERINSTCONDITIONBASE__H__
#define __ENTERINSTCONDITIONBASE__H__

#include <corelib/corelibdef.h>
#include <string>

class EnterInstParamBase;
class Player;

#define ENTERINSTCHECKIMPL_DEFINE( classname)						\
public:																\
	virtual const NS_STL::string& get_classname(){					\
		static NS_STL::string s_##classname_name =#classname;		\
		return s_##classname_name;									\
	}

/**
* @class EnterInstConditionBase
* 
* @brief ���븱�������ж�
* 
**/
class EnterInstConditionBase
{
public:
	virtual const NS_STL::string& get_classname() =0;

	/*
	* ��main map���ж��Ƿ��ܹ�����ָ������
	* @param param
	* @param curplayer
	* @return
	*/
	virtual S_BOOL join_instance( EnterInstParamBase* param, Player* curplayer) =0;
};

#endif	//__ENTERINSTCONDITIONBASE__H__
