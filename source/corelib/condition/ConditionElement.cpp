/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "corelib/condition/ConditionElement.h"

#include "corelib/xml/XmlUtil.h"
#include <string>

#include "corelib/condition/ConditionLeaf.h"
#include "corelib/condition/ConditionNode.h"

ConditionElement* ConditionElement::construct_condition( TiXmlElement* e)
{
	TiXmlElement* root =e->FirstChildElement( "condition");
	if( root == 0)
		return 0;

	NS_STL::string tye =XmlUtil::GetXmlAttrStr( root, "type", "node");
	if( tye == "node")
		return FRAMEWK_NEW ConditionNode( root);
	else if( tye == "leaf")
		return FRAMEWK_NEW ConditionLeaf( root);

	return 0;
}
