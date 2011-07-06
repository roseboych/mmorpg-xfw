/**
* css application
*
* @category		cmsvr predefine
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CMSVR_PRE__H__
#define __CMSVR_PRE__H__

#include <prolib/core_type.h>
#include <prolib/datastore/world_define.h>

//坐标到pane格子的转化
#define POSX2PANEX(x)	( (x<0)?((S_INT_32)x/MAP_PLANE_DIAMETER - 1):((S_INT_32)x/MAP_PLANE_DIAMETER))
#define POSY2PANEY(y)	( (y<0)?((S_INT_32)y/MAP_PLANE_DIAMETER - 1):((S_INT_32)y/MAP_PLANE_DIAMETER))

/**
* pane的区域
**/
struct pane_region
{
	pane_region():x1(NO_INITVALUE),y1(NO_INITVALUE),x2(NO_INITVALUE),y2(NO_INITVALUE){}

	bool is_empty(){
		return x1 > x2 || y1 > y2;
	}

	S_INT_32	x1;
	S_INT_32	y1;
	S_INT_32	x2;
	S_INT_32	y2;
};

#endif	//__CMSVR_PRE__H__
