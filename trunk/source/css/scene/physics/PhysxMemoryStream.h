/**
* css application
*
* @category		physics
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#ifndef __PHYSXMEMORYSTREAM__H__
#define __PHYSXMEMORYSTREAM__H__

#include "Nxf.h"
#include "NxSimpleTypes.h"
#include "NxStream.h"

/**
* @class PhysxMemoryWriteBuffer
* 
* @brief 创建mesh时使用
* 
**/
class PhysxMemoryWriteBuffer : public NxStream
{
public:
	PhysxMemoryWriteBuffer();
	virtual	~PhysxMemoryWriteBuffer();
	void clear();

	virtual NxU8 readByte()	const
	{
		NX_ASSERT(0);
		return 0;
	}
	
	virtual	NxU16 readWord() const
	{
		NX_ASSERT(0);
		return 0;
	}

	virtual	NxU32 readDword() const
	{
		NX_ASSERT(0);
		return 0;
	}

	virtual	float readFloat() const
	{
		NX_ASSERT(0);
		return 0.0f;
	}
	
	virtual	double readDouble() const
	{
		NX_ASSERT(0);
		return 0.0;
	}

	virtual	void readBuffer( void* buffer, NxU32 size) const
	{
		NX_ASSERT(0);
	}

	virtual	NxStream& storeByte( NxU8 b);
	virtual	NxStream& storeWord( NxU16 w);
	virtual	NxStream& storeDword( NxU32 d);
	virtual	NxStream& storeFloat( NxReal f);
	virtual	NxStream& storeDouble( NxF64 f);
	virtual	NxStream& storeBuffer( const void* buffer, NxU32 size);

public:
	NxU32	currentsize_;
	NxU32	maxsize_;
	NxU8*	data_;
};

/**
* @class PhysxMemoryReadBuffer
* 
* @brief 创建mesh时使用
* 
**/
class PhysxMemoryReadBuffer : public NxStream
{
public:
	PhysxMemoryReadBuffer(const NxU8* data);
	virtual ~PhysxMemoryReadBuffer();

	virtual	NxU8 readByte()	const;
	virtual	NxU16 readWord() const;
	virtual	NxU32 readDword() const;
	virtual	float readFloat() const;
	virtual	double readDouble() const;
	virtual	void readBuffer(void* buffer, NxU32 size) const;

	virtual	NxStream& storeByte(NxU8 b)
	{
		NX_ASSERT(0);
		return *this;
	}

	virtual	NxStream& storeWord(NxU16 w)
	{
		NX_ASSERT(0);
		return *this;
	}

	virtual	NxStream& storeDword(NxU32 d)
	{
		NX_ASSERT(0);
		return *this;
	}

	virtual	NxStream& storeFloat(NxReal f)
	{
		NX_ASSERT(0);
		return *this;
	}

	virtual	NxStream& storeDouble(NxF64 f)
	{
		NX_ASSERT(0);
		return *this;
	}

	virtual	NxStream& storeBuffer(const void* buffer, NxU32 size)
	{
		NX_ASSERT(0);
		return *this;
	}

public:
	//读取的缓存
	mutable const NxU8* buffer_;
};

#endif	//__PHYSXMEMORYSTREAM__H__
