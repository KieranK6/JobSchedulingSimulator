#include "Job.h"
#include "Scheduler.h"

int totalJobTime;

int main()
{
	Job a = Job("A", 10, 12); //starts at 10, 22 a completes
	Job b = Job("B", 15, 8); //starts at 22, 30 b completes
	Job c = Job("C", 20, 35); //starts at 60, 95 c completes
	Job d = Job("D", 2, 30); //starts at 30, 60 d completes


	std::list<Job> tempJobList;
	tempJobList.push_front(a);
	tempJobList.push_front(b);
	tempJobList.push_front(c);
	tempJobList.push_front(d);

	Scheduler scheduler = Scheduler();

	scheduler.Arrive(a);
	scheduler.Arrive(b);
	scheduler.Arrive(c);
	scheduler.Arrive(d);


	//scheduler.RunFIFO();
	scheduler.Simulate();

	std::getchar();
}