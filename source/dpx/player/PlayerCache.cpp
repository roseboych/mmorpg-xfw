/**
* dpx application
*
* @category		player
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "PlayerCache.h"

PlayerCache::PlayerCache()
{
}

PlayerCache::~PlayerCache()
{
}

void PlayerCache::clear_chrlist()
{
	chrs_init_ =false;

	for( size_t ii =0; ii < chrs_.size(); ++ii)
		delete chrs_[ii];
	chrs_.clear();
}

ChrListInfo* PlayerCache::is_chrinlist( int cid)
{
	if( !chrs_init_)
		return 0;

	for( size_t ii =0; ii < chrs_.size(); ++ii)
	{
		if( chrs_[ii]->chrid_ == cid)
			return chrs_[ii];
	}

	return 0;
}

void PlayerCache::rm_chrinlist( int cid)
{
	for( CHRLIST_VECTOR::iterator iter =chrs_.begin(); iter != chrs_.end(); ++iter)
	{
		ChrListInfo* p =(*iter);
		if( p->chrid_ == cid)
		{
			chrs_.erase( iter);
			return;
		}
	}
}

void PlayerCache::chrdata_load( int cid)
{
	//导入数据
	for( size_t ii =0; ii < chrs_.size(); ++ii)
	{
		if( chrs_[ii]->chrid_ == cid)
			break;
	}
	clear_chrlist();

	is_chrload_ =true;
}

void PlayerCache::clear_itemlist()
{
	for( EQUIPITEMS_MAP::iterator iter =items_.begin(); iter != items_.end(); ++iter)
		delete iter->second;
	items_.clear();
}

void PlayerCache::clear_petlist()
{
	for( PETDATA_MAP::iterator iter =pets_.begin(); iter != pets_.end(); ++iter)
		delete iter->second;
	pets_.clear();
}

void PlayerCache::clear_bufferlist()
{
	for( BUFFERDATA_MAP::iterator iter =buffers_.begin(); iter != buffers_.end(); ++iter)
		delete iter->second;
	buffers_.clear();
}