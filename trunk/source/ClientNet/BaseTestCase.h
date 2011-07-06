/**
* Client Net Module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __BASETESTCASE__H__
#define __BASETESTCASE__H__

#include "prolib/BasicProtocol.h"

#include <string>

/**
* @class	BaseTestCase
* @author	Created by Lu Jianfeng ( lujf1978@163.com ), 2008
* 
* @brief testcase基类
**/
class BaseTestCase
{
public:
	BaseTestCase();
	virtual ~BaseTestCase();

	/**
	* 获取具体case的注册用名字
	* @return 注册名字，在testcasedef.h里面定义
	**/
	virtual std::string get_name() =0;

	/**
	* testcase的主入口
	* @return
	**/
	virtual void deal_echo( const BasicProtocol *pb) =0;

	/**
	* testcase的主动功能实现
	* 
	**/
	virtual void case_tick(){};

	/**
	* 根据支持的类型创建测试用例
	*
	* @param casename
	* @return BaseTestCase*
	**/
	static BaseTestCase* new_testcase( std::string casename);

};

#endif	//__BASETESTCASE__H__