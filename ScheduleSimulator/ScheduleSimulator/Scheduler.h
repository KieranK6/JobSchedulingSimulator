#pragma once
#include <list>
#include "Job.h"

class Scheduler
{
public:
	std::list<Job> incopmpleteJobList;
	std::list<Job> completeJobList;

	//Job currentJob;

	int elapsedTick;

public:
	Scheduler();
	~Scheduler();

	void Arrive(Job job);
	void Tick();
};

