/**
* css application
*
* @category		buffer system
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __MYBUFFERCONTAINER__H__
#define __MYBUFFERCONTAINER__H__

#include "../../css_pre.h"
#include <prolib/Pro_all.h>
#include <corelib/corelibdef.h>

#include "BufferRuntime.h"

#include <map>

class Player;

NAMESPACE_APPSCRIPT_BEGIN

class ScriptContext;

NAMESPACE_APPSCRIPT_END

/**
* @class MyBufferContainer
* 
* @brief buffer管理
**/
class MyBufferContainer
{
	typedef NS_STL::map< S_INT_32, BufferRuntime>	BUFFRUNTIME_MAP;
public:
	MyBufferContainer();
	~MyBufferContainer();

	void set_owner( Player* owner);
	Player* get_owner(){ return owner_;}

	/**
	* 初始化玩家的buffer信息
	* @param pack
	**/
	void init_baseinfo( PRO::Pro_ChrLoad_ack* pack);

	void reset();

	void tick( const ACE_UINT64& tnow);

	/**
	* 计算buffer对属性的影响
	* @param context
	**/
	void affect_playerprops( app::script::ScriptContext& context);

	/**
	* 附加一个新的buffer
	* @param buffcode
	* @return
	**/
	BufferRuntime* attach_newbuffer( S_INT_32 buffcode);

	/**
	* 消除一个buffer
	* @param buffcode
	**/
	void cancel_onebuffer( S_INT_32 buffcode);

protected:
	/**
	* 根据buffer编号获取buffer信息
	* @param buffcode
	**/
	BufferRuntime* get_bufferbyid( S_INT_32 buffcode);

	/**
	* 通知buffer变化
	* @param type 0:增加 1:删除 2:修改
	**/
	void notify_bufferchange( BufferRuntime& buff, S_INT_8 type);

protected:
	//所有运行时buffer
	BUFFRUNTIME_MAP	buff_runtimes_;

	Player*	owner_;
};

#endif	//__MYBUFFERCONTAINER__H__
