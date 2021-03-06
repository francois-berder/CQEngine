/*
*
*		qiu_hao		2017/12/25		04:11.pm
*/
#ifndef __CQALLOCATOR_H__
#define __CQALLOCATOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "CQMacros.h"

NS_CQ_BEGIN

class AllocatorI
{
public:
	virtual ~AllocatorI() = 0;

	virtual void* alloc(size_t _size) throw(std::bad_alloc) = 0;

	virtual void* alloc(size_t _size, const char *_file, size_t _line) throw(std::bad_alloc) = 0;

	virtual void free(void *_ptr) = 0;

	virtual void free(void *_ptr, const char *_file, size_t _line) = 0;

};

class DefaultAllocator : public AllocatorI
{
public:
	DefaultAllocator();

	virtual ~DefaultAllocator();

public:
	void* alloc(size_t _size) throw(std::bad_alloc);

	void* alloc(size_t _size, const char *_file, size_t _line) throw(std::bad_alloc);

	void free(void *_ptr);

	void free(void *_ptr, const char *_file, size_t _line);
public:
	size_t getSize1(void *_ptr);
	size_t getSize2(void *_ptr);
	size_t getSize3(void *_ptr);
	size_t getSize4(void *_ptr) ;

	void setSize1(void *_ptr, const size_t _size1);
	void setSize2(void *_ptr, const size_t _size1);
	void setSize3(void *_ptr, const size_t _size1);
	void setSize4(void *_ptr, const size_t _size1);

private:
	struct Info
	{ size_t size_[4] = {};};
	std::map<void *, Info> map_;
};

NS_CQ_END

#include "CQAllocator.inl"

#endif /*__CQALLOCATOR_H__*/