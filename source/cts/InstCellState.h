/**
* cts application
*
* @category		transcript manage
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __INSTCELLSTATE__H__
#define __INSTCELLSTATE__H__

#include "cts_pre.h"
#include <reslib/server/service_define.h>

#include <reslib/deploy/INSCellOption.h>
#include "CSSLink.h"

/**
* @class InstCellState
* 
* @brief 副本状态
* 
**/
class InstCellState
{
public:
	InstCellState();

	void reset();

	void set_state( TRANSCRIPT_SERVER_STATE v){ state_ =v;}
	TRANSCRIPT_SERVER_STATE get_state(){ return state_;}

	INSCellOption*	get_option(){ return opt_;}
	void			set_option( INSCellOption* v){ opt_ =v;}

	CSSLink*	get_csslink(){ return css_server_;}
	void		set_csslink( CSSLink* csv){ css_server_ =csv;}

	int			get_mapid(){ return opt_->get_mapid();}
	int			get_cellid(){ return opt_->get_cellid();}

	bool can_entercell(){ return state_ == TRANSCRIPT_SVRST_CANENTER;}

private:
	//当前状态
	TRANSCRIPT_SERVER_STATE	state_;

	//关联的css服务器
	CSSLink*		css_server_;
	//相关配置
	INSCellOption*	opt_;
};

#endif	//__INSTCELLSTATE__H__
