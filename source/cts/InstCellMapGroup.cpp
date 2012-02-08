/**
* cts application
*
* @category		transcript manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "InstCellMapGroup.h"

InstCellMapGroup::InstCellMapGroup( int mapid):
mapid_( mapid)
{

}

InstCellMapGroup::~InstCellMapGroup()
{
	release();
}

void InstCellMapGroup::release()
{
	cells_.clear();
}

void InstCellMapGroup::regist_instcell( InstCellState* pcell)
{
	cells_.push_back( pcell);
}

InstCellState* InstCellMapGroup::get_canentercell()
{
	for( size_t ii =0; ii < cells_.size(); ++ii)
	{
		if( cells_[ii]->can_entercell())
			return cells_[ii];
	}

	return 0;
}
