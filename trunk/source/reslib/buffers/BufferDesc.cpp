/**
* reslib application
* 
* @category		buffer config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "reslib/buffers/BufferDesc.h"

#include <prolib/Pro_define.h>

BufferDesc::BufferDesc():
buffer_id_( NO_INITVALUE),
save2db_( false),
scopetype_( 0),
scopetime_( 0),
tickstep_( 500),
notifyother_( false),
affectbaseprop_( false),
scripttick_( false)
{
}

bool BufferDesc::validate_buffer( int idhead)
{
	int idmask =buffer_id_ & MASK_BUFFER_IDHEAD;
	if( idhead != idmask)
		return false;

	//TODO:buffer定义的其他校验

	return true;
}

bool BufferDesc::is_sysbuffer()
{
	return (buffer_id_ & BUFFERID_HEAD_SYS) > 0;
}

NS_STL::string BufferDesc::get_scriptfile()
{
	NS_STL::string ret =script_file_;

	return ret + ".lua";
}
