/**
* rgs application
*
* @category		db action
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __DBACTION_DEF__H__
#define __DBACTION_DEF__H__

//存储过程定义
#define DBPROC_USERLOGIN	"{call getuserinfo(:uname<char[20],in>)}"


//定义的action类型
#define DBACTION_LOGIN	1

#endif	//__DBACTION_DEF__H__
