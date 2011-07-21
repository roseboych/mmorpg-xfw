/**
* reslib application
* 
* @category		task resource
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "reslib/tasks/TaskConfig.h"

#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/script/ScriptContext.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>
#include <corelib/log/logmacro.h>

#include "reslib/deploy/GlobalConfig.h"

#include <ace/Assert.h>

TaskConfig::TaskConfig()
{

}

TaskConfig::~TaskConfig()
{
	release();
}

void TaskConfig::release()
{
	TASKINFO_MAP::iterator eiter =tasks_res_.end();
	for( TASKINFO_MAP::iterator iter =tasks_res_.begin(); iter != eiter; ++iter)
		delete iter->second;
	tasks_res_.clear();
}

bool TaskConfig::load_taskconfig()
{
	IConfigContentSource* dsrc =GLOBALCONFIG_INS->get_confsrc();
	ACE_ASSERT( dsrc != 0);

	//读取任务基础脚本
	task_script_ =dsrc->get_txtfilecontent( "appdata/tasks", "task-base.lua");

	NS_STL::string cnt =dsrc->get_txtfilecontent( "appdata/tasks", "taskdrama.xml");
	TiXmlDocument doc;
	if( !XmlUtil::loadfromstring( (char*)cnt.c_str(), doc))
		return false;

	TiXmlElement* root =doc.RootElement();
	TiXmlElement* valtasks =root->FirstChildElement( "support-task");
	ACE_ASSERT( valtasks != 0);

	//加载可用的任务配置
	for( TiXmlElement* e =valtasks->FirstChildElement( "task"); e != NULL; e =e->NextSiblingElement( "task"))
	{
		S_INT_32 iid =XmlUtil::GetXmlAttrInt( e, "id", NO_INITVALUE);
		if( iid == NO_INITVALUE)
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "load tasks.xml error - task:%d hasn't a legal id!", iid);
			return false;
		}

		TaskInfo* ptask =get_taskinfobyid( iid);
		if( ptask != 0)
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "load tasks.xml error - task:%d redfined!", iid);
			return false;
		}

		ptask =FRAMEWK_NEW TaskInfo();
		ptask->task_id_ =iid;
		tasks_res_[ptask->task_id_] =ptask;

		ptask->is_validate_ =XmlUtil::GetXmlAttrYesNo( e, "validate", false);

		//加载任务信息
		if( !ptask->load_task())
		{
			MODULE_LOG_ERROR( MODULE_BOOT, "load task[appdata/tasks/%d] failed", iid);
			return false;
		}
	}

	//加载任务组合信息

	return true;
}

bool TaskConfig::regist_to_storyscriptcontext( app::script::ScriptContext& context)
{
	//注册任务基础脚本
	if( !context.run_script( task_script_.c_str()))
	{
		MODULE_LOG_ERROR( MODULE_BOOT, "regist appdata/tasks/task-base.lua script file failed");
		return false;
	}

	//注册各个副本的脚本
	TASKINFO_MAP::iterator eiter =tasks_res_.end();
	for( TASKINFO_MAP::iterator iter =tasks_res_.begin(); iter != eiter; ++iter)
	{
		TaskInfo* ptask =iter->second;
		if( ptask->fun_type_ == FUNSUPPORT_SCRIPT)
		{
			ACE_ASSERT( ptask->task_script_ != "");
			if( !context.run_script( ptask->task_script_.c_str()))
			{
				MODULE_LOG_ERROR( MODULE_BOOT, "regist appdata/tasks/%d/server.lua script file failed", ptask->task_id_);
				return false;
			}
		}
	}

	return true;
}

TaskInfo* TaskConfig::get_taskinfobyid( S_INT_32 id)
{
	TASKINFO_MAP::iterator fiter =tasks_res_.find( id);
	if( fiter == tasks_res_.end())
		return 0;
	return fiter->second;
}
