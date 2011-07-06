/**
* reslib
*
* @category		deploy config module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "reslib/deploy/GlobalConfig.h"

#include <corelib/corelibdef.h>
#include <corelib/mconf/IConfigContentSource.h>
#include <corelib/xml/XmlUtil.h>
#include <corelib/util/ShareUtil.h>
#include <corelib/util/FileUtil.h>

#include "reslib/FileConfigSourceImpl.h"
#include "reslib/deploy/LGSOption.h"
#include "reslib/deploy/RGSOption.h"
#include "reslib/deploy/SvrTeamOption.h"
#include "reslib/deploy/CSSOption.h"

GlobalConfig::GlobalConfig():conf_src_( 0)
{
	//可以修改配置数据源实现
	conf_src_ =STATICRES_NEW FileConfigSourceImpl();
}

GlobalConfig::~GlobalConfig(void)
{
	release();

	if( conf_src_)
	{
		conf_src_->uninit_source();
		delete conf_src_;
	}

	conf_src_ =0;
}

void GlobalConfig::release()
{
	SVRTEAMOPTION_MAP::iterator eiter3 =svrteams_.end();
	for( SVRTEAMOPTION_MAP::iterator iter =svrteams_.begin(); iter != eiter3; ++iter)
		delete iter->second;
	svrteams_.clear();

	LGSOPTION_MAP::iterator eiter =lgss_.end();
	for( LGSOPTION_MAP::iterator iter =lgss_.begin(); iter != eiter; ++iter)
		delete iter->second;
	lgss_.clear();
}

bool GlobalConfig::init_config()
{
	if( !load_svrno())
		return false;

	if( !conf_src_->init_source( cnfprop_))
		return false;

	//加载系统配置文件
	if( !load_regionoption())
		return false;

	if( !load_svrteamoption())
		return false;

	if( !check_validate())
		return false;

	build_config();

	return true;
}

bool GlobalConfig::load_svrno()
{
	NS_STL::string cnt =FileUtil::get_filecontent( ".", "svrno");
	if( cnt.size() == 0)
		return false;

	TiXmlDocument doc;
	if( !XmlUtil::loadfromstring( (char*)cnt.c_str(), doc))
		return false;

	TiXmlElement* root =doc.RootElement();
	
	regionsn_ =XmlUtil::GetXmlAttrInt( root, "region", 0);
	if( regionsn_ == 0)
		return false;
	appsn_ =XmlUtil::GetXmlAttrInt( root, "sn", 0);
	if( appsn_ == 0)
		return false;

	cpunum_ =XmlUtil::GetXmlAttrInt( root, "cpu", 1);
	loopnum_ =XmlUtil::GetXmlAttrInt( root, "loopnum", 100);

	//获取配置文件根目录
	NS_STL::string srct =XmlUtil::GetXmlAttrStr( root, "confsrc", "filesrc");
	if( srct == "filesrc")
	{
		TiXmlElement* xcnf =root->FirstChildElement( "filesrc");
		if( xcnf == 0)
			return false;

		NS_STL::string v =XmlUtil::GetXmlAttrStr( xcnf, "datadir", "./datas");
		cnfprop_["dirbase"] =v;
	}
	else
		return false;

	return true;
}

bool GlobalConfig::load_regionoption()
{
	NS_STL::string cnt =conf_src_->get_txtfilecontent( "system", "regionsvr.xml");

	TiXmlDocument doc;
	if( !XmlUtil::loadfromstring( (char*)cnt.c_str(), doc))
		return false;

	TiXmlElement* root =doc.RootElement();
	
	TiXmlElement* db =root->FirstChildElement( "userdb");
	if( db == 0)
		return false;

	//加载数据库配置
	if( !this->load_dbconfig( db, dbconf_))
		return false;

	//lgss
	TiXmlElement* lgss =root->FirstChildElement( "lgss");
	if( lgss == 0)
		return false;
	
	for( TiXmlElement* e =lgss->FirstChildElement( "lgs"); e != NULL; e =e->NextSiblingElement( "lgs"))
	{
		int sn =XmlUtil::GetXmlAttrInt( e, "sn", 0);
		if( sn == 0)
			return false;
		if( this->get_lgsoption( sn))
			return false;

		LGSOption* opt =STATICRES_NEW LGSOption();
		opt->sn_ =sn;
		opt->ip_ =XmlUtil::GetXmlAttrStr( e, "ip");
		opt->port_ =XmlUtil::GetXmlAttrInt( e, "port");
		opt->maxuser_ =XmlUtil::GetXmlAttrInt( e, "maxuser");

		lgss_[sn] =opt;
	}

	//rgs
	TiXmlElement* rgs =root->FirstChildElement( "rgs");
	if( rgs == 0)
		return false;

	rgsopt_.dbinfo_ =dbconf_;
	rgsopt_.ip_ =XmlUtil::GetXmlAttrStr( rgs, "ip");
	rgsopt_.port_ =XmlUtil::GetXmlAttrInt( rgs, "port");
	rgsopt_.controller_port_ =XmlUtil::GetXmlAttrInt( rgs, "controllerport", 1200);

	TiXmlElement* controllers =rgs->FirstChildElement( "controllers");
	if( controllers)
	{
		for( TiXmlElement* e =controllers->FirstChildElement( "user"); e != NULL; e =e->NextSiblingElement( "user"))
		{
			NS_STL::string name =XmlUtil::GetXmlAttrStr( e, "name", "");
			NS_STL::string pwd =XmlUtil::GetXmlAttrStr( e, "password", "");

			if( name == "")
				continue;

			rgsopt_.controller_users_[name] =pwd;
		}
	}

	//加载服务器组信息
	for( TiXmlElement* e =root->FirstChildElement( "svrteam"); e != NULL; e =e->NextSiblingElement( "svrteam"))
	{
		int sn =XmlUtil::GetXmlAttrInt( e, "sn", 0);
		if( sn == 0)
			return false;
		if( this->get_svrteamoption( sn))
			return false;

		SvrTeamOption* opt =STATICRES_NEW SvrTeamOption();
		opt->sn_ =sn;
		opt->maxuser_ =XmlUtil::GetXmlAttrInt( e, "maxuser");

		svrteams_[sn] =opt;
	}

	return true;
}

bool GlobalConfig::load_svrteamoption()
{
	SVRTEAMOPTION_MAP::iterator eiter =svrteams_.end();
	for( SVRTEAMOPTION_MAP::iterator iter =svrteams_.begin(); iter != eiter; ++iter)
	{
		SvrTeamOption* opt =iter->second;

		if( !load_specialsvrteam( opt))
			return false;
	}

	return true;
}

bool GlobalConfig::load_specialsvrteam( SvrTeamOption* opt)
{
	//加载服务器组的详细信息
	NS_STL::string svrconf =ShareUtil::combineintstr( opt->sn_, ".svrteam");

	NS_STL::string cnt =conf_src_->get_txtfilecontent( "system", svrconf.c_str());
	TiXmlDocument doc;
	if( !XmlUtil::loadfromstring( (char*)cnt.c_str(), doc))
		return false;

	TiXmlElement* root =doc.RootElement();

	//cts
	TiXmlElement* cts =root->FirstChildElement( "cts");
	if( cts == 0)
		return false;

	opt->ctsopt_.ip_ =XmlUtil::GetXmlAttrStr( cts, "ip");
	opt->ctsopt_.port_ =XmlUtil::GetXmlAttrInt( cts, "port");

	//gts
	TiXmlElement* gts =root->FirstChildElement( "gts");
	if( gts == 0)
		return false;

	for( TiXmlElement* e=gts->FirstChildElement("gateway"); e != NULL; e =e->NextSiblingElement( "gateway"))
	{
		int sn =XmlUtil::GetXmlAttrInt( e, "sn", 0);
		if( sn == 0)
			return false;
		if( opt->get_gtsoption( sn))
			return false;

		GTSOption* g =STATICRES_NEW GTSOption();
		g->sn_ =sn;
		g->ip_ =XmlUtil::GetXmlAttrStr( e, "ip");
		g->port_ =XmlUtil::GetXmlAttrInt( e, "port");
		g->startuser_ =XmlUtil::GetXmlAttrInt( e, "startuser");
		g->usernum_ =XmlUtil::GetXmlAttrInt( e, "usernum");

		opt->gtss_[sn] =g;
	}

	//dpx
	TiXmlElement* dpx =root->FirstChildElement( "dpx");
	if( dpx == 0)
		return false;

	opt->dpxopt_.ip_ =XmlUtil::GetXmlAttrStr( dpx, "ip");
	opt->dpxopt_.port_ =XmlUtil::GetXmlAttrInt( dpx, "port");
	opt->dpxopt_.maxuser_ =opt->maxuser_;

	//dpx db
	TiXmlElement* dpxdb =dpx->FirstChildElement( "gamedb");
	if( dpxdb == 0)
		return false;
	this->load_dbconfig( dpxdb, opt->dpxopt_.db_);

	//csss
	TiXmlElement* csss =root->FirstChildElement("css");
	if( csss == 0)
		return false;

	for( TiXmlElement* e =csss->FirstChildElement("cs"); e != NULL; e =e->NextSiblingElement( "cs"))
	{
		int sn =XmlUtil::GetXmlAttrInt( e, "sn", 0);
		if( sn == 0)
			return false;
		if( opt->get_cssoption( sn))
			return false;

		CSSOption* c =STATICRES_NEW CSSOption();
		c->sn_ =sn;
		c->ip_ =XmlUtil::GetXmlAttrStr( e, "ip");
		c->port_ =XmlUtil::GetXmlAttrInt( e, "port");
		c->svrteamopt_ =opt;

		//支持的地图
		CODE_BLOCK_BEGIN()
		NS_STL::string m2 =XmlUtil::GetXmlAttrStr( e, "map");
		NS_STL::vector<int> maps;
		if( !ShareUtil::splitstr2int( m2.c_str(), ",", maps))
			return false;
		for( size_t ii =0; ii < maps.size(); ++ii)
			c->mapids_.push_back( maps[ii]);

		CODE_BLOCK_END()

		//load ins config
		TiXmlElement* inss =e->FirstChildElement("inss");
		if( inss)
		{
			for( TiXmlElement* e2 =inss->FirstChildElement("ins"); e2 != NULL; e2 =e2->NextSiblingElement( "ins"))
			{
				int sn2 =XmlUtil::GetXmlAttrInt( e2, "sn", 0);
				if( sn2 == 0)
					return false;
				if( c->get_insoption( sn2))
					return false;

				INSOption* c2 =STATICRES_NEW INSOption();
				c->inss_[sn2] =c2;

				c2->sn_ =sn2;
				c2->cssopt_ =c;

				c2->nums_ =XmlUtil::GetXmlAttrInt( e2, "nums", 0);
				if( c2->nums_ <= 0)
					return false;
			}
		}

		opt->csss_[sn] =c;
	}

	return true;
}

bool GlobalConfig::load_dbconfig( TiXmlElement* e, DBConfig& db)
{
	if( e == 0)
		return false;

	db.dbname_ =XmlUtil::GetXmlAttrStr( e, "dbname", "game");
	db.dbuser_ =XmlUtil::GetXmlAttrStr( e, "user");
	db.dbpwd_ =XmlUtil::GetXmlAttrStr( e, "pwd");
	db.dbthreadnum_ =XmlUtil::GetXmlAttrInt( e, "threadnum", 4);
	db.dbloopstep_ =XmlUtil::GetXmlAttrInt( e, "loopstep", 20);

	return true;
}

bool GlobalConfig::check_validate()
{
	for( SVRTEAMOPTION_MAP::iterator iter =svrteams_.begin(); iter != svrteams_.end(); ++iter)
	{
		SvrTeamOption* opt =iter->second;
		if( !opt->check_validate())
			return false;
	}

	return true;
}

LGSOption* GlobalConfig::get_lgsoption( int sn)
{
	LGSOPTION_MAP::iterator fiter =lgss_.find( sn);
	if( fiter == lgss_.end())
		return 0;

	return fiter->second;
}

void GlobalConfig::get_lgsoptions( NS_STL::list<LGSOption*>& ret)
{
	LGSOPTION_MAP::iterator iter, eiter =lgss_.end();
	for( iter =lgss_.begin(); iter != eiter; ++iter)
		ret.push_back( iter->second);
}

int GlobalConfig::get_lgsnums()
{
	return (int)lgss_.size();
}

SvrTeamOption* GlobalConfig::get_svrteamoption( int sn)
{
	SVRTEAMOPTION_MAP::iterator fiter =svrteams_.find( sn);
	if( fiter == svrteams_.end())
		return 0;

	return fiter->second;
}

void GlobalConfig::get_svrteamoptions( NS_STL::list<SvrTeamOption*>& ret)
{
	SVRTEAMOPTION_MAP::iterator iter, eiter =svrteams_.end();
	for( iter =svrteams_.begin(); iter != eiter; ++iter)
		ret.push_back( iter->second);
}

int GlobalConfig::get_svrteamnums()
{
	return (int)svrteams_.size();
}

#include <algorithm>

template< class T>
static bool normal_sort( const T* l, const T* r)
{
	return l->sn_ < r->sn_;
}

void GlobalConfig::build_config()
{
	//lgs
	NS_STL::list<LGSOption*> lgs;
	this->get_lgsoptions( lgs);
	lgs.sort( normal_sort<LGSOption>);

	int index =0;
	for( NS_STL::list<LGSOption*>::iterator iter =lgs.begin(); iter != lgs.end(); ++iter, ++index)
		(*iter)->server_index_ =index;
	lgs.clear();

	//svrteam
	NS_STL::list<SvrTeamOption*> sts;
	this->get_svrteamoptions( sts);
	sts.sort( normal_sort<SvrTeamOption>);

	index =0;
	//统计大区玩家数
	int all_user =0;
	for( NS_STL::list<SvrTeamOption*>::iterator iter =sts.begin(); iter != sts.end(); ++iter, ++index)
	{
		(*iter)->server_index_ =index;
		(*iter)->build_config();
		all_user += (*iter)->maxuser_;
	}
	sts.clear();

	//rgs
	rgsopt_.max_users_ =all_user;
}

CSSOption* GlobalConfig::get_cssoptbymapid( S_INT_32 mid)
{
	CSSOption* ret =0;
	SVRTEAMOPTION_MAP::iterator iter =svrteams_.begin(), eiter =svrteams_.end();
	for( ; iter != eiter; ++iter)
	{
		SvrTeamOption* st =iter->second;
		ret =st->get_cssoptbymapid( mid);
		if( ret)
			return ret;
	}

	return ret;
}