/**
* css application
*
* @category		player pet
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "MyPetInfo.h"

MyPetInfo::MyPetInfo()
{

}

MyPetInfo::MyPetInfo( const PRO::petdata_item& pdata)
{
	this->petdata_ =pdata;
}
