/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __SERIALIZEOBJECTFACTORY__H__
#define __SERIALIZEOBJECTFACTORY__H__

#include "prolib/SerializeObject.h"

/**
* @class SerializeObjectFactory
* 
* @brief 对象序列化factory基类
**/
class SerializeObjectFactory
{
public:
	/**
	* 从内存序列化对象
	* @param pdata
	* @param tlen
	* @ext
	* @return
	**/
	virtual SerializeObject* to_protocol( char** pdata, S_INT_32& tlen, void* ext =0) =0;
};

#endif	//__SERIALIZEOBJECTFACTORY__H__
