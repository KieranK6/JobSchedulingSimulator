#pragma once
#include <iostream>

class Job
{
public:
	std::string name;
	int runTime, arrivalTime, endTime;
	bool jobComplete;



public:
	Job(std::string name, int arrivalTime, int runTime);
	~Job();

	void jobTick();

};

