#pragma once
#include <list>
#include "Job.h"

class Scheduler
{
public:
	std::list<Job> incompleteJobList;
	std::list<Job> completeJobList;

	bool isComplete;

	//Job currentJob;

	int elapsedTick;

public:
	Scheduler();
	~Scheduler();

	void Arrive(Job job);
	void Tick();
};

