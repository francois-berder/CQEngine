//*****************************************************************************
//
//! \file CQRenderer.h
//! \brief interface file of render.
//! \author qiu_hao
//! \date 2018/03/31 05:57.pm
//
//*****************************************************************************
#ifndef __CQRENDERER_H__
#define __CQRENDERER_H__

#include <vector>
#include "CQMacros.h"
#include "CQNoncopyable.h"
#include "CQVisiableObj.h"

NS_CQ_BEGIN

class CQContext;

class CQRenderer : private CQNoncopyable
{
public:
	virtual ~CQRenderer();

public:
	CQContext* getContext();

	void draw(std::vector<CQVisiableObj*> _visibleObjVec);

private:
	friend class CQCore;

	explicit CQRenderer();

private:
	CQContext * context_;
};

NS_CQ_END

#endif /*__CQRENDERER_H__*/

