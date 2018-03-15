#include "Job.h"
#include "Scheduler.h"

int totalJobTime;

int main()
{
	Job a = Job("JobA", 10, 12); //starts at 10, 22 a completes
	Job b = Job("JobB", 15, 8); //starts at 22, 30 b completes
	Job c = Job("JobC", 1, 35); //starts at 60, 95 c completes
	Job d = Job("JobD", 1, 30); //starts at 30, 60 d completes
	
	//FIFO	c, d, b, a
	//SJF	d, c, b, a
	//STTC  

	Scheduler scheduler = Scheduler();

	scheduler.loadedJobsFromFile.push_back(a);
	scheduler.loadedJobsFromFile.push_back(b);
	scheduler.loadedJobsFromFile.push_back(c);
	scheduler.loadedJobsFromFile.push_back(d);

	//scheduler.Arrive(a);
	//scheduler.Arrive(b);
	//scheduler.Arrive(c);
	//scheduler.Arrive(d);

	scheduler.Simulate();

	std::getchar();
}