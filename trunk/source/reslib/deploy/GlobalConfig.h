/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __GLOBALCONFIG__H__
#define __GLOBALCONFIG__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>

#include <map>
#include <list>
#include <string>
#include <vector>

#include <reslib/deploy/DBConfig.h>
#include <reslib/deploy/RGSOption.h>

class IConfigContentSource;
class SvrTeamOption;
class LGSOption;
class TiXmlElement;
class CSSOption;

/**
* @class Asynch_Read_Stream_Wrap
* 
* @brief ace异步读取的封装类
**/
class GlobalConfig
{
	typedef NS_STL::map< int, LGSOption*>	LGSOPTION_MAP;
	typedef NS_STL::map< int, SvrTeamOption*>	SVRTEAMOPTION_MAP;

public:
	GlobalConfig();
	~GlobalConfig();

public:

	/**
	* 初始化配置管理器
	* @return
	**/
	bool init_config();

	/**
	* 获取配置的数据源
	* @return
	**/
	IConfigContentSource* get_confsrc(){ return conf_src_;}

	/**
	* 根据sn获取lgs配置信息
	* @param sn
	*
	* @return LGSOption*
	**/
	LGSOption* get_lgsoption( int sn);

	/**
	* 获取所有的lgs配置信息
	* @param ret
	**/
	void get_lgsoptions( NS_STL::list<LGSOption*>& ret);

	/**
	* 获取lgs的数量
	**/
	int get_lgsnums();

	RGSOption* get_rgsoption(){ return &rgsopt_;}

	/**
	* 根据sn获取svrteamoption配置信息
	* @param sn
	*
	* @return SvrTeamOption*
	**/
	SvrTeamOption* get_svrteamoption( int sn);

	/**
	* 获取所有的svrteam配置信息
	* @param ret
	**/
	void get_svrteamoptions( NS_STL::list<SvrTeamOption*>& ret);

	/**
	* 获取svrteam的数量
	**/
	int get_svrteamnums();

	int get_regionsn(){ return regionsn_;}
	int get_appsn(){ return appsn_;}
	int get_cpunum(){ return cpunum_;}
	int get_loopnum(){ return loopnum_;}

	CSSOption* get_cssoptbymapid( int mid);

protected:
	/**
	* 加载大区配置
	* @return 
	**/
	bool load_regionoption();
	/**
	* 加载运行时配置文件
	**/
	bool load_svrno();
	/**
	* 加载服务器组配置
	**/
	bool load_svrteamoption();

	/**
	* 加载数据库配置
	**/
	bool load_dbconfig( TiXmlElement* e, DBConfig& db);

	/**
	* 完成数据验证和统计
	* @return
	**/
	bool check_validate();

	/**
	* 为各个配置分配索引等
	**/
	void build_config();

	/**
	* 加载指定的服务器组配置
	* @param opt
	*
	* @return
	**/
	bool load_specialsvrteam( SvrTeamOption* opt);

	void release();

private:
	//当前应用的region和id
	int	regionsn_;
	int appsn_;
	int cpunum_;
	int loopnum_;
	//配置文件数据源配置
	NS_STL::map< NS_STL::string, NS_STL::string> cnfprop_;

	//logs配置信息
	LGSOPTION_MAP		lgss_;
	RGSOption			rgsopt_;
	SVRTEAMOPTION_MAP	svrteams_;
	DBConfig			dbconf_;

	//配置内容数据源
	IConfigContentSource*	conf_src_;
};

#define GLOBALCONFIG_INS ACE_Singleton< GlobalConfig, ACE_Null_Mutex>::instance()

#endif	//__GLOBALCONFIG__H__
