#include "CTimer.h"

CTimer::CTimer()
{
	mStart = std::chrono::high_resolution_clock::now();
	mStop = std::chrono::high_resolution_clock::now();
}

bool CTimer::Start()
{
	if (mRunning) return false;
	mStart = std::chrono::high_resolution_clock::now();
	mRunning = true;
	return true;
}

bool CTimer::Stop()
{
	if (!mRunning) return false;
	mStop = std::chrono::high_resolution_clock::now();
	mRunning = false;
	return true;
}

void CTimer::Restart()
{
	mRunning = true;
	mStart = std::chrono::high_resolution_clock::now();
}

double CTimer::GetMiliseconds()
{
	if (mRunning)
	{
		auto Elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - mStart);
		return Elapsed.count();
	}
	else
	{
		auto Elapsed = std::chrono::duration<double, std::milli>(mStop - mStart);
		return Elapsed.count();
	}
}
