/*
*
*		qiu_hao		2018/08/18		11:10.pm
*/

#include "CQCore.h"

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

USING_NS_CQ

class HelloWorldScene : public CQScene
{
public:
	// NO Call onInit here.
	explicit HelloWorldScene() {} 

	virtual ~HelloWorldScene() {}
public:
	///
	virtual void onInit();

	///
	virtual void update();

	///
	virtual void onDestory();

};

#endif /* __HELLOWORLD_SCENE_H__ */
