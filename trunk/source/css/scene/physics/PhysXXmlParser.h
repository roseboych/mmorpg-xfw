/**
* css application
*
* @category		physics
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PHYSXXMLPARSER__H__
#define __PHYSXXMLPARSER__H__

#define NOMINMAX

#include <boost/pool/object_pool.hpp>
#include <boost/lexical_cast.hpp>

#include <corelib/xml/tinyxml.h>
#include <corelib/xml/XmlUtil.h>

#include "PhysxDataStruct.h"
#include "PhysxMemoryStream.h"
#include "PhysXPhysicsImpl.h"

/**
* @class PhysXXmlParser
* 
* @brief 物理系统数据文件解析
* 
**/
class PhysXXmlParser
{
public:
	typedef NS_STL::map< NS_STL::string, physx_actor_desc>	ACTOR_DESC_MAP;
	typedef NS_STL::vector<physx_data_tmp>	PHYSX_DATA_TMP;
	typedef NS_STL::vector<NxActor *>	PHYSX_ACTORS;

public:
	PhysXXmlParser( PhysXPhysicsImpl& processor);
	~PhysXXmlParser();
	  
	bool load_physics_data( NS_STL::string& content);
	void unload_physics_data();

private:
	bool parse_obj_node( TiXmlElement* node, NxActorDesc*& actorDesc, unsigned int group_mask);
	bool parse_data_node( TiXmlElement* node, physx_data_tmp& sdk_data);

	bool parse_shape_node_base( TiXmlElement* node, NxShapeDesc* desc, float& scale);
	bool parse_shape_node_box( TiXmlElement* node, NxBoxShapeDesc* desc);
	bool parse_shape_node_capsule( TiXmlElement* node, NxCapsuleShapeDesc* capsuleDesc);
	bool parse_shape_node_sphere( TiXmlElement* node, NxSphereShapeDesc* sphereDesc);
	bool parse_shape_node_mesh( TiXmlElement* node, NxTriangleMeshShapeDesc* desc);
	bool parse_shape_node_heigthfield( TiXmlElement* node, NxHeightFieldShapeDesc* desc);
	bool parse_shape_node_plane( TiXmlElement* node, NxPlaneShapeDesc* planeDesc);

	bool parse_data_node_heightfield( TiXmlElement* node, physx_data_tmp& sdk_data);
	bool parse_data_node_mesh( TiXmlElement* node, physx_data_tmp& sdk_data);

	void release_actorDesc( NxActorDesc * actorDesc, bool ok = false);

	inline bool get_float( TiXmlElement* node, float& f);
	inline bool get_Vec3( TiXmlElement* node, NxVec3& V);
	inline bool get_Mat33( TiXmlElement* node, NxMat33& M);
	inline bool get_U16( TiXmlElement* node, NxU16& u16);

	S_UINT_32 generate_iid(){ return id_generator_++;}

protected:
	boost::object_pool<NxActorDesc>				actordesc_pool_;
	//下面的buff都NxActorDesc对象绑定
	boost::object_pool<physx_actor_info>		userdata_pool_;
	boost::object_pool<NxBoxShapeDesc>			boxdesc_pool_;
	boost::object_pool<NxSphereShapeDesc>		spheredesc_pool_;
	boost::object_pool<NxCapsuleShapeDesc>		capsuledesc_pool_;
	boost::object_pool<NxHeightFieldShapeDesc>	heightfielddesc_pool_;
	boost::object_pool<NxTriangleMeshShapeDesc>	meshdesc_pool_;
	boost::object_pool<NxPlaneShapeDesc>		planedesc_pool_;

	//当前解析的actor id自动生成器
	S_UINT_32	id_generator_;
	//mesh用
	PhysxMemoryWriteBuffer	cooking_stream_;

	ACTOR_DESC_MAP	actordesc_;
	//heightfieldmesh数据
	PHYSX_DATA_TMP	heightfield_mesh_datas_;

	//静态actor列表
	PHYSX_ACTORS static_actors_;

	//物理处理器
	PhysXPhysicsImpl&	physx_processor_;
};

#include "PhysXXmlParser.inl"

#endif	//__PHYSXXMLPARSER__H__
