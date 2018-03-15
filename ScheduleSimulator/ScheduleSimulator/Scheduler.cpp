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

	FIFOAvgTurnaround = 0;
	SJFAvgTurnaround = 0;
	STTCAvgTurnaround = 0;
	RROneAvgTurnaround = 0;
	RRTwoAvgTurnaround = 0;

	FIFOAvgResponce = 0;
	SJFAvgResponce = 0;
	STTCAvgResponce = 0;
	RROneAvgResponce = 0;
	RRTwoAvgResponce = 0;
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


	CalculateStats();
	PrintStats();
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
		if (currentFIFOJob->hasStarted == false)
		{
			currentFIFOJob->firstrunTime = elapsedTick;
			currentFIFOJob->hasStarted = true;
		}	
		currentFIFOJob->jobTick();

		if (currentFIFOJob->jobComplete == true)
		{ //Check if first time being simulated, if it is store firstRunTime
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
		if (currentSJFJob->hasStarted == false)
		{ //Check if first time being simulated, if it is store firstRunTime
			currentSJFJob->firstrunTime = elapsedTick;
			currentSJFJob->hasStarted = true;
		}
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
		if (currentSTTCJob->hasStarted == false)
		{ //Check if first time being simulated, if it is store firstRunTime
			currentSTTCJob->firstrunTime = elapsedTick;
			currentSTTCJob->hasStarted = true;
		}
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
	if (currentRROneJob->hasStarted == false)
	{ //Check if first time being simulated, if it is store firstRunTime
		currentRROneJob->firstrunTime = elapsedTick;
		currentRROneJob->hasStarted = true;
	}
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
	if (currentRRTwoJob->hasStarted == false)
	{ //Check if first time being simulated, if it is store firstRunTime
		currentRRTwoJob->firstrunTime = elapsedTick;
		currentRRTwoJob->hasStarted = true;
	}
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

void Scheduler::CalculateStats()
{ //Turnaround = completedTime - arrivalTime   Response = firstrun - arrival
	// FIFO stats
	//Average turnaround	Average Responce
	if (!completeFIFOJobList.empty())
	{
		for (Job j : completeFIFOJobList)
		{
			std::map<std::string, int> turnaroundTimeMap;
			std::map<std::string, int> responceTimeMap;

			int turnaroundTime = CalculateTurnaround(j.arrivalTime, j.endTime);
			int responceTime = CalculateResponce(j.arrivalTime, j.firstrunTime);

			FIFOResponceTimes.push_back(responceTime);
			FIFOTurnaroundTimes.push_back(turnaroundTime);

			//turnaroundTimeMap.emplace("FIFO", turnaroundTime);
			//responceTimeMap.emplace("FIFO", responceTime);

			turnaroundTimeMap.emplace(j.name, turnaroundTime);
			responceTimeMap.emplace(j.name, responceTime);
			FIFOjobsTurnaround.push_back(turnaroundTimeMap);
			FIFOjobsResponce.push_back(responceTimeMap);
		}
		FIFOAvgResponce = CalculateAvgTime(FIFOResponceTimes);
		FIFOAvgTurnaround = CalculateAvgTime(FIFOTurnaroundTimes);
	}
	

	// SJF stats
	//Average turnaround	Average Responce
	if (!completeSJFJobList.empty())
	{
		for (Job j : completeSJFJobList)
		{
			std::map<std::string, int> turnaroundTimeMap;
			std::map<std::string, int> responceTimeMap;

			int turnaroundTime = CalculateTurnaround(j.arrivalTime, j.endTime);
			int responceTime = CalculateResponce(j.arrivalTime, j.firstrunTime);

			SJFResponceTimes.push_back(responceTime);
			SJFTurnaroundTimes.push_back(turnaroundTime);

			//turnaroundTimeMap.emplace("SJF", turnaroundTime);
			//responceTimeMap.emplace("SJF", responceTime);

			turnAroundTimes.emplace(j.name, turnaroundTimeMap);
			responceTimes.emplace(j.name, responceTimeMap);
		}
		SJFAvgResponce = CalculateAvgTime(SJFResponceTimes);
		SJFAvgTurnaround = CalculateAvgTime(SJFTurnaroundTimes);
	}

	// STTC stats
	//Average turnaround	Average Responce
	if (!completeSTTCJobList.empty())
	{
		for (Job j : completeSTTCJobList)
		{
			std::map<std::string, int> turnaroundTimeMap;
			std::map<std::string, int> responceTimeMap;

			int turnaroundTime = CalculateTurnaround(j.arrivalTime, j.endTime);
			int responceTime = CalculateResponce(j.arrivalTime, j.firstrunTime);

			STTCResponceTimes.push_back(responceTime);
			STTCTurnaroundTimes.push_back(turnaroundTime);

			turnaroundTimeMap.emplace("STTC", turnaroundTime);
			responceTimeMap.emplace("STTC", responceTime);

			turnAroundTimes.emplace(j.name, turnaroundTimeMap);
			responceTimes.emplace(j.name, responceTimeMap);
		}
		STTCAvgResponce = CalculateAvgTime(STTCResponceTimes);
		STTCAvgTurnaround = CalculateAvgTime(STTCTurnaroundTimes);
	}

	// RROne stats
	//Average turnaround	Average Responce
	if (!completeRROneJobList.empty())
	{
		for (Job j : completeRROneJobList)
		{
			std::map<std::string, int> turnaroundTimeMap;
			std::map<std::string, int> responceTimeMap;

			int turnaroundTime = CalculateTurnaround(j.arrivalTime, j.endTime);
			int responceTime = CalculateResponce(j.arrivalTime, j.firstrunTime);

			RROneResponceTimes.push_back(responceTime);
			RROneTurnaroundTimes.push_back(turnaroundTime);

			turnaroundTimeMap.emplace("RROne", turnaroundTime);
			responceTimeMap.emplace("RROne", responceTime);

			turnAroundTimes.emplace(j.name, turnaroundTimeMap);
			responceTimes.emplace(j.name, responceTimeMap);
		}
		RROneAvgResponce = CalculateAvgTime(RROneResponceTimes);
		RROneAvgTurnaround = CalculateAvgTime(RROneTurnaroundTimes);
	}

	// RRTwo stats
	//Average turnaround	Average Responce
	if (!completeRRTwoJobList.empty())
	{
		for (Job j : completeRRTwoJobList)
		{
			std::map<std::string, int> turnaroundTimeMap;
			std::map<std::string, int> responceTimeMap;

			int turnaroundTime = CalculateTurnaround(j.arrivalTime, j.endTime);
			int responceTime = CalculateResponce(j.arrivalTime, j.firstrunTime);

			RRTwoResponceTimes.push_back(responceTime);
			RRTwoTurnaroundTimes.push_back(turnaroundTime);

			turnaroundTimeMap.emplace("RRTwo", turnaroundTime);
			responceTimeMap.emplace("RRTwo", responceTime);

			turnAroundTimes.emplace(j.name, turnaroundTimeMap);
			responceTimes.emplace(j.name, responceTimeMap);
		}
		RRTwoAvgResponce = CalculateAvgTime(RRTwoResponceTimes);
		RRTwoAvgTurnaround = CalculateAvgTime(RRTwoTurnaroundTimes);
	}
}

void Scheduler::addJobsToDictionary()
{

}

int Scheduler::CalculateTurnaround(int arrival, int end)
{ //return ticks between arrival and end
	return end - arrival;
}
int Scheduler::CalculateResponce(int arrival, int firstrun)
{ //return ticks between first arrival and first simulation
	return firstrun - arrival;
}
double Scheduler::CalculateAvgTime(std::vector<int> turnaroundTimes)
{
	double cumulativeTurnTime = 0;
	int count = 0;
	for (double time : turnaroundTimes)
	{
		cumulativeTurnTime += time;
		count++;
	}
	return cumulativeTurnTime / count;
}


void Scheduler::PrintStats()
{ //std::cout << "" << std::endl;
	///PER JOB STATS
	std::cout << std::endl;
	std::cout << "# \tJob \tFIFO \tSJF \tSTTC \tRR1 \tRR2" << std::endl;
	//Turnaround
	//std::cout << "T" << "\tJobOne" << "\t10.5"  << std::endl;
	std::cout << "\t Per Job stats not Implemented yet" << std::endl;
	


	///Avg Job Stats
	std::cout << std::endl;
	std::cout << "# \tScheduler \tAVG_Turnaround \tAVG_Responce" << std::endl;
	
	std::cout << "@" << "\tFIFO" << "\t\t" << FIFOAvgTurnaround << "\t\t" << FIFOAvgResponce << std::endl;
	std::cout << "@" << "\tSJF" << "\t\t" << SJFAvgTurnaround << "\t\t" << SJFAvgResponce << std::endl;
	std::cout << "@" << "\tSTTC" << "\t\t" << STTCAvgTurnaround << "\t\t" << STTCAvgResponce << std::endl;
	std::cout << "@" << "\tRROne" << "\t\t" << RROneAvgTurnaround << "\t\t" << RROneAvgResponce << std::endl;
	std::cout << "@" << "\tRRTwo" << "\t\t" << RRTwoAvgTurnaround << "\t\t" << RRTwoAvgResponce << std::endl;

	std::cout << "= Aggregate stats complete" << std::endl;
	

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

