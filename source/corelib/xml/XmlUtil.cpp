/**
* corelib
*
* @category		xml module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "XmlUtil.h"

bool XmlUtil::loadfromfile(char * sfile, TiXmlDocument& doc)
{
	if(sfile == 0)
		return false;

	size_t iSize=strlen(sfile);

	sfile[iSize]='\n';
	sfile[iSize+1]=0;
	sfile[iSize+2]='\0';
	
	if( doc.Parse(sfile) == 0)
		return false;

	return true;
}

bool XmlUtil::loadfromstring(char *contents, TiXmlDocument& doc)
{
	if( contents == 0 || strlen( contents) == 0)
		return false;

	doc.Parse( contents);
	if( doc.Error())
		return false;

	return true;
}


bool XmlUtil::GetXmlAttrBool( TiXmlElement* e, const char* attr, bool defv)
{
	if( e == 0)
		return defv;

	const char* r =e->Attribute( attr);
	if( r == 0)
		return defv;

	if( stricmp( r, "true") ==0)
		return true;
	else if( stricmp( r, "false") ==0)
		return false;
	else
		return defv;
}

bool XmlUtil::GetXmlAttrBoolFromInt( TiXmlElement* e, const char* attr, bool defv, int ntruev)
{
	if( e == 0)
		return defv;

	int r =(defv?ntruev+1:ntruev);
	e->QueryIntAttribute( attr, &r);

	return (r != ntruev);
}

int XmlUtil::GetXmlAttrInt( TiXmlElement* e, const char* attr, int defv)
{
	if( e == 0)
		return defv;

	int r =defv;
	e->QueryIntAttribute( attr, &r);

	return r;
}

float XmlUtil::GetXmlAttrFloat( TiXmlElement* e, const char* attr, float defv)
{
	if( e == 0)
		return defv;

	double r =(double)defv;
	e->QueryDoubleAttribute( attr, &r);

	return (float)r;
}

NS_STL::string XmlUtil::GetXmlAttrStr( TiXmlElement*e, const char* attr,  const char* defv)
{
	if( e == 0)
		return defv;

	const char* r =e->Attribute( attr);
	if( r == 0)
		return defv;

	return r;
}

NS_STL::string XmlUtil::GetXmlText( TiXmlElement* e, const char* defv)
{
	if( e == 0)
		return defv;

	const TiXmlNode* child = e->FirstChild();
	if( child)
	{
		const TiXmlText* childText =child->ToText();
		if( childText)
			return childText->Value();
	}

	return defv;
}
