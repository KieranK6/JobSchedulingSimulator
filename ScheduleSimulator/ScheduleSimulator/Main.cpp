#include "Job.h"
#include "Scheduler.h"

int main()
{
	Job a = Job("A", 10, 19);
	Job b = Job("B", 15, 6);
	Job c = Job("C", 20, 29);
	Job d = Job("D", 15, 30);

	Scheduler FIFO = Scheduler();

	FIFO.Arrive(a);
	FIFO.Arrive(b);
	FIFO.Arrive(c);
	FIFO.Arrive(d);

	while (FIFO.isComplete != true)
	{
		FIFO.Tick();
	}

}