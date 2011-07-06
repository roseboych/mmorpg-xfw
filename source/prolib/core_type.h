/**
* protocol module
* 
* 重定义了协议使用的数据类型
*
* @category		
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __CORE_TYPE__H__
#define __CORE_TYPE__H__

#define PROTOCOL_NAMESPACE_BEGIN	namespace PRO {
#define PROTOCOL_NAMESPACE_END		}
#define USE_PROTOCOL_NAMESPACE		using namespace PRO;

#define PROLIB_NAMESPACE_BEGIN		namespace PROLIB{
#define PROLIB_NAMESPACE_END		}
#define USE_PROLIB_NAMESPACE		using namespace PROLIB;

#ifdef _USE_32BIT_TIME_T
	#undef _USE_32BIT_TIME_T
#endif	//_USE_32BIT_TIME_T

#define S_INT_64	__int64
#define	S_INT_32	int
#define S_INT_16	short
#define S_INT_8		char
#define S_CHAR_8	char
#define S_FLOAT_32	float
#define S_BOOL		bool

#ifdef _USE_32BIT_TIME_T
	#define S_TIMESTAMP		long
#else
	#define S_TIMESTAMP		unsigned __int64
#endif	//unsigned int

#define S_UINT_32		unsigned int
#define S_UINT_16		unsigned short
#define S_UINT_8		unsigned char
#define S_UCHAR_8		unsigned char
#define S_UINT_64		unsigned __int64
#define S_ULONG_32		unsigned long
#define S_ULONG_64		unsigned __int64

#define S_BITFIELD(PARAMNAME)	unsigned char ##PARAMNAME:1

#define NS_STL	std

#define	NO_INITVALUE	-1

#define PARAM_UNUSED(pv)	((void)&pv)

//服务器类型
enum NETSVR_TYPE_ENUM{
	//rgs服务器
	NETSVR_RGS =0,
	//lgs
	NETSVR_LGS,
	//cts
	NETSVR_CTS,
	//dpx
	NETSVR_DPX,
	//css
	NETSVR_CSS,
	//gts
	NETSVR_GTS,
	//no
	NETSVR_NONE
};

//uuid session
struct uuid_session
{
	void set_uuid( S_INT_32 uid, S_TIMESTAMP st){ this->userid_ =uid; this->init_timestamp_ =st;}
	void set_timestamp( S_TIMESTAMP st){ this->init_timestamp_ =st;}
	void set_userid( S_INT_32 uid){ this->userid_ =uid;}

	bool is_samesession( S_INT_32 uid, S_TIMESTAMP st){ return this->userid_ == uid && this->init_timestamp_ == st;}
	bool is_samesession( uuid_session& sess){ return this->userid_ == sess.userid_ && this->init_timestamp_ == sess.init_timestamp_;}

	void reset(){ userid_ =NO_INITVALUE; init_timestamp_ =0;}

	S_INT_32		userid_;
	S_TIMESTAMP		init_timestamp_;
};

//定义了所有支持的object serialize factory
enum SERIALIZEFACTORY_ENUM
{
	SERIALIZEFACTORY_NONE =-1,
	//技能包序列化
	SERIALIZEFACTORY_SKILLPKG,
	SERIALIZEFACTORY_MAX
};

#endif //__CORE_TYPE__H__
