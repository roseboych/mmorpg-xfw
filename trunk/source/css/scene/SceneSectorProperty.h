/**
* css application
*
* @category		scene manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SCENESECTORPROPERTY__H__
#define __SCENESECTORPROPERTY__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "../css_pre.h"
#include <prolib/Pro_define.h>

/**
* @class SceneSectorProperty
* 
* @brief 场景中块信息的附加数据
* 通常用来存储地块属性，比如非战斗区域
**/
class SceneSectorProperty
{
public:
	SceneSectorProperty();

protected:
	SCENEOWNER_TYPE_ENUM	sceneowner_type_;
};

#endif	//__SCENESECTORPROPERTY__H__
