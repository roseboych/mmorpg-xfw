/**
* rgs application
*
* @category		controller
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#include "ControllerAcceptor.h"

#include "../RGSModule.h"
#include "ControllerLink.h"

#ifdef CORELIB_WIN32
Controller_Service* ControllerAcceptor::make_handler()
{
	ControllerLink* pLink =RGSMODULE->get_controllerlink();
	if( !pLink->initialize_link())
		return 0;

	return pLink->get_controllerservice();
}

#else

int ControllerAcceptor::handle_input( ACE_HANDLE handle)
{
	ControllerLink* pLink =RGSMODULE->get_controllerlink();
	if( !pLink->initialize_link())
		return 0;

	Controller_Service* s =pLink->get_controllerservice();

	if( acceptor_.accept( s->peer()) == -1)
	{
		MODULE_LOG_WARN( MODULE_BOOT, "ControllerAcceptor::handle_input accept failed");
		
		pLink->reset();
	}
	else if( s->open() != 0)
	{
		s->handle_close();

		pLink->reset();
	}

	return 0;
}

#endif