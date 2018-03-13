#include "Job.h"



Job::Job(std::string name, int arrivalTime, int runTime) : name(name), arrivalTime(arrivalTime), runTime(runTime)
{
	
}


Job::~Job()
{
}

void Job::jobTick()
{

}