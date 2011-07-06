/**
* css application
*
* @category		physics
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "PhysXPhysicsImpl.h"

#include <corelib/mconf/GlobalConfig.h>
#include <corelib/mconf/IConfigContentSource.h>

#include "PhysXXmlParser.h"

PhysXPhysicsImpl::PhysXPhysicsImpl():
phys_sdk_( NULL),
phys_scene_( NULL),
phys_scenequery_( NULL),
cooking_( NULL),
query_report_( hit_result_),
data_parser_( 0)
{
	gravity_axis_.x_ =0;
	gravity_axis_.y_ =0;
	gravity_axis_.z_ =1;
	is_groupmask_flag_ =true;

	mask1_ = 0x80000000;
	mask2_.bits0 = mask2_.bits1 = mask2_.bits2 = 0;
	mask2_.bits3 = 0x80000000;
}

int PhysXPhysicsImpl::get_shape_group_mask( unsigned int groupMask, unsigned int& mask1, NxGroupsMask& mask2)
{
	static const int bitnum = sizeof(unsigned int) * 8;

	if( is_groupmask_flag_)
	{
		unsigned int temp_mask[4] = {0,0,0,0};
		if( groupMask == STATIC_ACTOR_GROUP_MASK)
		{
			//static actor被分在31组，组内掩码第127位
			mask1 = 31;
			mask2.bits0 = 0;
			mask2.bits1 = 0;
			mask2.bits2 = 0;
			mask2.bits3 = 0x80000000;
		}
		else
		{
			/*
			groupMask值范围[0，31*127)，对应到副本id作为掩码值，则一个cs不能产生的副本id只能在[0，31*127)之间的值。
			dynamic actor在[0，30]组，组内掩码[0，126]位
			*/
			mask1 = groupMask / 127;
			if( mask1 > 30)
				return 0;

			int temp = groupMask % 127;
			ACE_ASSERT(temp / bitnum < 4);
			temp_mask[temp / bitnum] |= (unsigned int)1 << (temp % bitnum);
			mask2.bits0 = temp_mask[0];
			mask2.bits1 = temp_mask[1];
			mask2.bits2 = temp_mask[2];
			mask2.bits3 = temp_mask[3];
		}

		return 1;
	}

	return 2;
}

int PhysXPhysicsImpl::get_ray_group_mask(unsigned int groupMask, unsigned int& mask1, NxGroupsMask& mask2)
{
	unsigned int shape_make1;
	NxGroupsMask shape_make2;
	int result = get_shape_group_mask( groupMask, shape_make1, shape_make2);
	if( result == 1)
	{
		mask1 = mask1_ | ((unsigned int)1 << shape_make1);
		mask2.bits0 = shape_make2.bits0;
		mask2.bits1 = shape_make2.bits1;
		mask2.bits2 = shape_make2.bits2;
		mask2.bits3 = mask2_.bits3 | shape_make2.bits3;
	}

	return result;
}

bool PhysXPhysicsImpl::init_physics( MapResInfo* minfo)
{
	if( !inherit::init_physics( minfo))
		return false;

	//初始化物理系统
	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	phys_sdk_ = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION, NULL, NULL, desc, &errorCode);
	if( phys_sdk_ == NULL)
		return false;

	NxSceneDesc sceneDesc;
	sceneDesc.gravity = NxVec3( gravity_axis_.x_, gravity_axis_.y_, gravity_axis_.z_)*(-9.81f);
	phys_scene_ =phys_sdk_->createScene( sceneDesc);
	if( phys_scene_ == NULL)
		return false;

	if( is_groupmask_flag_)
	{
		phys_scene_->setFilterOps( NX_FILTEROP_OR, NX_FILTEROP_OR, NX_FILTEROP_AND);
		phys_scene_->setFilterBool( true);
		NxGroupsMask zeroMask;
		zeroMask.bits0 = 0;
		zeroMask.bits1 = 0;
		zeroMask.bits2 = 0;
		zeroMask.bits3 = 0;
		phys_scene_->setFilterConstant0( zeroMask);
		phys_scene_->setFilterConstant1( zeroMask);
	}

	NxSceneQueryDesc sceneQueryDesc;
	sceneQueryDesc.report = &query_report_;
	phys_scenequery_ = phys_scene_->createSceneQuery( sceneQueryDesc);
	if( !phys_scenequery_)
		return false;

	cooking_ = NxGetCookingLib( NX_PHYSICS_SDK_VERSION);
	if( !cooking_ || !cooking_->NxInitCooking())
		return false;

	//加载物理信息
	if( !load_physics_data())
		return false;

	return true;
}

void PhysXPhysicsImpl::uninit_physics()
{
	if( phys_sdk_ != NULL)
	{
		//释放数据
		if( data_parser_)
			delete data_parser_;
		data_parser_ =0;

		if( cooking_ != NULL)
		{
			cooking_->NxCloseCooking();
			cooking_ = NULL;
		}

		if( phys_scene_ != NULL)
		{
			if( phys_scenequery_)
			{
				phys_scene_->releaseSceneQuery( *phys_scenequery_);
				phys_scenequery_ =NULL;
			}

			phys_sdk_->releaseScene( *phys_scene_);
			phys_scene_ = NULL;
		}

		NxReleasePhysicsSDK( phys_sdk_);
		phys_sdk_ = NULL;
	}
}

void* PhysXPhysicsImpl::do_raycasting_closest( NxVec3& from, NxVec3& dir, float distance, 
											  unsigned int mask1, const NxGroupsMask& mask2)
{
	ACE_ASSERT( distance <= NX_MAX_F32);

	NxRaycastHit hit;
	NxRay worldRay;
	worldRay.dir = dir;
	worldRay.dir.normalize();
	worldRay.orig = from;

	//重置信息
	hit_result_.flags = 0;

	//同步模式进行射线检测
	if( is_groupmask_flag_)
		phys_scenequery_->raycastClosestShape(worldRay, NX_ALL_SHAPES, hit_result_, mask1, distance, NX_RAYCAST_DISTANCE, &mask2);
	else
		phys_scenequery_->raycastClosestShape(worldRay, NX_ALL_SHAPES, hit_result_, 0x00000001, distance, NX_RAYCAST_DISTANCE);

	phys_scenequery_->execute();
	phys_scenequery_->finish(true);

	//result
	if( hit_result_.flags == 0)
		return 0;

	//fetch hit info
	physx_actor_info* hit_info = static_cast<physx_actor_info *>( hit_result_.shape->getActor().userData);
	hit_info->distance_ = hit_result_.distance;
	return hit_info;
}

bool PhysXPhysicsImpl::load_physics_data()
{
	if( data_parser_)
		return true;

	data_parser_ =new PhysXXmlParser( *this);

	IConfigContentSource* psrc =GLOBALCONFIG_INS->get_confsrc();
	NS_STL::string path =get_mapopt()->get_respath();
	NS_STL::string content =psrc->get_txtfilecontent( path.c_str(), "physxdata.xml");

	/*
	if( !data_parser_->load_physics_data( content))
		return false;
	*/

	return true;
}

bool PhysXPhysicsImpl::move_check( const coordinate_pointer& startpos, const coordinate_pointer& targetpos)
{
	return true;
}

bool PhysXPhysicsImpl::move_check( S_FLOAT_32 ox, S_FLOAT_32 oy, S_FLOAT_32 oz, S_FLOAT_32 x, S_FLOAT_32 y, S_FLOAT_32 z)
{

	return true;
}
