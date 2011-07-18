/**
* css
*
* @category		enter inst
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __ENTERINSTCONDHELPER__H__
#define __ENTERINSTCONDHELPER__H__

#include <corelib/corelibdef.h>
#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include "EnterInstConditionBase.h"

#include <vector>

class BaseStoryService;
class StoryMapOption;
class Player;

/**
* @class EnterInstCondHelper
* 
* @brief 进入副本条件判定帮助类
* 
**/
class EnterInstCondHelper
{
	typedef NS_STL::vector<EnterInstConditionBase*> ENTERINSTCOND_VECTOR;
public:
	EnterInstCondHelper();
	~EnterInstCondHelper();

	/**
	* 是否能进入副本条件判定
	* @param psvr
	* @param instopt		副本地图配置
	* @param curplayer
	* @return
	**/
	S_BOOL can_joininstance( BaseStoryService* psvr, StoryMapOption* instopt, Player* curplayer);

protected:
	EnterInstConditionBase* get_enterinstimpl( NS_STL::string clsname);

private:
	//支持的所有条件
	ENTERINSTCOND_VECTOR	conds_;
};

#define ENTERINSTHelper ACE_Singleton< EnterInstCondHelper, ACE_Null_Mutex>::instance()

#endif	//__ENTERINSTCONDHELPER__H__
