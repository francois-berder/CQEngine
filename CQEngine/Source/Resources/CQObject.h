/*
*
*		qiu_hao		2018/03/10		11:02.pm
*/
#ifndef __CQOBJECT_H__
#define __CQOBJECT_H__

#include "CQMacros.h"
#include <string>

NS_CQ_BEGIN

class CQObject
{
public:
	explicit CQObject();

	virtual ~CQObject();
public:
	/// Interface
	virtual void onInit() = 0;
	virtual void update() = 0;
	virtual void onDestory() = 0;

public:
	///
	void setName(const std::string _name);

	///
	const std::string& getName() const;

	///
	const uint32_t getTag() const;
	
	////
	void setTag(uint32_t _tag);

	///
	const uint64_t getID() const;

private:
	const uint64_t genID() const;

protected:
	std::string		name_;
	uint32_t		tag_;
	uint64_t		id_;
};

#include "CQObject.inl"

NS_CQ_END

#endif /*__CQOBJECT_H__*/