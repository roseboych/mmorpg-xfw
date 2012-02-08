/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "reslib/tasks/TaskInfo.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"
#include "reslib/items/ItemsConfig.h"
#include "reslib/items/ItemInfoBase.h"

#include "reslib/tasks/TaskParamBase.h"

#include <ace/Assert.h>


TaskInfo::TaskInfo():
task_id_( NO_INITVALUE),
is_validate_( true),
fun_type_( FUNSUPPORT_NONE),
clsimpl_param_( 0)
{
}

TaskInfo::~TaskInfo()
{
	release();
}

void TaskInfo::release()
{
	for( size_t ii =0; ii < taskconsumes_.size(); ++ii)
		delete taskconsumes_[ii];
	taskconsumes_.clear();

	for( size_t ii =0; ii < taskrewards_.size(); ++ii)
		delete taskrewards_[ii];
	taskrewards_.clear();

	if( clsimpl_param_)
		delete clsimpl_param_;
	clsimpl_param_ =0;
}

bool TaskInfo::load_task()
{
	//构造路径
	{
		res_path_ ="appdata/tasks/";
		char buf[16] ={0,};
		NS_STL::string ret =ACE_OS::itoa( task_id_, buf, 10);
		res_path_ += ret;
		res_path_ += "/";
	}

	IConfigContentSource* dsrc =GLOBALCONFIG_INS->get_confsrc();
	ACE_ASSERT( dsrc != 0);

	NS_STL::string cnt =dsrc->get_txtfilecontent( res_path_.c_str(), "task.xml");
	TiXmlDocument doc;
	if( !XmlUtil::loadfromstring( (char*)cnt.c_str(), doc))
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "load task[appdata/tasks/%d/task.xml] failed", this->task_id_);
		return false;
	}

	TiXmlElement* e =doc.RootElement();

	NS_STL::string ctype =XmlUtil::GetXmlAttrStr( e, "condtype", "");
	if( ctype == "class")
		fun_type_ =FUNSUPPORT_CLASSIMPL;
	else if( ctype == "script")
		fun_type_ =FUNSUPPORT_SCRIPT;
	ACE_ASSERT( fun_type_ == FUNSUPPORT_CLASSIMPL || fun_type_ == FUNSUPPORT_SCRIPT);

	//load task consume
	TiXmlElement* pconsume =e->FirstChildElement( "task-consume");
	if( pconsume)
	{
		for( TiXmlElement* e1 =pconsume->FirstChildElement( "item"); e1 != NULL; e1 =e1->NextSiblingElement( "item"))
		{
			int itemid =XmlUtil::GetXmlAttrInt( e1, "id", NO_INITVALUE);
			ItemInfoBase* pitem =ITEMRES_CFG->get_iteminfobyid( itemid);
			if( pitem == 0)
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "load task[%d] consume failed, consume item:%d not exist", this->task_id_, itemid);
				return false;
			}

			TaskConsume* pc =FRAMEWK_NEW TaskConsume();
			pc->item_ =pitem;
			pc->item_nums_ =XmlUtil::GetXmlAttrInt( e1, "num", 1);
			pc->must_taskitem_ =XmlUtil::GetXmlAttrYesNo( e1, "taskitem", true);
			pc->will_destroy_ =XmlUtil::GetXmlAttrYesNo( e1, "destroy", true);

			if( !pc->validate())
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "load task[%d] consume failed, consume item:%d can't pass validate", this->task_id_, itemid);
				return false;
			}

			taskconsumes_.push_back( pc);
		}
	}

	//load task reward
	TiXmlElement* preward =e->FirstChildElement( "task-reward");
	if( preward)
	{
		for( TiXmlElement* e1 =preward->FirstChildElement( "group"); e1 != NULL; e1 =e1->NextSiblingElement( "group"))
		{
			int gid =XmlUtil::GetXmlAttrInt( e1, "id", NO_INITVALUE);
			TaskRewardGroup *pgroup =FRAMEWK_NEW TaskRewardGroup();
			pgroup->group_id_ =gid;
			taskrewards_.push_back( pgroup);

			if( !pgroup->load_reward( e1))
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "load task[%d] reward failed", this->task_id_);
				return false;
			}
		}
	}

	//load c++ implementation
	if( fun_type_ == FUNSUPPORT_CLASSIMPL)
	{
		//加载对应的c++配置
		TiXmlElement* tc =e->FirstChildElement( "task-condition");
		ACE_ASSERT( tc != 0);
		NS_STL::string cimpl =XmlUtil::GetXmlAttrStr( tc, "classimpl");
		TiXmlElement* xparam =tc->FirstChildElement( cimpl);
		ACE_ASSERT( xparam != 0);

		clsimpl_param_ =TaskParamBase::create( cimpl);
		ACE_ASSERT( clsimpl_param_ != 0);
		if( !clsimpl_param_->load_params( xparam))
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "load task[%d] class implementent parameter failed", this->task_id_);
			return false;
		}
	}
	else	//FUNSUPPORT_SCRIPT
	{
		task_script_ =dsrc->get_txtfilecontent( res_path_.c_str(), "server.lua");
		if( task_script_ == "")
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "load task script[appdata/tasks/%d/server.lua] failed", this->task_id_);
			return false;
		}
	}
	
	return true;
}

TaskRewardGroup* TaskInfo::get_rewardbygid( int gid)
{
	for( size_t ii =0; ii < taskrewards_.size(); ++ii)
	{
		if( taskrewards_[ii]->group_id_ == gid)
			return taskrewards_[ii];
	}

	return 0;
}