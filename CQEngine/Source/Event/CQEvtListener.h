//*****************************************************************************
//
//! \file CQEvtListener.h
//! \brief A tiny event listener.
//! \author qiu_hao
//! \date 2018/11/23 05:16.pm
//
//*****************************************************************************
#ifndef __CQEVTLISTENER_H__
#define __CQEVTLISTENER_H__

#include <functional>
#include "CQMacros.h"
#include "CQNoncopyable.h"

NS_CQ_BEGIN

// TODO:
// Very similar to TimeAction, except that one is based on time and one is based on events. 
// Both can be unified in the future.

class CQEvtListener : private CQNoncopyable
{
public:
	typedef std::function<void(void*)> Callback;

public:
	explicit CQEvtListener();

	CQEvtListener(unsigned int _evtID, Callback& _cb);

	bool operator==(const CQEvtListener& _other);

	bool operator!=(const CQEvtListener& _other);

	virtual ~CQEvtListener();

public:
	void setEvtID(unsigned int _evtID);
	
	void setCB(Callback _cb);

	unsigned int getEvtID() const;

	Callback getCB();

	bool isCBValid();

private:
	unsigned int evtID_;

	Callback cb_;
};

NS_CQ_END

#endif /* __CQEVTLISTENER_H__ */

