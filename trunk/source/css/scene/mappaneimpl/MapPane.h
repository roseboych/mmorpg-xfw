/**
* css application
*
* @category		cmsvr module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __MAPPANE__H__
#define __MAPPANE__H__

#include "../../css_pre.h"

#include <prolib/BasicProtocol.h>

#include "../../character/Player.h"
#include "../SceneSectorProperty.h"

class SceneManagePaneImpl;

/**
* @class MapPane
* 
* @brief 地图块信息
**/
class MapPane
{
	friend class SceneManagePaneImpl;
public:
	typedef NS_STL::map<S_INT_32, UnitBase*>	UNITOBJ_MAP;

public:
	MapPane();

	/**
	* 复位格子信息
	**/
	void reset();

	/**
	* 注册活动对象
	* @param pobj
	**/
	void regist_unitobj( UnitBase* pobj);
	void unregist_unitobj( UnitBase* pobj);

	/**
	* in,out活动对象填充
	* @param punit
	* @param bin	true:计算进入视野的 false:计算移出视野的
	**/
	void unit_inout_fill( UnitBase* punit, bool bin);

	/**
	* 广播协议
	* @param sender
	* @param p
	* @param includeme	是否发送给自己
	* @param freepro	传入的p是否能够被释放
	**/
	void broadcast( UnitBase* sender, BasicProtocol* p, bool includeme =false, bool freepro =false);

public:
	//------------------------sector property wrapper----------------------------//

protected:
	//包含的玩家信息
	UNITOBJ_MAP	players_;

	//对应的地形属性
	SceneSectorProperty		sector_prop_;

	//pane坐标
	S_INT_32 pane_x_;
	S_INT_32 pane_y_;
};

#endif	//__MAPPANE__H__
