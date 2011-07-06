/**
* css application
*
* @category		physics
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PHYSXDATASTRUCT__H__
#define __PHYSXDATASTRUCT__H__

#include <prolib/core_type.h>
#include <string>

#include "../cmsvr_struct.h"

#include "NxPhysics.h"
#include "NxCooking.h"

/**
* @class physx_actor_info
* 
* @brief 物理配置数据中actor自定义数据
* 
**/
struct physx_actor_info
{
	//actor_info
	unsigned int		id_;
	NS_STL::string		name_;
	coordinate_pointer	pos_;
	rotation_pointer	rotate_[3];
	//距离碰撞的距离
	float				distance_;
};

/**
* @class physx_actor_desc
* 
* @brief 暂存mesh和heightFeild数据的结构。
* mesh，heightFeild得解析需要分两部分完成，所以第一次碰到时暂存解析信息
* 
**/
struct physx_actor_desc
{
	NxActorDesc*	actordesc_;	
	NxShapeDesc*	desc_;
};

/**
* @class physx_sdk_data_tmp
* 
* @brief 暂存由NxPhysicsSDK对象生成的NxMeshData和NxHeightField数据。unload_physics_data时，释放这些数据对象
* 
**/
struct physx_data_tmp
{
	int		type_;
	void*	data_;
};

/**
* 支持的物理数据类型
**/
enum SHAPE_TYPE
{
	SHAPE_PLANE,
	SHAPE_SPHERE,
	SHAPE_BOX,
	SHAPE_CAPSULE,
	SHAPE_MESH,
	SHAPE_HEIGHTFIELD,
};

//static actor 的分组mask
#define STATIC_ACTOR_GROUP_MASK NO_INITVALUE

#endif	//__PHYSXDATASTRUCT__H__
