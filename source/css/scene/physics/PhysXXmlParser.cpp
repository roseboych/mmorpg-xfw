/**
* css application
*
* @category		physics
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "PhysXXmlParser.h"

#include "physxbase64.h"
#include "PhysXPhysicsImpl.h"

PhysXXmlParser::PhysXXmlParser( PhysXPhysicsImpl& processor):physx_processor_(processor), id_generator_(1)
{
}

PhysXXmlParser::~PhysXXmlParser()
{
	unload_physics_data();
}

void PhysXXmlParser::unload_physics_data()
{
	for( PHYSX_ACTORS::iterator iter2 = static_actors_.begin(); iter2 != static_actors_.end(); ++iter2)
	{
		physx_actor_info* user_data = (physx_actor_info*)((*iter2)->userData);
		userdata_pool_.destroy( user_data);
		physx_processor_.phys_scene_->releaseActor(*(*iter2));
	}
	static_actors_.clear();

	for( PHYSX_DATA_TMP::iterator iter =heightfield_mesh_datas_.begin(); iter != heightfield_mesh_datas_.end(); ++iter)
	{
		physx_data_tmp& dt =(*iter);
		switch( dt.type_)
		{
		case SHAPE_MESH:
			physx_processor_.phys_sdk_->releaseTriangleMesh(*(NxTriangleMesh*)( dt.data_));
			break;
		case SHAPE_HEIGHTFIELD:
			physx_processor_.phys_sdk_->releaseHeightField(*(NxHeightField*)( dt.data_));
			break;
		}
	}
	heightfield_mesh_datas_.clear();

	actordesc_.clear();
}

bool PhysXXmlParser::load_physics_data( NS_STL::string& content)
{
	TiXmlDocument doc;
	if( XmlUtil::loadfromstring( (char*)content.c_str(), doc))
		return false;
	//清空数据
	actordesc_.clear();

	NS_STL::vector<NxActorDesc*> actorDesc_v(1024);

	TiXmlElement* root_elem = doc.RootElement();
	//load objects
	TiXmlElement* objects = root_elem->FirstChildElement( "objects");
	for( TiXmlElement* obj = objects->FirstChildElement( "object"); obj != NULL; obj =obj->NextSiblingElement( "object"))
	{
		NxActorDesc* actorDesc =0;
		if( !parse_obj_node( obj, actorDesc, STATIC_ACTOR_GROUP_MASK))
		{
			const char* name = obj->Attribute("name");
			goto L_EXIT_FAIL;
		}

		actorDesc_v.push_back( actorDesc);
	}

	//load datas
	TiXmlElement* datas = root_elem->FirstChildElement( "datas");
	if( datas)
	{
		for( TiXmlElement* obj =datas->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
		{
			physx_data_tmp sdk_data;
			if( !parse_data_node( obj, sdk_data))
			{
				const char * name = obj->Attribute("name");
				goto L_EXIT_FAIL;
			}

			heightfield_mesh_datas_.push_back( sdk_data);
		}
	}

	//创建对象
	for( size_t i = 0; i < actorDesc_v.size(); ++i)
	{
		NxActor * actor = physx_processor_.phys_scene_->createActor( *actorDesc_v[i]);
		if( actor)
			static_actors_.push_back(actor);
		else
			goto L_EXIT_FAIL;
	}

	//释放NxActorDesc对象
	for(size_t i = 0; i < actorDesc_v.size(); ++i)
		release_actorDesc( actorDesc_v[i], true);
	
	return true;

L_EXIT_FAIL:

	for(size_t i = 0; i < actorDesc_v.size(); ++i)
		release_actorDesc( actorDesc_v[i]);
	actorDesc_v.clear();

	//部分创建的actor销毁
	for(size_t i =0; i < static_actors_.size(); ++i)
		physx_processor_.phys_scene_->releaseActor( *static_actors_[i]);
	static_actors_.clear();

	//销毁已创建的heightField,和meshData数据
	for( size_t i =0; i < heightfield_mesh_datas_.size(); ++i)
	{
		switch( heightfield_mesh_datas_[i].type_)
		{
		case SHAPE_MESH:
			physx_processor_.phys_sdk_->releaseTriangleMesh( *(NxTriangleMesh*)( heightfield_mesh_datas_[i].data_));

			break;
		case SHAPE_HEIGHTFIELD:
			physx_processor_.phys_sdk_->releaseHeightField( *(NxHeightField*)( heightfield_mesh_datas_[i].data_));

			break;
		}
	}
	heightfield_mesh_datas_.clear();

	return false;
}

bool PhysXXmlParser::parse_obj_node( TiXmlElement* node, NxActorDesc*& actorDesc, unsigned int group_mask)
{
	actorDesc = actordesc_pool_.construct();
	if( !actorDesc)
		return false;

	NS_STL::vector<NxShapeDesc*> shapeDesc_v(1024);
	unsigned int mask1;
	NxGroupsMask mask2;
	NxVec3 temp_row;

	physx_actor_info * user_data = userdata_pool_.construct();
	if( !user_data)
		goto L_FAIL_EXIT;

	actorDesc->userData = (void *)user_data;

	user_data->id_ = generate_iid();
	user_data->name_ = XmlUtil::GetXmlAttrStr( node, "name");

	TiXmlElement* trans = node->FirstChildElement( "translation");
	if( trans == 0)
		goto L_FAIL_EXIT;

	if( !get_Vec3( trans, actorDesc->globalPose.t))
		goto L_FAIL_EXIT;
	user_data->pos_.reset( actorDesc->globalPose.t.x, actorDesc->globalPose.t.y, actorDesc->globalPose.t.z);

	//Rotation node
	TiXmlElement* rotat = node->FirstChildElement( "rotation");
	if( rotat == 0)
		goto L_FAIL_EXIT;
	if( !get_Mat33( rotat, actorDesc->globalPose.M))
		goto L_FAIL_EXIT;

	temp_row = actorDesc->globalPose.M.getRow(0);
	user_data->rotate_[0].reset(temp_row.x, temp_row.y, temp_row.z);
	temp_row = actorDesc->globalPose.M.getRow(1);
	user_data->rotate_[1].reset(temp_row.x, temp_row.y, temp_row.z);
	temp_row = actorDesc->globalPose.M.getRow(2);
	user_data->rotate_[2].reset(temp_row.x, temp_row.y, temp_row.z);

	//shapedesc
	for( TiXmlElement* sdesc = node->FirstChildElement("shapedesc"); sdesc != NULL; sdesc = sdesc->NextSiblingElement("shapedesc"))
	{
		NS_STL::string str_type = XmlUtil::GetXmlAttrStr( sdesc, "type");
		if( str_type == "box")
		{
			NxBoxShapeDesc * desc = boxdesc_pool_.construct();
			if( !parse_shape_node_box( sdesc, desc))
			{
				if( desc)
					boxdesc_pool_.destroy( desc);
				goto L_FAIL_EXIT;
			}

			shapeDesc_v.push_back(desc);
		}
		else if ( str_type == "capsule")
		{
			NxCapsuleShapeDesc * desc = capsuledesc_pool_.construct();
			if( !parse_shape_node_capsule( sdesc, desc))
			{
				if(desc)
					capsuledesc_pool_.destroy(desc);
				goto L_FAIL_EXIT;
			}

			shapeDesc_v.push_back(desc);
		}
		else if( str_type == "sphere")
		{
			NxSphereShapeDesc * desc = spheredesc_pool_.construct();
			if( !parse_shape_node_sphere( sdesc, desc))
			{
				if(desc)
					spheredesc_pool_.destroy(desc);
				goto L_FAIL_EXIT;
			}

			shapeDesc_v.push_back(desc);
		}
		else if (str_type == "plane")
		{
			NxPlaneShapeDesc * desc = planedesc_pool_.construct();
			if( !parse_shape_node_plane( sdesc, desc))
			{
				if(desc)
					planedesc_pool_.destroy(desc);
				goto L_FAIL_EXIT;
			}

			shapeDesc_v.push_back(desc);
		}
		else if( str_type == "trianglemesh")
		{
			physx_actor_desc actor_desc;
			actor_desc.actordesc_ = actorDesc;
			NxTriangleMeshShapeDesc * desc = meshdesc_pool_.construct();
			if( !parse_shape_node_mesh( sdesc, desc))
			{
				if(desc)
					meshdesc_pool_.destroy(desc);
				goto L_FAIL_EXIT;
			}

			shapeDesc_v.push_back(desc);
			actor_desc.desc_ = desc;
			NS_STL::string data_name = XmlUtil::GetXmlAttrStr( sdesc,"data");
			if( data_name == "")
				goto L_FAIL_EXIT;

			actordesc_.insert( NS_STL::make_pair( data_name, actor_desc));
		}
		else if( str_type == "heightfield")
		{
			physx_actor_desc actor_desc;
			actor_desc.actordesc_ = actorDesc;

			NxHeightFieldShapeDesc * desc = heightfielddesc_pool_.construct();
			if( !parse_shape_node_heigthfield( sdesc, desc))
			{
				if(desc)
					heightfielddesc_pool_.destroy(desc);
				goto L_FAIL_EXIT;
			}
			shapeDesc_v.push_back(desc);

			actor_desc.desc_ = desc;

			NS_STL::string data_name = XmlUtil::GetXmlAttrStr( sdesc,"data");
			if( data_name == "")
				goto L_FAIL_EXIT;

			NS_STL::string axis = XmlUtil::GetXmlAttrStr( sdesc,"axis");
			if( axis != "")
			{
				if( axis == "z")
					desc->localPose.M.rotX( NxPiF32/2);
				else if( axis == "x")
					goto L_FAIL_EXIT;
			}

			actordesc_.insert( NS_STL::make_pair( data_name, actor_desc));
		}
		else
			goto L_FAIL_EXIT;
	}

	//获取掩码
	int get_mask_result = physx_processor_.get_shape_group_mask( group_mask, mask1, mask2);
	if( !get_mask_result)
	{
		goto L_FAIL_EXIT;
	}

	//push shapeDesc
	for( size_t i = 0; i < shapeDesc_v.size(); ++i)
	{
		if( get_mask_result == 1)
		{
			shapeDesc_v[i]->group = mask1;
			shapeDesc_v[i]->groupsMask = mask2;
		}

		actorDesc->shapes.pushBack( shapeDesc_v[i]);
	}
	
	return true;

L_FAIL_EXIT:
	release_actorDesc( actorDesc);

	return false;
}

bool PhysXXmlParser::parse_shape_node_base( TiXmlElement* node, NxShapeDesc* desc, float& scale)
{
	//Translation
	if( node == 0)
		return false;

	TiXmlElement* tran =node->FirstChildElement( "translation");
	if( tran == 0)
		return false;

	if( !get_Vec3( tran, desc->localPose.t))
		return false;

	//Rotation node
	TiXmlElement* rotat =node->FirstChildElement( "rotation");
	if( rotat == 0)
		return false;

	if( !get_Mat33( rotat, desc->localPose.M))
		return false;

	//Scale node
	TiXmlElement* sc =node->FirstChildElement( "scale");
	if( sc == 0)
		return false;

	if( !get_float( sc, scale))
		return false;

	//WireColor node. pass
	TiXmlElement* wc =node->FirstChildElement( "wirecolor");
	if( wc == 0)
		return false;

	return true;
}

bool PhysXXmlParser::parse_shape_node_box( TiXmlElement* node, NxBoxShapeDesc* desc)
{
	if( !desc)
		return false;

	float scale =0.f;
	if( !parse_shape_node_base( node, desc, scale))
		return false;

	//Dimensions node
	TiXmlElement* dim =node->FirstChildElement( "dimension");
	if( dim == 0)
		return false;

	if( !get_Vec3( dim, desc->dimensions)) 
		return false;

	desc->dimensions *= scale;

	return true;
}

bool PhysXXmlParser::parse_shape_node_capsule( TiXmlElement* node, NxCapsuleShapeDesc* desc)
{
	if( desc == 0)
		return false;

	float scale =0.f;
	if( !parse_shape_node_base( node, desc, scale))
		return false;

	//Radius node
	TiXmlElement* radius =node->FirstChildElement( "radius");
	if( radius == 0)
		return false;

	if( !get_float( radius, desc->radius)) 
		return false;

	desc->radius *= scale;

	//Height node
	TiXmlElement* height =node->FirstChildElement( "height");
	if( height == 0)
		return false;

	if( !get_float( height, desc->height)) 
		return false;

	desc->height *= scale;

	return true;
}

bool PhysXXmlParser::parse_shape_node_sphere( TiXmlElement* node, NxSphereShapeDesc* desc)
{
	if( desc == 0)
		return false;

	float scale =0.f;
	if( !parse_shape_node_base( node, desc, scale))
		return false;

	//Radius node
	TiXmlElement* radius =node->FirstChildElement( "radius");
	if( radius == 0)
		return false;

	if( !get_float( radius, desc->radius)) 
		return false;

	desc->radius *= scale;

	return true;
}

bool PhysXXmlParser::parse_shape_node_mesh( TiXmlElement* node, NxTriangleMeshShapeDesc* desc)
{
	if( desc == 0)
		return false;

	float scale =0.f;
	if( !parse_shape_node_base( node, desc, scale))
		return false;

#ifdef NX_SUPPORT_MESH_SCALE
	desc->scale	= scale;
#endif

	return true;
}

bool PhysXXmlParser::parse_shape_node_heigthfield( TiXmlElement* node, NxHeightFieldShapeDesc* desc)
{
	if( desc == 0)
		return false;

	float scale =0.f;
	if( !parse_shape_node_base( node, desc, scale))
		return false;

	//HeightScale
	TiXmlElement* heightscale =node->FirstChildElement("heightscale");
	if( heightscale == 0)
		return false;

	if( !get_float( heightscale, desc->heightScale)) 
		return false;
	desc->heightScale *= scale;

	//RowScale
	TiXmlElement* rowscale =node->FirstChildElement( "rowscale");
	if( rowscale == 0)
		return false;

	if( !get_float( rowscale, desc->rowScale)) 
		return false;

	desc->rowScale *= scale;

	//ColumnScale
	TiXmlElement* columnscale =node->FirstChildElement( "columnscale");
	if( columnscale == 0)
		return false;

	if( !get_float( columnscale, desc->columnScale)) 
		return false;

	desc->columnScale *= scale;

	//materialIndexHighBits
	TiXmlElement* mih =node->FirstChildElement( "materialindexhightbits");
	if( mih == 0)
		return false;
	if( !get_U16( mih, desc->materialIndexHighBits)) 
		return false;

	//holeMaterial
	TiXmlElement* hm =node->FirstChildElement( "holematerial");
	if( hm == 0)
		return false;

	if( !get_U16( hm, desc->holeMaterial)) 
		return false;

	return true;
}

bool PhysXXmlParser::parse_shape_node_plane( TiXmlElement* node, NxPlaneShapeDesc* desc)
{
	if( desc == 0)
		return false;

	float scale =0.f;
	if( !parse_shape_node_base( node, desc, scale))
		return false;

	//Size. pass.
	TiXmlElement* size =node->FirstChildElement("size");
	if( size == 0)
		return false;

	//Normal
	TiXmlElement* normal =node->FirstChildElement( "normal");
	if( normal == 0)
		return false;

	if( !get_Vec3( normal, desc->normal)) 
		return false;

	//Distance
	TiXmlElement* distance =node->FirstChildElement( "distance");
	if( distance == 0)
		return false;

	if( !get_float( distance, desc->d)) 
		return false;

	return true;
}

bool PhysXXmlParser::parse_data_node( TiXmlElement* node, physx_data_tmp& sdk_data)
{
	bool ret =false;

	NS_STL::string str_value = node->Value();
	if( str_value == "heightfiled")
		ret = parse_data_node_heightfield( node, sdk_data);
	else if( str_value == "trianglemesh")
		ret = parse_data_node_mesh( node, sdk_data);
	else
		return false;

	return ret;
}

bool PhysXXmlParser::parse_data_node_heightfield( TiXmlElement* node, physx_data_tmp& sdk_data)
{
	static NxU32 temp_samples[1025*1025];

	NxHeightField* heightField =0;

	NS_STL::string data_name =XmlUtil::GetXmlAttrStr( node, "name");
	int rows =XmlUtil::GetXmlAttrInt( node, "rows");
	int columns =XmlUtil::GetXmlAttrInt( node, "columns");
	NS_STL::string data = XmlUtil::GetXmlText( node);

	if( data_name != "" || data != "")
		return false;

	ACTOR_DESC_MAP::iterator iter = actordesc_.find( data_name);
	if( iter == actordesc_.end())
		return false;

	physx_actor_desc* temp_actor_desc = &(iter->second);
	NxActorDesc* actorDesc_ptr = temp_actor_desc->actordesc_;
	NxHeightFieldShapeDesc* shapeDesc = (NxHeightFieldShapeDesc *)temp_actor_desc->desc_;

	NxHeightFieldDesc desc;

	desc.nbColumns = boost::lexical_cast<NxU32>(columns);
	desc.nbRows = boost::lexical_cast<NxU32>(rows);
	int sample_num = desc.nbColumns * desc.nbRows;
	if( sample_num <= 1025*1025)
		desc.samples = &temp_samples; 
	else
		desc.samples = new NxU32[sample_num]; 

	desc.sampleStride  = sizeof( NxU32);

	size_t out_len = sample_num * desc.sampleStride;
	if( !physx_base64::decode( desc.samples, data.c_str(), out_len) || (out_len != sample_num * desc.sampleStride))
		goto L_FAIL_EXIT;

	heightField = physx_processor_.phys_sdk_->createHeightField( desc);
	if( !heightField)
		goto L_FAIL_EXIT;

	if( sample_num > 1025*1025)
	{
		delete [] desc.samples;
		desc.samples = NULL;
	}

	shapeDesc->heightField = heightField;
	sdk_data.type_ = SHAPE_HEIGHTFIELD;
	sdk_data.data_ = heightField;

	return true;

L_FAIL_EXIT:
	if( sample_num > 1025*1025 && desc.samples != NULL)
		delete [] desc.samples;

	return false;
}

bool PhysXXmlParser::parse_data_node_mesh( TiXmlElement* node, physx_data_tmp& sdk_data)
{
	static NxVec3 temp_points[1024];
	static NxU32  temp_Triangles[3072];

	PhysxMemoryReadBuffer readBuffer(NULL);
	cooking_stream_.clear();

	NS_STL::string data_name =XmlUtil::GetXmlAttrStr( node, "name");
	NS_STL::string ptCount =XmlUtil::GetXmlAttrStr( node, "ptcount");
	NS_STL::string triCount =XmlUtil::GetXmlAttrStr( node, "tricount");
	if( data_name == "" || ptCount == "" || triCount == "")
		return false;

	//Points node
	TiXmlElement* points =node->FirstChildElement( "points");
	if( points == 0)
		return false;

	NS_STL::string pt_data = XmlUtil::GetXmlText( points);
	if( pt_data == "")
		return false;

	//Triangles
	TiXmlElement* triangles =node->FirstChildElement( "triangles");
	if( triangles == 0)
		return false;

	NS_STL::string tri_data = XmlUtil::GetXmlText( triangles);
	if( tri_data == "")
		return false;

	ACTOR_DESC_MAP::iterator iter = actordesc_.find( data_name);
	if( iter == actordesc_.end())
		return false;

	physx_actor_desc* temp_actor_desc = &(iter->second);

	NxActorDesc* actorDesc_ptr = temp_actor_desc->actordesc_;
	NxTriangleMeshShapeDesc* shapeDesc = (NxTriangleMeshShapeDesc*)temp_actor_desc->desc_;

	NxTriangleMeshDesc desc;
	desc.numVertices = boost::lexical_cast<NxU32>(ptCount.c_str());
	desc.numTriangles = boost::lexical_cast<NxU32>(triCount.c_str());
	desc.pointStrideBytes = sizeof(NxVec3);  
	desc.triangleStrideBytes = 3*sizeof(NxU32);
	desc.flags = 0;

	if( desc.numVertices <= 1024)
		desc.points = &temp_points;  
	else
		desc.points = new NxVec3[desc.numVertices];

	if(desc.numTriangles <= 3072)
		desc.triangles = &temp_Triangles;
	else
		desc.triangles = new NxU32[desc.numTriangles];

	//Points node
	void* point_ptr =(void*)desc.points;
	size_t out_len = desc.numVertices * desc.pointStrideBytes;
	if( !physx_base64::decode( point_ptr, pt_data.c_str(), out_len) || ( out_len != desc.numVertices * desc.pointStrideBytes))
		goto L_FAIL_EXIT;

	//Triangles
	void* triangles_ptr =(void*)desc.triangles;
	out_len = desc.numTriangles * desc.triangleStrideBytes;
	if( !physx_base64::decode( triangles_ptr, tri_data.c_str(), out_len) || (out_len != desc.numTriangles * desc.triangleStrideBytes))
		goto L_FAIL_EXIT;

	if( !physx_processor_.cooking_->NxCookTriangleMesh( desc, cooking_stream_))
		goto L_FAIL_EXIT;

	readBuffer =PhysxMemoryReadBuffer( cooking_stream_.data_);
	NxTriangleMesh* triangleMesh =physx_processor_.phys_sdk_->createTriangleMesh( readBuffer);
	if( desc.numVertices > 1024)
	{
		delete [] desc.points;
		desc.points = NULL;
	}

	if( desc.numTriangles > 3072)		
	{
		delete [] desc.triangles;
		desc.triangles = NULL;
	}

	shapeDesc->meshData = triangleMesh;
	sdk_data.type_ = SHAPE_MESH;
	sdk_data.data_ = triangleMesh;

	return true;

L_FAIL_EXIT:
	if( desc.numVertices > 1024 && desc.points)
		delete [] desc.points;
	if( desc.numTriangles > 3072 && desc.triangles)
		delete [] desc.triangles;

	return false;
}

void PhysXXmlParser::release_actorDesc( NxActorDesc * actorDesc, bool ok)
{
	if( !actorDesc)
		return;

	for(NxArray<NxShapeDesc*>::iterator iter = actorDesc->shapes.begin(); iter != actorDesc->shapes.end(); ++iter)
	{
		switch((*iter)->getType())
		{
		case NX_SHAPE_PLANE:
			planedesc_pool_.destroy( (NxPlaneShapeDesc*)(*iter));
			break;
		case NX_SHAPE_SPHERE:
			spheredesc_pool_.destroy( (NxSphereShapeDesc*)(*iter));
			break;
		case NX_SHAPE_BOX:
			boxdesc_pool_.destroy( (NxBoxShapeDesc*)(*iter));
			break;
		case NX_SHAPE_CAPSULE:
			capsuledesc_pool_.destroy( (NxCapsuleShapeDesc*)(*iter));
			break;
		case NX_SHAPE_MESH:
			meshdesc_pool_.destroy( (NxTriangleMeshShapeDesc*)(*iter));
			break;
		case NX_SHAPE_HEIGHTFIELD:
			heightfielddesc_pool_.destroy( (NxHeightFieldShapeDesc*)(*iter));
			break;
		}
	}

	if( actorDesc->userData && !ok)
		userdata_pool_.destroy( (physx_actor_info*)actorDesc->userData);

	actordesc_pool_.destroy( actorDesc);
}
