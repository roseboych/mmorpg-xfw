/**
* cts application
*
* @category		transcript manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __INSTCELLMAPGROUP__H__
#define __INSTCELLMAPGROUP__H__

#include "cts_pre.h"
#include "InstCellState.h"

#include <vector>

/**
* @class InstCellMapGroup
* 
* @brief 副本按地图分类
* 
**/
class InstCellMapGroup
{
public:
	typedef NS_STL::vector<InstCellState*>	INSTCELLSTATE_VECTOR;

	InstCellMapGroup( int mapid);
	~InstCellMapGroup();

	void regist_instcell( InstCellState* pcell);

	InstCellState* get_canentercell();

protected:
	void release();

private:
	//副本地图id
	int						mapid_;

	//关联的副本
	INSTCELLSTATE_VECTOR	cells_;
};

#endif	//__INSTCELLMAPGROUP__H__
