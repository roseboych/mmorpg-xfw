/**
* css application
*
* @category		player state
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __BEHAVIORTREENODE__H__
#define __BEHAVIORTREENODE__H__

#include "../css_pre.h"

#include <list>
class BaseActionState;
class PlayerBehaviorLoader;

/**
* @class BehaviorTreeNode
* 
* @brief player的behaviortree的配置节点
**/
class BehaviorTreeNode
{
	friend class BaseActionState;
	friend class PlayerBehaviorLoader;
public:
	typedef NS_STL::list<char>	GOTO_STATE_LIST;

private:
	BehaviorTreeNode();

public:
	void regist_nextstate( char s);

	bool is_ingotostate( char s);

	char get_referfightstate(){ return refer_fightstate_;}

protected:
	//支持的下级状态列表
	GOTO_STATE_LIST	goto_states_;

	//非战斗状态对应的战斗状态
	char			refer_fightstate_;
};

#endif	//__BEHAVIORTREENODE__H__
