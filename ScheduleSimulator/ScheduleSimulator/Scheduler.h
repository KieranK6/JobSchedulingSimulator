#pragma once
#include <list>
#include "Job.h"
#include <queue>
#include <iostream>
#include <algorithm>

class Scheduler
{
public:
	std::queue<Job> incompleteFIFOJobQueue;
	std::queue<Job> incompleteSJFJobQueue;
	std::queue<Job> incompleteSTTCJobQueue;
	std::queue<Job> incompleteRROneJobQueue;
	std::queue<Job> incompleteRRTwoJobQueue;

	std::list<Job> completeFIFOJobList;
	std::list<Job> completeSJFJobList;
	std::list<Job> completeSTTCJobList;
	std::list<Job> completeRROneJobList;
	std::list<Job> completeRRTwoJobList;

	bool isComplete;
	int elapsedTick;

	bool running;
	bool FIFOcomplete;
	bool SJFcomplete;
	bool STTCcomplete;
	bool RROneComplete;
	bool RRTwoComplete;

	//int ArrivedCount;
	int totalJobCount;
	

	//bool SortByArrivalTime(Job i, Job j);
	//bool SortByRunTime(Job i, Job j);

	bool sortedSJF;
	bool sortedFIFO;
	bool sortedSTTC;
	bool sortedRROne;
	bool sortedRRTwo;

	std::list<Job> loadedJobsFromFile;

	Job* currentFIFOJob;
	Job* currentSJFJob;
	Job* currentSTTCJob;
	Job currentRR1Job;
	Job currentRR2Job;

	

public:
	Scheduler();
	~Scheduler();

	void CheckArrivals();
	void Arrive(Job job);
	void loadJobs();
	void Tick();
	void RunFIFO();
	void RunSJF();
	void RunSTTC();
	void Simulate();
	//void sort();

	void printTick();
	void checkComplete();
	void printCompleteJob(std::string method, std::string name);

	//void sortQueue(std::queue<Job>* sortableQueue, bool sortBy); //Passed bool causing error C2064
	void sortFIFOQueue(std::queue<Job>* sortableQueue);
	void sortSJFQueue(std::queue<Job>* sortableQueue);
	void sortSTTCQueue(std::queue<Job>* sortableQueue);
};

