#include "JobRunner.h"

#include <assert.h>

#include "JobSystem.h"

namespace Engine
{
	namespace JobSystem
	{

		DWORD WINAPI JobRunner(void* i_pThreadInput)
		{
			assert(i_pThreadInput);

			JobRunnerInput* pInput = reinterpret_cast<JobRunnerInput*>(i_pThreadInput);
			assert(pInput->m_pQueue);

			bool bDone = false;

			do
			{
				QueuedJob* pJob = pInput->m_pQueue->GetWhenAvailable();
				if (pJob)
				{
					pInput->m_pQueue->StartingJob(pJob);
					pJob->Function();

					pInput->m_pQueue->FinishedJob(pJob);
				}

				bDone = pInput->m_pQueue->ShutdownRequested();

			} while (bDone == false);

#ifdef _DEBUG
#endif
			return 0;
		}

	} // namespace JobSystem
} // namespace Engine
