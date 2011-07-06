/**
* reslib application
* 
* @category		buffer config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __BUFFERDESC__H__
#define __BUFFERDESC__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "reslib/reslib_def.h"
#include <prolib/core_type.h>

#include <string>

/**
* @class BufferDesc
* 
* @brief buffer配置
**/
class BufferDesc
{
public:
	BufferDesc();

	int get_bufferid(){ return buffer_id_;}

	/**
	* 根据id校验buffer类型
	* @param idhead <prolib/datastore/buffer_define.h>定义头部类型
	**/
	bool validate_buffer( int idhead);

	bool will_affectbaseprop(){ return affectbaseprop_;}

	/**
	* 是否是系统buffer
	* @return true:是系统buffer false:是应用buffer
	**/
	bool is_sysbuffer();

	/**
	* 是否在线时间
	* @return
	**/
	bool is_onlinescope(){ return scopetype_ == 0;}

	/**
	* 是否支持脚本
	* @return
	**/
	bool is_supportscript(){ return script_file_.size() > 0;}

	NS_STL::string get_scriptfile();

public:
	//buffer编号.分别在sys_buffer,app_buffer2个块内保证唯一。
	int		buffer_id_;

	//[true/false]是否需要永久保存(下线之后是否有效)
	bool	save2db_;
	//[0/1]计时类型。0:按照在线时间计算 1:按照绝对时间计算
	char	scopetype_;
	//有效时间长度,精确到秒
	int		scopetime_;
	//运算间隔，精确到毫秒(缺省情况为500)
	int		tickstep_;

	//[true/false]是否通知他人(需要表现的buffer)
	bool	notifyother_;

	//[true/false|false]是否会影响玩家基本属性
	bool	affectbaseprop_;
	//[true/false|false]脚本是否支持tick函数
	bool	scripttick_;

	//对应的执行脚本文件名，不区分大小写
	NS_STL::string script_file_;
	NS_STL::string script_content_;
};

#endif	//__BUFFERDESC__H__
