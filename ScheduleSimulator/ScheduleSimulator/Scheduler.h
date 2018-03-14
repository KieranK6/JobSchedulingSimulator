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
	std::queue<Job> incompleteRR1JobQueue;
	std::queue<Job> incompleteRR2JobQueue;

	std::list<Job> completeFIFOJobList;
	std::list<Job> completeSJFJobList;
	std::list<Job> completeSTTCJobList;
	std::list<Job> completeRR1JobList;
	std::list<Job> completeRR2JobList;

	bool isComplete;
	int elapsedTick;

	bool running;
	bool FIFOcomplete;
	bool SJFcomplete;
	bool STTCcomplete;
	bool RR1Complete;
	bool RR2Complete;


	

	//bool SortByArrivalTime(Job i, Job j);
	//bool SortByRunTime(Job i, Job j);

	bool sortedSJF;
	bool sortedFIFO;

	std::list<Job> loadedJobsFromFile;

	Job currentFIFOJob;
	Job currentSJFJob;
	Job currentSTTCJob;
	Job currentRR1Job;
	Job currentRR2Job;

	

public:
	Scheduler();
	~Scheduler();

	void Arrive(Job job);
	void Tick();
	void RunFIFO();
	void RunSJF();
	void Simulate();
	//void sort();

	void printTick();
	void checkComplete();

	//void sortQueue(std::queue<Job>* sortableQueue, bool sortBy);
	void sortFIFOQueue(std::queue<Job>* sortableQueue);
	void sortSJFQueue(std::queue<Job>* sortableQueue);
};

