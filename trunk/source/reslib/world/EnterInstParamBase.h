/**
* reslib
*
* @category		��������
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __ENTERINSTPARAMBASE__H__
#define __ENTERINSTPARAMBASE__H__

#include <corelib/corelibdef.h>
#include <string>

class TiXmlElement;

#define ENTERINSTCHECK_DEFINE( classname)							\
public:																\
	static const NS_STL::string& get_classname(){					\
		static NS_STL::string s_##classname_name =#classname;		\
		return s_##classname_name;									\
	}

/**
* @class EnterInstParamBase
* 
* @brief ���븱����ͼ�ж��������û���
* 
**/
class EnterInstParamBase
{
public:
	static EnterInstParamBase* create( NS_STL::string clsname);

public:
	/**
	* ����������Ϣ
	* @param ptr
	* @return
	**/
	virtual bool load_params( TiXmlElement* ptr) =0;
};

#endif	//__ENTERINSTPARAMBASE__H__
