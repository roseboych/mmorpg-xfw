/**
* css application
*
* @category		physics
* @package		
* @author		Created by Lu Jianfeng, 2009
*/
#ifdef __GNUC__
#define POSSIBLY_UNUSED __attribute__ ((__unused__))
#else
#define POSSIBLY_UNUSED
#endif

inline 
bool PhysXXmlParser::get_float( TiXmlElement* node, float& f)
{
	const char * temp = XmlUtil::GetXmlText( node).c_str();

#if defined(_MSC_VER) && _MSC_VER >= 1400
	int POSSIBLY_UNUSED iFieldsAssigned = sscanf_s(temp, "%f", &f);
#else
	int POSSIBLY_UNUSED iFieldsAssigned = sscanf(temp, "%f", &f);
#endif

	if (iFieldsAssigned != 1)
		return false;

	return true;
}

inline 
bool PhysXXmlParser::get_Vec3( TiXmlElement* node, NxVec3& V)
{
	const char * temp = XmlUtil::GetXmlText( node).c_str();

	coordinate_pointer point;

#if defined(_MSC_VER) && _MSC_VER >= 1400
	int POSSIBLY_UNUSED iFieldsAssigned = sscanf_s(temp, "%f,%f,%f", &point.x_, &point.y_, &point.z_);
#else
	int POSSIBLY_UNUSED iFieldsAssigned = sscanf(temp, "%f,%f,%f", &point.x_, &point.y_, &point.z_);
#endif
	if (iFieldsAssigned != 3)
		return false;

	V = NxVec3( point.x_, point.y_, point.z_);

	return true;
}

inline 
bool PhysXXmlParser::get_Mat33( TiXmlElement* node, NxMat33& M)
{
	NxVec3 v[3];
	TiXmlElement * rotation_node = node->FirstChildElement("row");

	for( int i = 0; i < 3; ++i)
	{
		if( rotation_node == 0)
			return false;

		if( !get_Vec3( rotation_node, v[i]))
			return false;

		M.setRow( i, v[i]);

		rotation_node =rotation_node->NextSiblingElement("row");
	}

	return true;
}

inline 
bool PhysXXmlParser::get_U16( TiXmlElement* node, NxU16& u16)
{
	const char * temp = XmlUtil::GetXmlText( node).c_str();

#if defined(_MSC_VER) && _MSC_VER >= 1400
	int POSSIBLY_UNUSED iFieldsAssigned = sscanf_s(temp, "%hu", &u16);
#else
	int POSSIBLY_UNUSED iFieldsAssigned = sscanf(temp, "%hu", &u16);
#endif

	if (iFieldsAssigned != 1)
		return false;

	return true;
}
