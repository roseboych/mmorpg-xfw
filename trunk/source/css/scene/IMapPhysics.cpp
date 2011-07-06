/**
* css application
*
* @category		physics
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "IMapPhysics.h"

IMapPhysics::IMapPhysics():
res_opt_( 0)
{
}

bool IMapPhysics::init_physics( MapResInfo* minfo)
{
	this->res_opt_ =minfo;

	return true;
}

bool IMapPhysics::test_position_validate( S_FLOAT_32 x, S_FLOAT_32 y, S_FLOAT_32 z)
{
	if( x < res_opt_->leftx_ || x > res_opt_->bottomx_)
		return false;
	if( y < res_opt_->lefty_ || y >= res_opt_->bottomy_)
		return false;

	return true;
}

bool IMapPhysics::test_position_validate( const coordinate_pointer& pos)
{
	return test_position_validate( pos.x_, pos.y_, pos.z_);
}
