#pragma once
#include <list>
#include "Job.h"
#include <queue>
#include <iostream>
#include <algorithm>
#include <map>

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

	std::vector<int> FIFOTurnaroundTimes;
	std::vector<int> SJFTurnaroundTimes;
	std::vector<int> STTCTurnaroundTimes;
	std::vector<int> RROneTurnaroundTimes;
	std::vector<int> RRTwoTurnaroundTimes;

	std::vector<int> FIFOResponceTimes;
	std::vector<int> SJFResponceTimes;
	std::vector<int> STTCResponceTimes;
	std::vector<int> RROneResponceTimes;
	std::vector<int> RRTwoResponceTimes;

	
	std::map<std::string, std::map<std::string, int>> turnAroundTimes;
	std::map<std::string, std::map<std::string, int>> responceTimes;

	std::vector < std::map<std::string, int>> FIFOjobsTurnaround;
	std::vector < std::map<std::string, int>> FIFOjobsResponce;
	std::vector < std::map<std::string, int>> SJFjobsTurnaround;
	std::vector < std::map<std::string, int>> SJFjobsResponce;
	std::vector < std::map<std::string, int>> STTCjobsTurnaround;
	std::vector < std::map<std::string, int>> STTCjobsResponce;
	std::vector < std::map<std::string, int>> RROnejobsTurnaround;
	std::vector < std::map<std::string, int>> RROnejobsResponce;
	std::vector < std::map<std::string, int>> RRTwojobsTurnaround;
	std::vector < std::map<std::string, int>> RRTwojobsResponce;

	double FIFOAvgTurnaround;
	double SJFAvgTurnaround;
	double STTCAvgTurnaround;
	double RROneAvgTurnaround;
	double RRTwoAvgTurnaround;

	double FIFOAvgResponce;
	double SJFAvgResponce;
	double STTCAvgResponce;
	double RROneAvgResponce;
	double RRTwoAvgResponce;

	bool isComplete;
	int elapsedTick;
	int curTickRROne;
	int curTickRRTwo;

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
	Job* currentRROneJob;
	Job* currentRRTwoJob;

	

public:
	Scheduler();
	~Scheduler();

	void CheckArrivals();
	void Arrive(Job job);
	void LoadJobs();
	void Tick();
	void RunFIFO();
	void RunSJF();
	void RunSTTC();
	void RunRROne(int sliceTime);
	void RunRRTwo(int sliceTime);
	void Simulate();

	void CalculateStats();
	int CalculateTurnaround(int arrival, int end);
	int CalculateResponce(int arrival, int firstrun);
	double CalculateAvgTime(std::vector<int> times);
	double CalculateAvgResponce(std::vector<int> responceTimes);
	void PrintStats();

	void addJobsToDictionary();


	//void sort();

	void printTick();
	void checkComplete();
	void printCompleteJob(std::string method, std::string name);

	//void sortQueue(std::queue<Job>* sortableQueue, bool sortBy); //Passed bool causing error C2064
	void sortFIFOQueue(std::queue<Job>* sortableQueue);
	void sortSJFQueue(std::queue<Job>* sortableQueue);
	void sortSTTCQueue(std::queue<Job>* sortableQueue);

	void moveFirsttoBack(std::queue<Job>* sortableQueue);
};

