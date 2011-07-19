/**
* reslib
*
* @category		��������
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2011
*/
#ifndef __INSTMAPOPTION__H__
#define __INSTMAPOPTION__H__

#include <corelib/corelibdef.h>
#include <corelib/condition/ConditionElement.h>
#include "reslib/reslib_def.h"

#include <string>

class TiXmlElement;
class EnterInstParamBase;
class StoryMapOption;

/**
* @class InstMapOption
* 
* @brief ������ͼ�������
* 
**/
class InstMapOption
{
public:
	InstMapOption();

	/**
	* ���غ͸�����ص�����ѡ��
	* @param e
	**/
	S_BOOL	load_instmapopt( TiXmlElement* e, StoryMapOption* storyopt);

protected:
	/*
	* ���ؽ��븱���ж�
	* @param e
	* @return
	*/
	S_BOOL	load_enterinstopt( TiXmlElement* e);

public:
	//�Ƿ�����븱������
	S_BOOL				is_check_enterinst_;
	//���븱��֧��
	FUNCTION_SUPPORT	enterinst_fun_;
	//c++����
	EnterInstParamBase*	enterinst_param_;
	NS_STL::string		enterinst_class_;
	//condition����
	ConditionElement*	enterinst_condition_;

	//�ű���Ϣ
	NS_STL::string		inst_script_;

	//����״̬����ʵ��
	NS_STL::string		ctrl_classname_;
};

#endif	//__INSTMAPOPTION__H__