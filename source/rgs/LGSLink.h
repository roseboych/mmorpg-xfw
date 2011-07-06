/**
* rgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __LGSLINK__H__
#define __LGSLINK__H__

#include <prolib/core_type.h>
#include <corelib/core/LinkBase.h>

#include "RGS_Service.h"

/**
* @class LGSLink
* 
* @brief LGS关联数据、服务器启动时创建所有定义的LGS同时分派全局索引
* 
**/
class LGSLink : public LinkBase<RGS_Service>
{
	typedef LinkBase<RGS_Service> inherit;
public:
	LGSLink();

public:
	//---------------------配置数据-------------------//
	//系统索引
	S_INT_32	sn_;
};

#endif //__LGSLINK__H__
