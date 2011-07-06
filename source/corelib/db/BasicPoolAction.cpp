/**
* corelib
*
* @category		db module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "corelib/db/BasicPoolAction.h"

#include "corelib/corelibdef.h"

BasicPoolAction::BasicPoolAction( int t, S_TIMESTAMP st)
:type_( t),create_time_( st)
{
}

BasicPoolAction::~BasicPoolAction()
{
}
