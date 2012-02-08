/**
* css application
*
* @category		cmsvr predefine
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CMSVR_PRE__H__
#define __CMSVR_PRE__H__

#include <prolib/Pro_define.h>

//坐标到pane格子的转化
#define POSX2PANEX(x)	( (x<0)?((int)x/MAP_PLANE_DIAMETER - 1):((int)x/MAP_PLANE_DIAMETER))
#define POSY2PANEY(y)	( (y<0)?((int)y/MAP_PLANE_DIAMETER - 1):((int)y/MAP_PLANE_DIAMETER))

/**
* pane的区域
**/
struct pane_region
{
	pane_region():x1(NO_INITVALUE),y1(NO_INITVALUE),x2(NO_INITVALUE),y2(NO_INITVALUE){}

	bool is_empty(){
		return x1 > x2 || y1 > y2;
	}

	int	x1;
	int	y1;
	int	x2;
	int	y2;
};

#endif	//__CMSVR_PRE__H__
