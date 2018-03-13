#pragma once
#include "Scheduler.h"
class RoundRobin :
	public Scheduler
{
public:
	float sliceLength;
public:
	RoundRobin(float sliceLength);
	~RoundRobin();
};

