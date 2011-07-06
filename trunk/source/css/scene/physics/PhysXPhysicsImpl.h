/**
* css application
*
* @category		physics
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PHYSXPHYSICSIMPL__H__
#define __PHYSXPHYSICSIMPL__H__

#include <NxPhysics.h>

#include "IMapPhysics.h"
#include "PhysxDataStruct.h"

class PhysXXmlParser;

/**
* @class raycast_query_report
* 
* @brief 射线检测查询返回
* 
**/
class raycast_query_callback : public NxSceneQueryReport
{
public:
	raycast_query_callback( NxRaycastHit& hit):hit_(hit){}

private:
	virtual	NxQueryReportResult	onBooleanQuery(void* userData, bool result){ return NX_SQR_CONTINUE; }
	virtual	NxQueryReportResult	onShapeQuery(void* userData, NxU32 nbHits, NxShape** hits){ return NX_SQR_CONTINUE; }
	virtual	NxQueryReportResult	onSweepQuery(void* userData, NxU32 nbHits, NxSweepQueryHit* hits){ return NX_SQR_CONTINUE; }

	/**
	* 指出里射线检测查询
	**/
	virtual	NxQueryReportResult	onRaycastQuery( void* userData, NxU32 nbHits, const NxRaycastHit* hits)
	{
		if( nbHits > 0)
		{
			hit_ = *hits;
			return NX_SQR_ABORT_ALL_QUERIES;
		}
		else
		{
			return NX_SQR_CONTINUE;
		}
	}

private:
	//检测结果
	NxRaycastHit& hit_; 
};

/**
* @class PhysXPhysicsImpl
* 
* @brief 基于physx的地图物理系统实现.
* move_check:基于射线检测的移动合法行检测
* 
**/
class PhysXPhysicsImpl : public IMapPhysics
{
	friend class PhysXXmlParser;

	typedef IMapPhysics	inherit;
public:
	PhysXPhysicsImpl();

	//-----------------------------------------IMapPhysics Implement-------------------------------------//
	/**
	* @see IMapPhysics::init_physics( MapResInfo* minfo)
	**/
	virtual bool init_physics( MapResInfo* minfo);

	/**
	* @see IMapPhysics::uninit_physics()
	**/
	virtual void uninit_physics();

	/**
	* @see IMapPhysics::move_check( const coordinate_pointer& startpos, const coordinate_pointer& targetpos)
	**/
	virtual bool move_check( const coordinate_pointer& startpos, const coordinate_pointer& targetpos);
	virtual bool move_check( S_FLOAT_32 ox, S_FLOAT_32 oy, S_FLOAT_32 oz, S_FLOAT_32 x, S_FLOAT_32 y, S_FLOAT_32 z);

	/**
	* 获取检测结果
	**/
	NxRaycastHit& get_hitresult(){ return hit_result_;}

	int get_shape_group_mask( unsigned int groupMask, unsigned int& mask1, NxGroupsMask& mask2);
	int get_ray_group_mask( unsigned int groupMask, unsigned int& mask1, NxGroupsMask& mask2);

protected:
	/**
	* 射线检测
	* @param from	开始点
	* @param dir	方向
	* @param distance	距离
	* @param mask1
	* @param mask2	分组mask
	**/
	void* do_raycasting_closest( NxVec3& from, NxVec3& dir, float distance, unsigned int mask1, const NxGroupsMask& mask2);

	/**
	* 加载物理数据
	* @return 
	**/
	bool load_physics_data();

private:
	//地图物理数据
	PhysXXmlParser*	data_parser_;
	unsigned int	mask1_;
	NxGroupsMask	mask2_;

private:
	//射线检测
	raycast_query_callback query_report_;
	//射线检测返回
	NxRaycastHit	hit_result_;
	//是否支持组内掩码
	bool is_groupmask_flag_;
	//重力加速度
	rotation_pointer	gravity_axis_;
	NxCookingInterface* cooking_;
	//物理查询对象
	NxSceneQuery*   phys_scenequery_;
	//物理资源场景
	NxScene*		phys_scene_;
	//sdk资源
	NxPhysicsSDK*	phys_sdk_;
};

#endif	//__PHYSXPHYSICSIMPL__H__
