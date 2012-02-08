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
	float	posx_;
	float	posy_;
	float	posz_;
	float	face_;
};

//坐标
struct coordinate_pointer
{
	//坐标值
	float	x_;
	float	y_;
	float	z_;

	coordinate_pointer& reset( float _x, float _y, float _z)
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
	int					id_;
	coordinate_pointer	pos_;
	float				face_;
	//所属的地图
	int					ownermapid_;
};

//副本转跳点信息
struct transcript_teleport_info
{
	int					id_;
	//主地图信息
	int					mainmap_id_;
	coordinate_pointer	mainmap_pos_;
	//副本地图信息
	int					instmap_id_;
	coordinate_pointer	instmap_pos_;
};

#endif	//__RES_DEF__H__
