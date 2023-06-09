#include <ofx-host/suites/Multithread.h>
using namespace ofx::suites::v1;

#include <qsemaphore.h>
#include <qthread.h>

#include "logging.h"

thread_local bool t_IsSpawnedByOfx = false;
thread_local unsigned int t_ThreadId = 0;

OfxStatus MultiThreadSuite::SFN_SpawnThread(OfxThreadFunctionV1 func,
					    unsigned int nThreads, void *args)
{
	return OfxStatus();
}

OfxStatus MultiThreadSuite::SFN_GetNumCPUs(unsigned int *nCPUs)
{
	*nCPUs = QThread::idealThreadCount();
	return kOfxStatOK;
}

OfxStatus MultiThreadSuite::SFN_GetThreadIdx(unsigned int *threadIndex)
{
	*threadIndex = t_ThreadId;
	return kOfxStatOK;
}

int MultiThreadSuite::SFN_IsThreadSpawnedByMe(void)
{
	return t_IsSpawnedByOfx;
}

OfxStatus MultiThreadSuite::SFN_MutexCreate(OfxMutexHandle *mutex,
					    int lockCount)
{
	const auto sem = new QSemaphore(lockCount);
	*mutex = reinterpret_cast<OfxMutexHandle>(sem);
	return kOfxStatOK;
}

OfxStatus MultiThreadSuite::SFN_MutexDestroy(const OfxMutexHandle mutex)
{
	if (mutex == nullptr)
		return kOfxStatErrBadHandle;

	const auto sem = reinterpret_cast<QSemaphore *>(mutex);
	if (!sem->available())
		ofx_error(
			"Trying to destroy OFX Mutex that is not available. Plugin kinda fucked.");
	delete sem;

	return kOfxStatOK;
}

OfxStatus MultiThreadSuite::SFN_MutexLock(const OfxMutexHandle mutex)
{
	if (mutex == nullptr)
		return kOfxStatErrBadHandle;

	const auto sem = reinterpret_cast<QSemaphore *>(mutex);
	sem->acquire();

	return kOfxStatOK;
}

OfxStatus MultiThreadSuite::SFN_MutexUnlock(const OfxMutexHandle mutex)
{
	if (mutex == nullptr)
		return kOfxStatErrBadHandle;

	const auto sem = reinterpret_cast<QSemaphore *>(mutex);
	sem->release();

	return kOfxStatOK;
}

OfxStatus MultiThreadSuite::SFN_MutexTryLock(const OfxMutexHandle mutex)
{
	if (mutex == nullptr)
		return kOfxStatErrBadHandle;

	const auto sem = reinterpret_cast<QSemaphore *>(mutex);
	const bool acquired = sem->try_acquire();
	return acquired ? kOfxStatOK : kOfxStatFailed;
}
