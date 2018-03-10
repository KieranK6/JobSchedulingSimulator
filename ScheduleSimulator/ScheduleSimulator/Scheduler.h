#pragma once
#include <list>
#include "Job.h"

class Scheduler
{
public:
	std::list<Job> jobList;


public:
	Scheduler();
	~Scheduler();

	void Arrive(Job job);
	void Tick();
};

