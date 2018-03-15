#pragma once
#include <string>

class Job
{
public:
	std::string name;
	int runTime, arrivalTime, endTime;
	bool jobComplete;
	bool hasArrived;



public:
	Job(std::string name, int arrivalTime, int runTime);
	Job();
	~Job();

	bool operator == (const Job& j) const { return name == j.name && runTime == j.runTime && arrivalTime == j.arrivalTime && endTime == j.endTime && jobComplete == j.jobComplete && hasArrived == j.hasArrived; }

	void jobTick();

};

