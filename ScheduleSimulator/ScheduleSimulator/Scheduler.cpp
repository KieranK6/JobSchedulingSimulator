#include "Scheduler.h"



Scheduler::Scheduler()
{
}


Scheduler::~Scheduler()
{
}

void Scheduler::Arrive(Job job)
{
	jobList.push_front(job);
}

void Scheduler::Tick()
{

}
