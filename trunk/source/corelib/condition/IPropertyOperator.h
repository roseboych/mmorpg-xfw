/**
* corelib
*
* @category		Condition Lib
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __IPROPERTYOPERATOR__H__
#define __IPROPERTYOPERATOR__H__

#include <ace/Assert.h>

enum PROPERTYOPERATOR_VALUETYPE
{
	PROPERTYOPER_VT_INT =0,
	PROPERTYOPER_VT_FLOAT,
	PROPERTYOPER_VT_STR,
};

class PropertyOperatorValue
{
public:
	PropertyOperatorValue():value_type_(PROPERTYOPER_VT_INT){
		value_.int_ =0;
	}

	void set_int( S_INT_32 v){
		value_.int_ =v;
		value_type_ =PROPERTYOPER_VT_INT;
	}
	S_INT_32 get_int(){
		ACE_ASSERT( value_type_ == PROPERTYOPER_VT_INT);
		return value_.int_;
	}

	void set_float( S_FLOAT_32 v){
		value_.float_ =v;
		value_type_ =PROPERTYOPER_VT_FLOAT;
	}
	S_FLOAT_32 get_float(){
		ACE_ASSERT( value_type_ == PROPERTYOPER_VT_FLOAT);
		return value_.float_;
	}

	void set_str( const S_CHAR_8* v){
		str_ =v;
		value_type_ =PROPERTYOPER_VT_STR;
	}

	const NS_STL::string get_str(){
		ACE_ASSERT( value_type_ == PROPERTYOPER_VT_STR);
		return str_;
	}

	bool operator > ( const PropertyOperatorValue& v) const{
		ACE_ASSERT( (value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_);
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ > (S_INT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ > (S_FLOAT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ > v.str_;
		default:
			ACE_ASSERT( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

	bool operator >= ( const PropertyOperatorValue& v) const{
		ACE_ASSERT( (value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_);
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ >= (S_INT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ >= (S_FLOAT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ >= v.str_;
		default:
			ACE_ASSERT( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

	bool operator < ( const PropertyOperatorValue& v) const{
		ACE_ASSERT( (value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_);
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ < (S_INT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ < (S_FLOAT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ < v.str_;
		default:
			ACE_ASSERT( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

	bool operator <= ( const PropertyOperatorValue& v) const{
		ACE_ASSERT( (value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_);
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ <= (S_INT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ <= (S_FLOAT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ <= v.str_;
		default:
			ACE_ASSERT( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

	bool operator == ( const PropertyOperatorValue& v) const{
		ACE_ASSERT( (value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_);
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ == (S_INT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ == (S_FLOAT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ == v.str_;
		default:
			ACE_ASSERT( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

	bool operator != ( const PropertyOperatorValue& v) const{
		ACE_ASSERT( (value_type_ == PROPERTYOPER_VT_STR || v.value_type_ == PROPERTYOPER_VT_STR) && value_type_ != v.value_type_);
		switch( value_type_)
		{
		case PROPERTYOPER_VT_INT:
			return value_.int_ != (S_INT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_FLOAT:
			return value_.float_ != (S_FLOAT_32)( v.value_type_ == PROPERTYOPER_VT_INT? v.value_.int_:v.value_.float_);
		case PROPERTYOPER_VT_STR:
			return str_ != v.str_;
		default:
			ACE_ASSERT( false && "PropertyOperatorValue not support value type");
			return false;
		}
	}

public:
	union
	{
		S_INT_32		int_;
		S_FLOAT_32		float_;
	}value_;
	NS_STL::string	str_;

	PROPERTYOPERATOR_VALUETYPE	value_type_;
};

/**
* @class IPropertyOperator
* 
* @brief 支持条件判定的类必须实现该接口
**/
class IPropertyOperator
{
public:
	virtual PropertyOperatorValue get_property( const char* propname) =0;

};

#endif	//__IPROPERTYOPERATOR__H__
