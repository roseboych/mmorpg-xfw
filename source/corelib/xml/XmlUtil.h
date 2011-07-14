/**
* corelib
*
* @category		xml module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __XMLUTIL__H__
#define __XMLUTIL__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <prolib/core_type.h>
#include "corelib/xml/tinyxml.h"

/**
* @class	XmlUtil
* @author	Created by Lu Jianfeng ( lujf1978@163.com ), 2009
* 
* @brief xml文件处理类
**/
class XmlUtil
{
private:
	XmlUtil(){};
	~XmlUtil(){};

public:
	static bool loadfromfile(char * sfile, TiXmlDocument& doc);
	static bool loadfromstring(char *contents, TiXmlDocument& doc);

	/**
	* 获取xml节点属性
	* @param e		节点
	* @param attr	属性名称
	* @param defv	缺省值
	* @return 
	**/
	static bool GetXmlAttrBool( TiXmlElement* e, const char* attr, bool defv=false);
	static bool GetXmlAttrYesNo( TiXmlElement* e, const char* attr, bool defv=false);
	static int GetXmlAttrInt( TiXmlElement* e, const char* attr, int defv =0);
	static float GetXmlAttrFloat( TiXmlElement* e, const char* attr, float defv =0.f);
	static NS_STL::string GetXmlAttrStr( TiXmlElement*e, const char* attr, const char* defv="");

	/**
	* 获取xml节点属性
	* @param e		节点
	* @param attr	属性名称
	* @param defv	缺省值
	* @param ntruev	true值的int定义
	* @return bool
	**/
	static bool GetXmlAttrBoolFromInt( TiXmlElement* e, const char* attr, bool defv=false, int ntruev =0);

	static NS_STL::string GetXmlText( TiXmlElement* e, const char* defv="");
};

#endif	//__XMLUTIL__H__
