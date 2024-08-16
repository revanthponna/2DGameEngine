#pragma once

#include "Basic.hpp"

class Timer {

	public:

		static float		CalcLastFrameTime_ms	   () noexcept;

	private:

		static long		GetCurrentCPUCounter	   () noexcept;
		static long		GetCPUCounterFrequency_ms  () noexcept;

	private:

		static LARGE_INTEGER vTicks;
		static LARGE_INTEGER vFrequency;
		static long		     vLastFrameStartTick;

};
