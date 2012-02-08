/**
* css application
*
* @category		player pet
* @package		
* @author		Created by Lu Jianfeng, 2009
*/

inline 
PRO::petdata_item& MyPetInfo::get_petdata()
{
	return petdata_;
}

inline 
int MyPetInfo::get_petid()
{
	return petdata_.petid_;
}

inline 
int MyPetInfo::get_petcode()
{
	return petdata_.petcode_;
}

inline 
const char* MyPetInfo::get_petname()
{
	return petdata_.petname_.c_str();
}

inline 
void MyPetInfo::set_petname( const char* v)
{
	petdata_.petname_ =v;
}
