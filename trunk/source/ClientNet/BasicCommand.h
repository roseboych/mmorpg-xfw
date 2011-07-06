/**
* Client Net Module
* 
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __BASICCOMMAND__H__
#define __BASICCOMMAND__H__

/**
* @class	TBasicCommand
* 
* @brief 系统命令队列基类
**/
class BasicCommand
{
public:
	virtual ~BasicCommand(){}

	//执行命令
	virtual void run() =0;
};

#endif	//__BASICCOMMAND__H__
