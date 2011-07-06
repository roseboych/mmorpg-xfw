/**
* cts application
*
* @category		transcript manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "InstCellState.h"

InstCellState::InstCellState():
state_( TRANSCRIPT_SVRST_UNREGIST),
opt_( 0),
css_server_( 0)
{
}

void InstCellState::reset()
{
	state_ =TRANSCRIPT_SVRST_UNREGIST;
}
