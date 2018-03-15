#pragma once
#include <string>

class Job
{
public:
	std::string name;
	int runTime, arrivalTime, endTime, firstrunTime;
	bool jobComplete;
	bool hasArrived;
	bool hasStarted;



public:
	Job(std::string name, int arrivalTime, int runTime);
	Job();
	~Job();

	bool operator == (const Job& j) const { return name == j.name && runTime == j.runTime && arrivalTime == j.arrivalTime && endTime == j.endTime && firstrunTime == j.firstrunTime && jobComplete == j.jobComplete && hasArrived == j.hasArrived; }

	void jobTick();

};

