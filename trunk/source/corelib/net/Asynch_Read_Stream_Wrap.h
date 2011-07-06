/**
* corelib
*
* @category		iocp module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __ASYNCH_READ_STREAM_WRAP__H__
#define __ASYNCH_READ_STREAM_WRAP__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "ace/Asynch_IO.h"

/**
* @class Asynch_Read_Stream_Wrap
* 
* @brief ace异步读取的封装类
**/
class Asynch_Read_Stream_Wrap : public ACE_Asynch_Read_Stream
{
	typedef ACE_Asynch_Read_Stream inherit;
public:
	Asynch_Read_Stream_Wrap();

	virtual ~Asynch_Read_Stream_Wrap(){};

	virtual int open (ACE_Handler &handler, ACE_HANDLE handle = ACE_INVALID_HANDLE,
		const void *completion_key = 0, ACE_Proactor *proactor = 0);

};

#endif	//__ASYNCH_READ_STREAM_WRAP__H__
