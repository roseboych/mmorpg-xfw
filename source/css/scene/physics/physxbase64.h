/**
* css application
*
* @category		physics
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PHYSX_BASE64__H__
#define __PHYSX_BASE64__H__

#include <boost/cstdint.hpp>

/**
* @class physx_base64
* 
* @brief base64编码解码类
* 
**/
class physx_base64
{
public: 
	static char* encode( const void* pvSrcBinaryBuffer, int iSrcSize, int iMaxSizeToRead = 0);

	/**
	* 解码
	* @param stBytesWritten 传入目标buffer的大小，传出实际使用大小
	**/
	static bool decode( void* pvDestBinaryBuffer, const char* pcSrcBase64Ascii, size_t& stBytesWritten);

private:
	static const unsigned char ms_aucBase64Char[65];
	static const unsigned char ms_aucBase64ToBit6[128];
};

#endif // __PHYSX_BASE64__H__
