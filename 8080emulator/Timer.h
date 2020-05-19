#pragma once
#include <thread>

class Timer
{
public:
	Timer();

	void start();

	int readTime(); //in milliseconds

	void reset();

	void stop();

private:
	std::thread timerThread;

	int timeElapsed;

	bool isRunning;
};

