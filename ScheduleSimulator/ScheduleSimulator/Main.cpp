#include "Job.h"
#include "Scheduler.h"

int totalJobTime;

int main()
{
	Scheduler scheduler = Scheduler();

	scheduler.LoadJobs("Jobs.txt");

	//cout enter filename
	//cin filename
	//scheduler.loadjob(filename)

	scheduler.Simulate();

	std::getchar();
}