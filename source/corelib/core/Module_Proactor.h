/**
* corelib
*
* @category		core module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __MODULE_PROACTOR__H__
#define __MODULE_PROACTOR__H__

#if defined (_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable: 4267)
#endif /* _MSC_VER */

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include "corelib/corelibdef.h"

#include "ace/Task.h"
#include "ace/Auto_Ptr.h"

#ifdef CORELIB_WIN32
	#include "ace/Proactor.h"
#else
	#include "ace/config.h"
	#include "ace/Reactor.h"
#endif

/**
* @class Module_Proactor
*
* @brief 封装了Proactor
*
**/
class Module_Proactor : public ACE_Task_Base
{
public:
	Module_Proactor(void);
	virtual ~Module_Proactor(void);

public:
	virtual int svc (void);

	//initialized
	int initialize();

	//开始iocp
	void start();

	//结束iocp
	void stop();

	//根据cpu数量这是合适的线程数
	void set_thread_num( int n){
		thread_nums_ =n*2+2;
	}

#ifdef CORELIB_WIN32
	operator ACE_Proactor* (){
		return proactor_.get();
	}

#else
	operator ACE_Reactor* (){
		return reactor_.get();
	}

	void set_maxiohandle( int m){
		max_iohandle_ =(m<=0?5:m+5);
	}

#endif

protected:
	int	thread_nums_;

#ifdef CORELIB_WIN32
	ACE_Auto_Ptr<ACE_Proactor>	proactor_;

#else
	//能够处理的最大i/o句柄数
	int	max_iohandle_;
	//reactor单独使用，不加入singleton实例中
	ACE_Auto_Ptr<ACE_Reactor>	reactor_;

#endif
};


#endif	//__MODULE_PROACTOR__H__
