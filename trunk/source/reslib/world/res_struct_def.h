/**
* reslib
*
* @category		world config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __RES_DEF__H__
#define __RES_DEF__H__

#include <prolib/core_type.h>

//出生点结构定义
struct world_bornpos
{
	S_FLOAT_32	posx_;
	S_FLOAT_32	posy_;
	S_FLOAT_32	posz_;
	S_FLOAT_32	face_;
};

//坐标
struct coordinate_pointer
{
	//坐标值
	S_FLOAT_32	x_;
	S_FLOAT_32	y_;
	S_FLOAT_32	z_;

	coordinate_pointer& reset( S_FLOAT_32 _x, S_FLOAT_32 _y, S_FLOAT_32 _z)
	{
		x_ = _x;
		y_ = _y;
		z_ = _z;

		return *this;
	}
};

// 转跳点信息
struct teleport_info
{
	S_INT_32			id_;
	coordinate_pointer	pos_;
	S_FLOAT_32			face_;
	//所属的地图
	S_INT_32			ownermapid_;
};

//副本转跳点信息
struct transcript_teleport_info
{
	S_INT_32			id_;
	//主地图信息
	S_INT_32			mainmap_id_;
	coordinate_pointer	mainmap_pos_;
	//副本地图信息
	S_INT_32			instmap_id_;
	coordinate_pointer	instmap_pos_;
};

#endif	//__RES_DEF__H__
