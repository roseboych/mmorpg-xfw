/**
* Client Net Module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ) ( lujf1978@163.com ), 2009
*/
#include "BaseTestCase.h"

#include "testcasedef.h"
#include "impl/DemoGame_Impl.h"

BaseTestCase::BaseTestCase()
{
}

BaseTestCase::~BaseTestCase()
{
}

BaseTestCase* BaseTestCase::new_testcase( std::string casename)
{
	if( casename == TESTCASE_DEMO)
		return new DemoGame_Impl();
	else
		return 0;
}

