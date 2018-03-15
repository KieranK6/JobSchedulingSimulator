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
	RROneComplete = false;
	RRTwoComplete = false;

	sortedSJF = false;
	sortedFIFO = false;

	curTickRROne = 0;
	curTickRRTwo = 0;
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
	std::cout << "t \tFIFO \tSJF \tSTTC \tRR1 \tRR2" << std::endl;
	//loop
	while (running)
	{
		Tick(); //Time management
		CheckArrivals(); //Add jobs to Queues

		RunFIFO(); //Simulate FIFO
		RunSJF(); //Simulate SJF
		RunSTTC();//STTC
		RunRROne(5);//RR1(5)
		RunRRTwo(10);//RR2(10)

		printTick();

		checkComplete();
	}
}

void Scheduler::printTick()
{
	std::string FIFOname, SJFname, STTCname, RROneName, RRTwoName;
	if (incompleteFIFOJobQueue.empty()) //First in First out N/A
	{
		FIFOname = "N/A";
	}
	else if (!incompleteFIFOJobQueue.empty()) //First in First out Name
	{
		FIFOname = currentFIFOJob->name;
	}
	if (incompleteSJFJobQueue.empty()) //Shortest Job First N/A
	{
		SJFname = "N/A";
	}
	else if (!incompleteSJFJobQueue.empty()) //Shortest Job First name
	{
		SJFname = currentSJFJob->name;
	}
	if (incompleteSTTCJobQueue.empty()) //Shortest Time to Completion N/A
	{
		STTCname = "N/A";
	}
	else if (!incompleteSTTCJobQueue.empty()) //Shortest Time to Completion name
	{
		STTCname = currentSTTCJob->name;
	}
	if (incompleteRROneJobQueue.empty()) //Round Robin one N/A
	{
		RROneName = "N/A";
	}
	else if (!incompleteRROneJobQueue.empty()) //Round Robin one Name
	{
		RROneName = currentRROneJob->name;
	}
	if (incompleteRRTwoJobQueue.empty()) //Round Robin two N/A
	{
		RRTwoName = "N/A";
	}
	else if (!incompleteSTTCJobQueue.empty()) //Round Robin two name
	{
		RRTwoName = currentRRTwoJob->name;
	}
		
	std::cout << elapsedTick << ": \t" << FIFOname << "\t" << SJFname << "\t" << STTCname << "\t" << RROneName << "\t" << RRTwoName << std::endl;
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
	std::cout << "* " << method << " COMPLETE: " << name << " at tick " << elapsedTick << std::endl;
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

void Scheduler::RunRROne(int sliceTime)
{
	if (incompleteRROneJobQueue.empty() && completeRROneJobList.size() == loadedJobsFromFile.size())
	{
		RROneComplete = true;
		return;
	}

	//x ticks
	if (curTickRROne == sliceTime)
	{
		moveFirsttoBack(&incompleteRROneJobQueue); //Swap
		curTickRROne = 0;
	}

	curTickRROne++;
	
	currentRROneJob = &incompleteRROneJobQueue.front();
	currentRROneJob->jobTick();

	if (currentRROneJob->jobComplete == true)
	{
		printCompleteJob("RROne", currentRROneJob->name);

		currentRROneJob->endTime = elapsedTick;
		completeRROneJobList.push_front(incompleteRROneJobQueue.front());
		incompleteRROneJobQueue.pop();
		curTickRROne = 0;
	}
}

void Scheduler::RunRRTwo(int sliceTime)
{
	if (incompleteRRTwoJobQueue.empty() && completeRRTwoJobList.size() == loadedJobsFromFile.size())
	{
		RRTwoComplete = true;
		return;
	}

	//x ticks
	if (curTickRRTwo == sliceTime)
	{
		moveFirsttoBack(&incompleteRRTwoJobQueue); //Swap
		curTickRRTwo = 0;
	}

	curTickRRTwo++;

	currentRRTwoJob = &incompleteRRTwoJobQueue.front();
	currentRRTwoJob->jobTick();

	if (currentRRTwoJob->jobComplete == true)
	{
		printCompleteJob("RRTwo", currentRRTwoJob->name);

		currentRRTwoJob->endTime = elapsedTick;
		completeRRTwoJobList.push_front(incompleteRRTwoJobQueue.front());
		incompleteRRTwoJobQueue.pop();
		curTickRRTwo = 0;
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
				incompleteSTTCJobQueue.push(j);
				incompleteRROneJobQueue.push(j);
				incompleteRRTwoJobQueue.push(j);

				std::cout << "* ARRIVED: " << j.name << std::endl;
				j.hasArrived = true;
				loadedJobsFromFile.remove(j);
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

void Scheduler::moveFirsttoBack(std::queue<Job>* sortableQueue)
{
	sortableQueue->push(sortableQueue->front());
	sortableQueue->pop();
}

void Scheduler::sortSTTCQueue(std::queue<Job>* sortableQueue)
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
{
	if ((FIFOcomplete == true) && (SJFcomplete == true) && (STTCcomplete == true) && (RROneComplete == true) && (RRTwoComplete == true))
	{
		running = false;
		std::cout << "= SIMULATION COMPLETE" << std::endl;
	}
		
	else
		return;
}

