#pragma once
#include <chrono>

class CTimer
{
public:
	CTimer();

	bool Start();
	bool Stop();
	void Restart();
	double GetMiliseconds();

private:
	bool mRunning = false;

#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> mStart;
	std::chrono::time_point<std::chrono::steady_clock> mStop;
#else
	std::chrono::time_point<std::chrono::system_clock> mStart;
	std::chrono::time_point<std::chrono::system_clock> mStop;
#endif
};