/**
* buffer module
* 
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __BUFFER_DEFINE__H__
#define __BUFFER_DEFINE__H__

#include <prolib/core_type.h>

#define MASK_BUFFER_IDHEAD	0xFF000000
#define MASK_BUFFER_IDBIT	0x00FFFFFF
#define MASK_BUFFER_LENGTH	24

//IDHEAD
//系统buffer类型 0x10000000开始.(比如：在线检测,系统活动)
#define BUFFERID_HEAD_SYS	0x10000000

//应用定义的buff类型 0x20000000开始
#define BUFFERID_HEAD_APP	0x20000000

//定义的buffer编号
#define BUFFERID_SYS_ONLINET	0x10000001	//连续上线时间

PROTOCOL_NAMESPACE_BEGIN

//buffer信息
struct bufferdata_item
{
	//buffer编号
	S_INT_32	bufferid_;
	//类型 0:系统buffer 1:应用buffer
	S_INT_8		types_;
	S_INT_32	createtime_;
	S_INT_32	scopetime_;
};

PROTOCOL_NAMESPACE_END

#endif	//__BUFFER_DEFINE__H__
