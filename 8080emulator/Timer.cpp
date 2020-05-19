#include "Timer.h"

Timer::Timer()
{
}

void Timer::start() {
	timerThread = std::thread([&]() {
		isRunning = true;
		timeElapsed = 0;
		while (isRunning) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			++timeElapsed;
		}
		});
}

void Timer::reset() {
	timeElapsed = 0;
}

int Timer::readTime() {
	return timeElapsed;
}

void Timer::stop() {
	timerThread.join();
}