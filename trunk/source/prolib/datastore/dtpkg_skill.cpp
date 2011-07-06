/**
* skill module
*
* @category		data package define
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "prolib/datastore/dtpkg_skill.h"

PROTOCOL_NAMESPACE_BEGIN

template<class _T>
skpkg_base<_T>::skpkg_base():inherit( NO_INITVALUE, NO_INITVALUE)
{
	throw "can't directly call skpkg_base construct function";
}

//----------------------------------------skpkg_nearfight------------------------------------------------
skpkg_nearfight::skpkg_nearfight():
inherit( SERIALIZEFACTORY_SKILLPKG, SKILLDATAPKG_NEARFIGHT)
{
}

//----------------------------------------skpkg_farfight------------------------------------------------
skpkg_farfight::skpkg_farfight():
inherit( SERIALIZEFACTORY_SKILLPKG, SKILLDATAPKG_FARFIGHT)
{
}

PROTOCOL_NAMESPACE_END
