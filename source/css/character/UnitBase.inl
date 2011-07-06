/**
* css application
*
* @category		unit module
* @package		
* @author		Created by Lu Jianfeng, 2009
*/
inline 
S_INT_64 UnitBase::get_uuid()
{
	entity_uuid uid;
	uid.parts_[1] =get_actortype();
	uid.parts_[0] =get_id();

	return uid.uuid_;
}
