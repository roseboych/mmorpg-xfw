/**
* css application
*
* @category		transcript service
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __DEFAULTINSTCTRL__H__
#define __DEFAULTINSTCTRL__H__

#include "../css_pre.h"
#include "InstCtrlBase.h"

class DefaultInstCtrl : public InstCtrlBase
{
	INSTCTRL_DEFINE( DefaultInstCtrl);
	typedef InstCtrlBase	inherit;
public:

	DefaultInstCtrl( InstanceStoryServiceImpl* iss);
	virtual ~DefaultInstCtrl();

	virtual void init_ctrl();
	virtual bool tick( const ACE_UINT64& now_t);

protected:

};

#endif	//__DEFAULTINSTCTRL__H__
