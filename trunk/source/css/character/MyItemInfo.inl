/**
* css application
*
* @category		player item
* @package		
* @author		Created by Lu Jianfeng, 2009
*/

inline 
PRO::equipment_item& MyItemInfo::get_itemdata()
{
	return itemdata_;
}

inline 
int MyItemInfo::get_itemid()
{
	return itemdata_.itemid_;
}

inline 
int MyItemInfo::get_itemcode()
{
	return itemdata_.itemcode_;
}

inline
bool MyItemInfo::in_avatar()
{
	return itemdata_.inavatar_ != 0;
}
