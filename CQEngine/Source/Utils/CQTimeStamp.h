/*
*
*		qiu_hao		2018/02/22		22:30.pm
*/
#ifndef __COTIMESTAMP_H__
#define __COTIMESTAMP_H__

#include "CQMacros.h"
#include <chrono>

NS_CQ_BEGIN

using namespace std::chrono;

class CQTimeStamp
{
public:
	CQTimeStamp();
public:
	double getElapsedSecond();

	double getElapsedTimeInMilliSec();

	long long getElapsedTimeInMicroSec();

private:
	time_point<high_resolution_clock> begin_;
};


NS_CQ_END

#endif /* __COTIMESTAMP_H__ */
