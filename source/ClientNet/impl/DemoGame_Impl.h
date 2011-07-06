/**
* Client Net Module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __DEMOGAME_IMPL__H__
#define __DEMOGAME_IMPL__H__

#include "../BaseTestCase.h"

/**
* @class	DemoGame_Impl
* 
* @brief demo
**/
class DemoGame_Impl : public BaseTestCase
{
public:
	DemoGame_Impl(void);
	~DemoGame_Impl(void);

public:
	virtual std::string get_name();
	virtual void deal_echo( const BasicProtocol *pb);
	virtual void case_tick();

public:
	std::string username_;
};

#endif	//__DEMOGAME_IMPL__H__
