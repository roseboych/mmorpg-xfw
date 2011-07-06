/**
* corelib
*
* @category		iocp module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "corelib/net/Asynch_Read_Stream_Wrap.h"

#include "ace/Proactor.h"

Asynch_Read_Stream_Wrap::Asynch_Read_Stream_Wrap()
:inherit()
{
}

int Asynch_Read_Stream_Wrap::open( ACE_Handler &handler, ACE_HANDLE handle,
								   const void *completion_key, ACE_Proactor *proactor)
{
	// Get a proactor for/from the user.
	proactor = this->get_proactor (proactor, handler);

	// Now let us get the implementation initialized.
	if( this->implementation_ == 0)
	{
		if( (this->implementation_ = proactor->create_asynch_read_stream ()) == 0)
			return -1;
	}

	// Call the <open> method of the base class.
	return ACE_Asynch_Operation::open( handler, handle, completion_key, proactor);
}
