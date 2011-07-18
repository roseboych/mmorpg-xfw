/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#include "corelib/condition/ConditionNode.h"

#include "corelib/xml/XmlUtil.h"
#include <ace/Assert.h>

#include "corelib/condition/ConditionLeaf.h"

ConditionNode::ConditionNode( ConditionElement* l, ConditionElement* r, CONDITION_CONNECTOR connector):
left_( l),
right_( r),
connector_( connector)
{
	ACE_ASSERT( left_ != 0);

	if( connector_ != CONDITION_CONNECTOR_NOT)
		ACE_ASSERT( right_ != 0);
}

ConditionNode::ConditionNode( TiXmlElement* e)
{
	NS_STL::string conn =XmlUtil::GetXmlAttrStr( e, "connector", "and");
	CONDITION_CONNECTOR ctype =CONDITION_CONNECTOR_AND;
	if( conn == "and")
		ctype =CONDITION_CONNECTOR_AND;
	else if( conn == "or")
		ctype =CONDITION_CONNECTOR_OR;
	else
		ctype =CONDITION_CONNECTOR_NOT;

	ConditionElement *lnode =0, *rnode =0;

	TiXmlElement* l1 =e->FirstChildElement( "condition");
	NS_STL::string tye =XmlUtil::GetXmlAttrStr( l1, "type", "node");
	if( tye == "node")
		lnode =FRAMEWK_NEW ConditionNode( l1);
	else if( tye == "leaf")
		lnode =FRAMEWK_NEW ConditionLeaf( l1);

	TiXmlElement* r1 =0;
	if( l1)
	{
		r1 =l1->NextSiblingElement( "condition");
		tye =XmlUtil::GetXmlAttrStr( r1, "type", "node");
		if( tye == "node")
			rnode =FRAMEWK_NEW ConditionNode( r1);
		else if( tye == "leaf")
			rnode =FRAMEWK_NEW ConditionLeaf( r1);
	}

	left_ =lnode;
	right_ =rnode;
	connector_ =ctype;

	ACE_ASSERT( left_ != 0);
	if( connector_ != CONDITION_CONNECTOR_NOT)
		ACE_ASSERT( right_ != 0);
}

ConditionNode::~ConditionNode()
{
	if( left_)
		delete left_;
	left_ =0;

	if( right_)
		delete right_;
	right_ =0;
}

S_BOOL ConditionNode::condition_calcuate( ConditionContext& context)
{
	S_BOOL lret =true, rret =true;
	if( left_)
		lret =left_->condition_calcuate( context);
	if( right_)
		rret =right_->condition_calcuate( context);

	if( connector_ == CONDITION_CONNECTOR_NOT)
		return !lret;
	else if( connector_ == CONDITION_CONNECTOR_AND)
		return lret & rret;
	else if( connector_ == CONDITION_CONNECTOR_OR)
		return lret || rret;
	else
	{
		ACE_ASSERT( false && "ConditionNode::condition_calcuate Not support condition connector type");
		return false;
	}
}
