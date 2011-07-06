/**
* protocol module
* 
* @category		skill package serialize
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __SKILLPKGSERIALIZEFACTORYIMPL__H__
#define __SKILLPKGSERIALIZEFACTORYIMPL__H__

#include "prolib/SerializeObjectFactory.h"

/**
* @class SkillPkgSerializeFactoryImpl
* 
* @brief 技能数据包对象序列化factory
**/
class SkillPkgSerializeFactoryImpl : public SerializeObjectFactory
{
public:
	SkillPkgSerializeFactoryImpl();

	virtual SerializeObject* to_protocol( char** pdata, S_INT_32& tlen, void* ext =0);
};

#endif	//__SKILLPKGSERIALIZEFACTORYIMPL__H__
