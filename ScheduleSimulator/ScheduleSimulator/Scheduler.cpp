#include "Scheduler.h"

Scheduler::Scheduler()
{
	elapsedTick = 0;
	isComplete = 0;

	totalJobCount = 0;

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


bool SortByArrivalTime(Job i, Job j) { return i.arrivalTime < j.arrivalTime; }
bool SortByRunTime(Job i, Job j) { return i.runTime < j.runTime; }

//bool operator==(Job lhs, int rhs) { return lhs.arrivalTime == rhs; }

void Scheduler::Tick()
{
	elapsedTick++;
}

void Scheduler::Simulate()
{
	//loadInFiles
	//loadJobs();

	totalJobCount = loadedJobsFromFile.size();

	//print t FIFO SJF STTC RR1 RR2
	std::cout << "t \tFIFO \tSJF" << std::endl;
	//loop
	while (running)
	{
		Tick(); //Time management
		CheckArrivals(); //Add jobs to Queues

		RunFIFO(); //Simulate FIFO
		RunSJF(); //Simulate SJF
		//STTC
		//RR1(5)
		//RR2(10)

		printTick();

		checkComplete();
	}
}

void Scheduler::printTick()
{
	std::string FIFOname, SJFname;
	if (incompleteFIFOJobQueue.empty())
	{
		FIFOname = "Not Available";
	}
	else if (!incompleteFIFOJobQueue.empty())
	{
		FIFOname = currentFIFOJob->name;
	}
	if (incompleteSJFJobQueue.empty())
	{
		SJFname = "Not Available";
	}
	else if (!incompleteSJFJobQueue.empty())
	{
		SJFname = currentSJFJob->name;
	}
		
	std::cout << elapsedTick << ": \t" << FIFOname << "\t" << SJFname << std::endl;
}

void Scheduler::RunFIFO()
{
	if (incompleteFIFOJobQueue.empty() && completeFIFOJobList.size() == loadedJobsFromFile.size())
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

		currentFIFOJob = &incompleteFIFOJobQueue.front();
		currentFIFOJob->jobTick();

		if (currentFIFOJob->jobComplete == true)
		{
			printCompleteJob("FIFO", currentFIFOJob->name);

			currentFIFOJob->endTime = elapsedTick;
			completeFIFOJobList.push_front(incompleteFIFOJobQueue.front());
			incompleteFIFOJobQueue.pop();
		}
	}
}

void Scheduler::printCompleteJob(std::string method, std::string name)
{
	std::cout << method << " COMPLETE: " << name << " at tick " << elapsedTick << std::endl;
}


void Scheduler::RunSJF()
{

	if (incompleteSJFJobQueue.empty() && completeSJFJobList.size() == loadedJobsFromFile.size())
	{
		SJFcomplete = true;
		return;
	}
	else
	{
		if (!sortedSJF)
		{
			sortSJFQueue(&incompleteSJFJobQueue);
			sortedSJF = true;
		}

		currentSJFJob = &incompleteSJFJobQueue.front();
		currentSJFJob->jobTick();

		if (currentSJFJob->jobComplete == true)
		{
			printCompleteJob("SJF", currentSJFJob->name);

			currentSJFJob->endTime = elapsedTick;
			completeSJFJobList.push_front(incompleteSJFJobQueue.front());
			incompleteSJFJobQueue.pop();
		}
	}
}

void Scheduler::RunSTTC()
{

	if (incompleteSTTCJobQueue.empty() && completeSTTCJobList.size() == loadedJobsFromFile.size())
	{
		STTCcomplete = true;
		return;
	}
	else
	{
		sortSTTCQueue(&incompleteSTTCJobQueue);

		currentSTTCJob = &incompleteSTTCJobQueue.front();
		currentSTTCJob->jobTick();

		if (currentSTTCJob->jobComplete == true)
		{
			printCompleteJob("STTC", currentSTTCJob->name);

			currentSTTCJob->endTime = elapsedTick;
			completeSTTCJobList.push_front(incompleteSTTCJobQueue.front());
			incompleteSTTCJobQueue.pop();
		}
	}
}

void Scheduler::Arrive(Job j)
{
	incompleteFIFOJobQueue.push(j);
	incompleteSJFJobQueue.push(j);
}
void Scheduler::CheckArrivals()
{
	//check if loaded list is not empty
	if (!loadedJobsFromFile.empty())
	{
		for (Job j : loadedJobsFromFile)
		{
			if ((j.arrivalTime == elapsedTick) && j.hasArrived == false)
			{ //if job arrives at current tick remove from loaded list and add to each Queue
				incompleteFIFOJobQueue.push(j);
				incompleteSJFJobQueue.push(j);
				//incompleteSTTCJobQueue.push(j);
				//incompleteRR1JobQueue.push(loadedJobsFromFile[i]);
				//incompleteRR2JobQueue.push(loadedJobsFromFile[i]);

				std::cout << "* ARRIVED: " << j.name << std::endl;
				j.hasArrived = true;
				loadedJobsFromFile.remove(j);
				//loadedJobsFromFile.resize(loadedJobsFromFile.size()-1);
			}
		}
	}
}

//void Scheduler::sortQueue(std::queue<Job>* sortableQueue, bool sortBy)  //Wont take passed bool, giving error C2064, workaround is two functions below
//{
//	std::vector<Job> tempSortVector;
//	while (!sortableQueue->empty())
//	{
//		tempSortVector.push_back(sortableQueue->front());
//		sortableQueue->pop();
//	}
//
//	std::sort(tempSortVector.begin(), tempSortVector.end(), sortBy);
//
//	for (Job j : tempSortVector)
//	{
//		sortableQueue->push(j);
//	}
//}

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
	{
		running = false;
		std::cout << "= SIMULATION COMPLETE" << std::endl;
	}
		
	else
		return;
}

