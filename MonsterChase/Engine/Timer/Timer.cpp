
#include "Timer.hpp"

long Timer::vLastFrameStartTick = 0;
LARGE_INTEGER Timer::vTicks{};
LARGE_INTEGER Timer::vFrequency{};

long
Timer::GetCurrentCPUCounter() noexcept
{
	bool issuccess;

	issuccess = QueryPerformanceCounter(&vTicks);

	assert(issuccess);

	return ((long)vTicks.QuadPart);
}

long
Timer::GetCPUCounterFrequency_ms() noexcept
{
	bool issuccess;

	issuccess = QueryPerformanceFrequency(&vFrequency);

	assert(issuccess);

	return (long)(vFrequency.QuadPart / (long)1000.0);
}

float
Timer::CalcLastFrameTime_ms() noexcept
{
	float lastFrameTime_ms;
	long  currTick;
	long  elapsedTicks;

	currTick = GetCurrentCPUCounter();

	if (vLastFrameStartTick) {

		elapsedTicks = (currTick - vLastFrameStartTick);

		lastFrameTime_ms = (float)(elapsedTicks / GetCPUCounterFrequency_ms());
	}
	else {
		lastFrameTime_ms = (float) 13.3;	// assume a 60 Hz frame for the first call
	}

	vLastFrameStartTick = currTick;

	return lastFrameTime_ms;

}