#include "Job.h"
#include "Scheduler.h"

int totalJobTime;

int main()
{
	Job a = Job("A", 10, 12); //starts at 10, 22 a completes
	Job b = Job("B", 15, 8); //starts at 22, 30 b completes
	Job c = Job("C", 20, 35); //starts at 60, 95 c completes
	Job d = Job("D", 15, 30); //starts at 30, 60 d completes


	std::list<Job> tempJobList;
	tempJobList.push_front(a);
	tempJobList.push_front(b);
	tempJobList.push_front(c);
	tempJobList.push_front(d);

	Scheduler FIFO = Scheduler();

	FIFO.Arrive(a);
	FIFO.Arrive(b);
	FIFO.Arrive(c);
	FIFO.Arrive(d);

	for (Job j : tempJobList)
	{
		totalJobTime += j.runTime;
	}

	FIFO.Run(totalJobTime);

}