/**
* world config
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
* 定义了和地图配置相关的常量
*/
#ifndef __WORLD_DEFINE__H__
#define __WORLD_DEFINE__H__

#include <prolib/core_type.h>
#include <prolib/datastore/character_define.h>

//格子直径	10m
#define MAP_PLANE_DIAMETER		1000
//可视范围90m
//可视区域格子数 VIEW_PLANE_RADIUS*2+1=VIEW_PLANE_NUM
#define VIEW_PLANE_NUM			9
#define VIEW_PLANE_RADIUS		4

//无效的格子
#define INVALIDATE_PANEINDEX	0x7FFFFFFF

//地图sector的属性定义
//地块所有情况
enum SCENEOWNER_TYPE_ENUM
{
	//争议地区
	SCENEOWNER_DISPUTE =-1,
	//中立地区
	SCENEOWNER_NEUTRAL =-2,
	//人族地区
	SCENEOWNER_HUMAN = CHR_RACE_HUMAN,
};

#endif	//__WORLD_DEFINE__H__
