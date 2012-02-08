/**
* css application
*
* @category		player state
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __PLAYERBEHAVIORLOADER__H__
#define __PLAYERBEHAVIORLOADER__H__

#include <prolib/core_type.h>
#include <string>

#include "../BehaviorTreeNode.h"
#include "../BaseActionState.h"

/**
* @class PlayerBehaviorLoader
* 
* @brief behavior配置加载器
**/
class PlayerBehaviorLoader
{
private:
	PlayerBehaviorLoader(){}

public:
	/**
	* 加载behavior的配置信息
	* @return
	**/
	static bool load_behaviorconfig();

private:
	/**
	* 根据名字或者对应的类型
	**/
	static char get_behavior_type( const NS_STL::string& name);
};

#endif	//__PLAYERBEHAVIORLOADER__H__
