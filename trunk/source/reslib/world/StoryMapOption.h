/**
* reslib
*
* @category		world config
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __STORYMAPOPTION__H__
#define __STORYMAPOPTION__H__

#if !defined (ACE_LACKS_PRAGMA_ONCE) 
#pragma once
#endif

#include <corelib/corelibdef.h>
#include <prolib/core_type.h>
#include <corelib/mconf/IConfigContentSource.h>

#include "reslib/world/res_struct_def.h"
#include "reslib/world/InstMapOption.h"

#include <string>

class CSSOption;

/**
* @class StoryMapOption
* 
* @brief ��ͼ������Ϣ
**/
class StoryMapOption
{
public:
	StoryMapOption( const char* path, int mapid, int offsetx =0, int offsety =0, bool binst =false);

	/**
	* ���ص�ͼ��Դ
	* @return
	**/
	bool load_mapres();

	/**
	* ��ȡ��ͼ��Դ����·����·���°����͸õ�ͼ��ص�������Դ
	* @return
	**/
	NS_STL::string get_mapdefpath(){ return res_path_;}

	S_INT_32 get_mapid(){ return mapid_;}

	void world_to_local( S_FLOAT_32& x, S_FLOAT_32& y){
		x =x - offsetx_;
		y =y - offsety_;
	}

	void local_to_world( S_FLOAT_32&x, S_FLOAT_32& y){
		x =x + offsetx_;
		y =y + offsety_;
	}

	bool worldxy_inthismap( S_FLOAT_32 x, S_FLOAT_32 y);

	NS_STL::string get_respath(){ return res_path_;}

public:
	//��ͼ���
	S_INT_32	mapid_;

	//��Ⱥ͸߶�(��ͼ����)
	S_INT_32	leftx_, lefty_;
	S_INT_32	bottomx_, bottomy_;
	S_INT_32	width_;
	S_INT_32	height_;

	NS_STL::string	mapdesc_;

	//��������ͼ������ƫ��
	S_INT_32		offsetx_, offsety_;

	//������ͼ��չ����
	//�Ƿ񸱱���ͼ
	bool			is_instancemap_;
	//������ͼ�������
	InstMapOption	instmap_opt_;

	//������css������,ֻ����������Ч���Ը�����Ч
	CSSOption*	owner_css_;

protected:
	//��Դλ��
	NS_STL::string	res_path_;
};

#endif	//__STORYMAPOPTION__H__
