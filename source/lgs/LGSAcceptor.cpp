/**
* lgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "LGSAcceptor.h"

#include "LGSModule.h"

#ifdef CORELIB_WIN32
LGS_Service* LGSAcceptor::make_handler()
{
	Player* p =LGSMODULE->get_emptyplayer();
	if( p == 0)
		return 0;

	return p->get_service();
}

#else

int LGSAcceptor::handle_input( ACE_HANDLE handle)
{
	Player* p =LGSMODULE->get_emptyplayer();
	if( p == 0)
		return 0;

	LGS_Service* s =p->get_service();

	if( acceptor_.accept( s->peer()) == -1)
	{
		LGSMODULE->release_player( p);

		MODULE_LOG_WARN( MODULE_BOOT, "GuardAcceptor::handle_input accept failed");
	}
	else if( s->open() != 0)
	{
		s->handle_close();
	}

	return 0;
}

#endif