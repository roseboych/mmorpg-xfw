/**
* css application
*
* @category		physics
* @package		
* @author		Created by Lu Jianfeng ( lujf1978@163.com ), 2009
*/
#include "PhysxMemoryStream.h"

#include <stdio.h>
#include "NxPhysics.h"

//-----------------------------------------------PhysxMemoryWriteBuffer----------------------------------------------//
PhysxMemoryWriteBuffer::PhysxMemoryWriteBuffer():
currentsize_(0), maxsize_(0), data_( 0)
{
}

PhysxMemoryWriteBuffer::~PhysxMemoryWriteBuffer()
{
	NX_DELETE_ARRAY( data_);
}

void PhysxMemoryWriteBuffer::clear()
{
	currentsize_ = 0;
}

NxStream& PhysxMemoryWriteBuffer::storeByte( NxU8 b)
{
	storeBuffer( &b, sizeof( NxU8));

	return *this;
}

NxStream& PhysxMemoryWriteBuffer::storeWord( NxU16 w)
{
	storeBuffer( &w, sizeof(NxU16));

	return *this;
}

NxStream& PhysxMemoryWriteBuffer::storeDword( NxU32 d)
{
	storeBuffer( &d, sizeof( NxU32));

	return *this;
}

NxStream& PhysxMemoryWriteBuffer::storeFloat( NxReal f)
{
	storeBuffer( &f, sizeof(NxReal));

	return *this;
}

NxStream& PhysxMemoryWriteBuffer::storeDouble( NxF64 f)
{
	storeBuffer( &f, sizeof( NxF64));

	return *this;
}

NxStream& PhysxMemoryWriteBuffer::storeBuffer( const void* buffer, NxU32 size)
{
	NxU32 expectedSize = currentsize_ + size;
	if( expectedSize > maxsize_)
	{
		maxsize_ = expectedSize + 4096;

		NxU8* newData = new NxU8[maxsize_];
		NX_ASSERT( newData!=NULL);

		if( data_)
		{
			memcpy( newData, data_, currentsize_);
			delete[] data_;
		}
		data_ = newData;
	}
	memcpy( data_ + currentsize_, buffer, size);
	currentsize_ += size;

	return *this;
}

//-----------------------------------------------------PhysxMemoryReadBuffer--------------------------------------------
PhysxMemoryReadBuffer::PhysxMemoryReadBuffer(const NxU8* data):
buffer_(data)
{
}

PhysxMemoryReadBuffer::~PhysxMemoryReadBuffer()
{
}

NxU8 PhysxMemoryReadBuffer::readByte() const
{
	NxU8 b;
	memcpy( &b, buffer_, sizeof( NxU8));
	buffer_ += sizeof(NxU8);

	return b;
}

NxU16 PhysxMemoryReadBuffer::readWord() const
{
	NxU16 w;
	memcpy( &w, buffer_, sizeof(NxU16));
	buffer_ += sizeof(NxU16);

	return w;
}

NxU32 PhysxMemoryReadBuffer::readDword() const
{
	NxU32 d;
	memcpy( &d, buffer_, sizeof(NxU32));
	buffer_ += sizeof(NxU32);

	return d;
}

float PhysxMemoryReadBuffer::readFloat() const
{
	float f;
	memcpy( &f, buffer_, sizeof(float));
	buffer_ += sizeof(float);
	return f;
}

double PhysxMemoryReadBuffer::readDouble() const
{
	double f;
	memcpy( &f, buffer_, sizeof(double));
	buffer_ += sizeof(double);

	return f;
}

void PhysxMemoryReadBuffer::readBuffer(void* dest, NxU32 size) const
{
	memcpy( dest, buffer_, size);
	buffer_ += size;
}
