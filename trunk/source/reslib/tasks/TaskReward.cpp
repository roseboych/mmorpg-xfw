/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "reslib/tasks/TaskReward.h"

#include <corelib/xml/XmlUtil.h>
#include <corelib/log/logmacro.h>
#include "reslib/items/ItemsConfig.h"
#include "reslib/items/ItemInfoBase.h"

#include <ace/Assert.h>

//-------------------------------------------------TaskReward--------------------------------------------------
TaskReward::TaskReward():
item_nums_( 0),
item_( 0)
{
}

int TaskReward::get_itemid()
{
	return item_->get_itemid();
}

TaskReward::operator ItemInfoBase*() const
{
	return item_;
}

//----------------------------------------------TaskRewardGroup-----------------------------------------------
TaskRewardGroup::TaskRewardGroup():
group_id_( 0),
exp_( 0)
{

}

TaskRewardGroup::~TaskRewardGroup()
{
	release();
}

void TaskRewardGroup::release()
{
	for( size_t ii =0; ii < rewards_.size(); ++ii)
		delete rewards_[ii];
	rewards_.clear();
}

bool TaskRewardGroup::load_reward( TiXmlElement* e1)
{
	exp_ =XmlUtil::GetXmlAttrInt( e1, "exp", 0);

	//加载奖励物品
	for( TiXmlElement* e =e1->FirstChildElement( "item"); e != NULL; e =e->NextSiblingElement( "item"))
	{
		int itemid =XmlUtil::GetXmlAttrInt( e, "id", NO_INITVALUE);
		ItemInfoBase* pitem =ITEMRES_CFG->get_iteminfobyid( itemid);
		if( pitem == 0)
			return false;

		TaskReward* pc =FRAMEWK_NEW TaskReward();
		pc->item_ =pitem;
		pc->item_nums_ =XmlUtil::GetXmlAttrInt( e, "num", 1);

		rewards_.push_back( pc);
	}

	return true;
}
