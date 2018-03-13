#include "Scheduler.h"



Scheduler::Scheduler()
{
	elapsedTick = 0;
	isComplete = false;
}


Scheduler::~Scheduler()
{
}

void Scheduler::Arrive(Job job)
{
	incompleteJobQueue.push(job);
}

void Scheduler::Tick()
{
	elapsedTick++;
	
	incompleteJobQueue.front().jobTick();

	if (incompleteJobQueue.front().jobComplete == true)
	{
		incompleteJobQueue.front().endTime = elapsedTick;
		completeJobList.push_front(incompleteJobQueue.front());
		incompleteJobQueue.pop();
	}
		
	
}

void Scheduler::Run(int totalJobTime)
{
	for (int i = 0; i <= totalJobTime; i++)
	{
		std::string curJob = incompleteJobQueue.front().name;
		int remainingTicks = incompleteJobQueue.front().runTime;
		//print out values
		std::string output = " | Job: " + curJob + " | Ticks Remaining: ";
		std::cout << "Current Tick: " << elapsedTick << output << remainingTicks << std::endl;

		if (remainingTicks <= 1)
			std::cout << curJob + " Complete !!" << std::endl;

		//Check for Arrivals

		//Do Tick
		Tick();
	}
}
