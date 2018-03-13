#include "Scheduler.h"



Scheduler::Scheduler()
{
}


Scheduler::~Scheduler()
{
}

void Scheduler::Arrive(Job job)
{
	incopmpleteJobList.push_front(job);
}

void Scheduler::Tick()
{
	elapsedTick++;

	
}
