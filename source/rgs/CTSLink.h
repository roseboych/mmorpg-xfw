/**
* rgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CTSLINK__H__
#define __CTSLINK__H__

#include <prolib/core_type.h>
#include <corelib/core/LinkBase.h>

#include "RGS_Service.h"

/**
* @class CTSLink
* 
* @brief CTS关联数据、服务器启动时创建所有定义的CTS同时分派全局索引
* 
**/
class CTSLink : public LinkBase<RGS_Service>
{
	typedef LinkBase<RGS_Service> inherit;
public:
	CTSLink();

public:
	//---------------------配置数据-------------------//
	S_INT_32	sn_;
};

#endif //__CTSLINK__H__
