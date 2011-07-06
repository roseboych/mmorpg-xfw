/**
* css application
*
* @category		transcript service
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "InstCtrlBase.h"

#include <string>
#include <corelib/corelibdef.h>

#include "DefaultInstCtrl.h"

INSTCTRL_REGIST_BEGIN()
	//INSTCTRL_REGIST_IMPL( DefaultInstCtrl);
	INSTCTRL_REGIST_IMPL_DEFAULT( DefaultInstCtrl);
INSTCTRL_REGIST_END()
