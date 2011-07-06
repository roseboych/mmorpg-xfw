/**
* protocol module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __SERIALIZEFACTORYCONTAINER__H__
#define __SERIALIZEFACTORYCONTAINER__H__

#include "prolib/core_type.h"
#include "prolib/SerializeObjectFactory.h"
#include "prolib/SingletonT.h"

/**
* @class SerializeFactoryContainer
* 
* @brief 序列化对象factory的容器
**/
class SerializeFactoryContainer : public SingletonT<SerializeFactoryContainer>
{
public:
	SerializeFactoryContainer();
	~SerializeFactoryContainer();

	/**
	* 根据编号获取支持的factory
	* @param type	SERIALIZEFACTORY_ENUM定义的类型
	* @return SerializeObjectFactory*
	**/
	SerializeObjectFactory* get_factory( S_INT_8 type);

private:
	//定义了所有支持的factory
	SerializeObjectFactory*	factory_[SERIALIZEFACTORY_MAX];
};

#endif	//__SERIALIZEFACTORYCONTAINER__H__
