/**
* rgs application
* 
* @category		rgs update
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __SVRTEAMINFO__H__
#define __SVRTEAMINFO__H__

#include <prolib/core_type.h>
#include <prolib/Pro_all.h>
#include <ace/Auto_Ptr.h>

/**
* @class SvrTeamInfo
*
* @brief 服务器组的信息维护
*
**/
class SvrTeamInfo
{
public:
	struct teaminfo
	{
		int		server_index_;
		int		teamid_;
		int		max_user_;
		int		cur_user_;
		//0:不可用 1:可用
		char	is_validate_;
	};

public:
	SvrTeamInfo();

	/**
	* 初始化服务器组信息
	**/
	void init_svrteaminfo();

	/**
	* 更新指定信息
	* @param info
	**/
	void update_teaminfo( PRO::Pro_SvrTeamInfo_NTF* info);

	/**
	* 构造一个协议
	**/
	PRO::Pro_SvrTeamInfo_NTF* construct_notify();

protected:
	inline teaminfo* get_teaminfo( int ind);

protected:
	//组信息
	ACE_Auto_Array_Ptr<teaminfo>	teams_;
	int								team_nums_;
};

inline 
SvrTeamInfo::teaminfo* SvrTeamInfo::get_teaminfo( int ind)
{
	SvrTeamInfo::teaminfo* ret =0;
	if( ind < 0 || ind >= team_nums_)
		return ret;

	return &( teams_[ind]);
}

#endif	//__SVRTEAMINFO__H__
