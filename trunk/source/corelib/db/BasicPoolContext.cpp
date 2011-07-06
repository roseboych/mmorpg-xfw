/**
* corelib
*
* @category		db module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "corelib/db/BasicPoolContext.h"

#include "corelib/corelibdef.h"

BasicPoolContext::BasicPoolContext()
:return_it_( false),
before_context_( 0),
after_context_(0),
map_id_( NO_INITVALUE),
used_it_( false)
{
}

BasicPoolContext::~BasicPoolContext(void)
{
}
