#include "Job.h"



Job::Job(std::string name, int arrivalTime, int runTime) : name(name), arrivalTime(arrivalTime), runTime(runTime)
{
	jobComplete = false;
}

Job::Job()
{
}


Job::~Job()
{
}

void Job::jobTick()
{
	runTime--;
	if (runTime <= 0)
		jobComplete = true;
}