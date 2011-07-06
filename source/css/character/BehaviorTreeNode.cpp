/**
* css application
*
* @category		player state
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "BehaviorTreeNode.h"

#include "BaseActionState.h"
#include "states/PlayerBehaviorLoader.h"

#include <algorithm>

BehaviorTreeNode::BehaviorTreeNode():
refer_fightstate_( CHRSTATE_NONE)
{
}

void BehaviorTreeNode::regist_nextstate( S_INT_8 s)
{
	goto_states_.push_back( s);
	goto_states_.unique();
}

bool BehaviorTreeNode::is_ingotostate( S_INT_8 s)
{
	return NS_STL::find( goto_states_.begin(), goto_states_.end(), s) != goto_states_.end();
}
