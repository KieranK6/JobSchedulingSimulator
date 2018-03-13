#include "Scheduler.h"



Scheduler::Scheduler()
{
	isComplete = false;
}


Scheduler::~Scheduler()
{
}

void Scheduler::Arrive(Job job)
{
	incompleteJobList.push_front(job);
}

void Scheduler::Tick()
{
	elapsedTick++;

	
}
