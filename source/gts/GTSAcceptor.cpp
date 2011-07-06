/**
* lgs application
*
* @category		net
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "GTSAcceptor.h"

#include "GTSModule.h"

#ifdef CORELIB_WIN32
GTS_Service* GTSAcceptor::make_handler()
{
	Player* p =GTSMODULE->get_emptyplayer();
	if( p == 0)
		return 0;

	return p->get_service();
}

#else

int GTSAcceptor::handle_input( ACE_HANDLE handle)
{
	Player* p =GTSMODULE->get_emptyplayer();
	if( p == 0)
		return 0;

	GTS_Service* s =p->get_service();

	if( acceptor_.accept( s->peer()) == -1)
	{
		GTSMODULE->release_player( p);

		MODULE_LOG_WARN( MODULE_BOOT, "GTSAcceptor::handle_input accept failed");
	}
	else if( s->open() != 0)
	{
		s->handle_close();
	}

	return 0;
}

#endif
