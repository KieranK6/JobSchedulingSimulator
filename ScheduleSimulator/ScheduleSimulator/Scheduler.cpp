#include "Scheduler.h"

Scheduler::Scheduler()
{
	elapsedTick = 0;
	isComplete = 0;

	running = true;
	FIFOcomplete = false;
	SJFcomplete = false;
	STTCcomplete = false;
	RR1Complete = false;
	RR2Complete = false;

	sortedSJF = false;
	sortedFIFO = false;
}

Scheduler::~Scheduler()
{
}

void Scheduler::Arrive(Job job)
{
	incompleteFIFOJobQueue.push(job);
}

bool SortByArrivalTime(Job i, Job j) { return i.arrivalTime < j.arrivalTime; }
bool SortByRunTime(Job i, Job j) { return i.runTime < j.runTime; }

void Scheduler::Tick()
{
	elapsedTick++;

	
}

void Scheduler::Simulate()
{
	//sort
	//sort();

	//print t FIFO SJF STTC RR1 RR2
	std::cout << "t \t FIFO \t SJF" << std::endl;
	//loop
	while (running)
	{
		Tick(); //Time management
		RunFIFO(); //Simulate FIFO
		RunSJF(); //Simulate SJF
		//STTC
		//RR1
		//RR2

		//print tick job1 job2 job3
		printTick();

		//check if complete
		checkComplete();
	}
}

void Scheduler::printTick()
{
	std::cout << elapsedTick << "/t" << currentFIFOJob.name << "/t" << currentSJFJob.name << std::endl;
}


void Scheduler::RunFIFO()
{
	if (incompleteFIFOJobQueue.empty())
	{
		FIFOcomplete = true;
		return;
	}
		
	else
	{
		if (!sortedFIFO)
		{
			sortFIFOQueue(&incompleteFIFOJobQueue);
			sortedFIFO = true;
		}

		currentFIFOJob = incompleteFIFOJobQueue.front();
		currentFIFOJob.jobTick();

		if (currentFIFOJob.jobComplete == true)
		{
			currentFIFOJob.endTime = elapsedTick;
			completeFIFOJobList.push_front(currentFIFOJob);
			incompleteFIFOJobQueue.pop();

			std::cout << "COMPLETE: " << currentFIFOJob.name << std::endl;
		}
	}

	

	//std::string curJob = incompleteFIFOJobQueue.front().name;
	//int remainingTicks = incompleteFIFOJobQueue.front().runTime;
	////print out values
	//std::string output = " | Job: " + curJob + " | Ticks Remaining: ";
	//std::cout << "Current Tick: " << elapsedTick << output << remainingTicks << std::endl;

	//if (remainingTicks <= 0)
	//	std::cout << "COMPLETE:" << curJob << std::endl;
}



void Scheduler::RunSJF()
{

	if (incompleteFIFOJobQueue.empty())
	{
		FIFOcomplete = true;
		return;
	}
	else
	{
		if (!sortedSJF)
		{
			sortSJFQueue(&incompleteSJFJobQueue);
			sortedSJF = true;
		}

		currentSJFJob = incompleteSJFJobQueue.front();
		currentSJFJob.jobTick();

		if (currentSJFJob.jobComplete == true)
		{
			currentSJFJob.endTime = elapsedTick;
			completeSJFJobList.push_front(currentSJFJob);
			incompleteSJFJobQueue.pop();

			std::cout << "COMPLETE: " << currentSJFJob.name << std::endl;
		}
	}
	//std::string curJob = incompleteFIFOJobQueue.front().name;
	//int remainingTicks = incompleteFIFOJobQueue.front().runTime;
	////print out values
	//std::string output = " | Job: " + curJob + " | Ticks Remaining: ";
	//std::cout << "Current Tick: " << elapsedTick << output << remainingTicks << std::endl;

	//if (remainingTicks <= 1)
	//	std::cout << curJob + " Complete !!" << std::endl;
}

void Scheduler::sortFIFOQueue(std::queue<Job>* sortableQueue)
{
	std::vector<Job> tempSortVector;
	while (!sortableQueue->empty())
	{
		tempSortVector.push_back(sortableQueue->front());
		sortableQueue->pop();
	}

	std::sort(tempSortVector.begin(), tempSortVector.end(), SortByArrivalTime);

	for (Job j : tempSortVector)
	{
		sortableQueue->push(j);
	}
}

void Scheduler::sortSJFQueue(std::queue<Job>* sortableQueue)
{
	std::vector<Job> tempSortVector;
	while (!sortableQueue->empty())
	{
		tempSortVector.push_back(sortableQueue->front());
		sortableQueue->pop();
	}

	std::sort(tempSortVector.begin(), tempSortVector.end(), SortByRunTime);

	for (Job j : tempSortVector)
	{
		sortableQueue->push(j);
	}
}

void Scheduler::checkComplete()
{ //if ((FIFOcomplete == true) && (SJFcomplete == true) && (STTCcomplete == true) && (RR1Complete == true) && (RR2Complete == true))
	if ((FIFOcomplete == true) && (SJFcomplete == true))
		running = false;
	else
		return;
}

