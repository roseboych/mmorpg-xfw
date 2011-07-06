/**
* corelib
*
* @category		util module
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2010
*/
#ifndef __CRCUTIL__H__
#define __CRCUTIL__H__

#include <prolib/core_type.h>

/**
* @class CRCUtil
* 
* @brief crc校验算法
**/
class CRCUtil
{
public:
	static S_ULONG_32 append_2_crc( S_ULONG_32 previousCRC, const char *rawData, S_ULONG_32 size);
	static S_ULONG_32 create_crc( const char *rawData, S_ULONG_32 size);
	static S_ULONG_32 create_crc( const char *string);
	static S_ULONG_32 create_crcnocase( const char *string);

	static S_ULONG_64 append_2_crc64( S_ULONG_64 previousCRC, const char *rawData, S_ULONG_32 size);
	static S_ULONG_64 create_crc64( const char *rawData, S_ULONG_32 size);
	static S_ULONG_64 create_crc64( const char *string);
	static S_ULONG_64 create_crc64nocase( const char *string);

protected:
	static S_ULONG_32 build_crc( const S_UINT_8 *buffer, int bytes);
	static S_ULONG_32 build_crcnocase( const S_UINT_8 *buffer, int bytes);

	static S_ULONG_64 build_crc64( const S_UINT_8 *buffer, int bytes);
	static S_ULONG_64 build_crc64nocase( const S_UINT_8 *buffer, int bytes);

private:
	static S_ULONG_32 crc_table32_[];
	static S_ULONG_64 crc_table64_[];
};

#endif //__CRCUTIL__H__
