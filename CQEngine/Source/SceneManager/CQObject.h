/*
*
*		qiu_hao		2018/03/10		11:02.pm
*/
#ifndef __CQOBJECT_H__
#define __CQOBJECT_H__

#include <string>
#include <memory>
#include <map>
#include "CQMacros.h"
#include "CQRunnable.h"

NS_CQ_BEGIN

class CQComponent;

class CQObject : public CQRunnable
{
public:
	explicit CQObject();

	virtual ~CQObject();

public:
	virtual void onInit();

	virtual void update();

	virtual void onDestory();

public:
	void setName(const std::string _name);
	
	void setTag(uint32_t _tag);

	const std::string& getName() const;

	const uint32_t getTag() const;

	const uint64_t getID() const;

public:
	void setComponent(std::shared_ptr<CQComponent> _component);

	std::shared_ptr<CQComponent> getComponentByName(const std::string& _name);

	void removeComponent(const std::string& _name);

private:
	const uint64_t genID() const;

protected:
	uint32_t		tag_;
	uint64_t		id_;
	std::string		name_;
	std::map<std::string, std::shared_ptr<CQComponent>> compMap_;
};

#include "CQObject.inl"

NS_CQ_END

#endif /*__CQOBJECT_H__*/