#pragma once
#include <list>
#include "Job.h"
#include <queue>
#include <iostream>

class Scheduler
{
public:
	std::queue<Job> incompleteJobQueue;
	std::list<Job> completeJobList;

	bool isComplete;
	//Job currentJob;

	int elapsedTick;

public:
	Scheduler();
	~Scheduler();

	void Arrive(Job job);
	void Tick();
	void Run(int totalJobTime);
};

