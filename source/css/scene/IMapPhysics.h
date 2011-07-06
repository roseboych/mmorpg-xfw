/**
* css application
*
* @category		physics
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __IMAPPHYSICS__H__
#define __IMAPPHYSICS__H__

#include "../actor/UnitBase.h"
#include "../resource/MapResInfo.h"

/**
* @class IMapPhysics
* 
* @brief 地图物理系统接口
**/
class IMapPhysics
{
public:
	IMapPhysics();

public:
	/**
	* 物理系统初始化
	* @param minfo	地图配置信息
	**/
	virtual bool init_physics( MapResInfo* minfo);

	/**
	* 物理系统释放资源
	**/
	virtual void uninit_physics() =0;

	/**
	* 移动合法性检测
	* @param startpos	移动开始点
	* @param targetpos	移动终点
	* @return 
	**/
	virtual bool move_check( const coordinate_pointer& startpos, const coordinate_pointer& targetpos) =0;
	virtual bool move_check( S_FLOAT_32 ox, S_FLOAT_32 oy, S_FLOAT_32 oz, S_FLOAT_32 x, S_FLOAT_32 y, S_FLOAT_32 z) =0;

	/**
	* 测试坐标点的合法性
	* @param x
	* @param y
	* @param z
	* @return
	**/
	bool test_position_validate( S_FLOAT_32 x, S_FLOAT_32 y, S_FLOAT_32 z);
	bool test_position_validate( const coordinate_pointer& pos);

	MapResInfo* get_mapopt(){ return res_opt_;}

protected:
	//地图配置
	MapResInfo*	res_opt_;
};

#endif	//__IMAPPHYSICS__H__
