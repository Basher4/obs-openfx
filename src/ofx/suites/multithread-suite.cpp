#include <qsemaphore.h>
#include <qthread.h>

#include "logger.h"
#include "ofx/suites/multithread-suite.h"
using namespace ofx::suites::v1;

thread_local bool t_IsSpawnedByOfx = false;
thread_local unsigned int t_ThreadId = 0;

OfxStatus MultiThreadSuite::SfnSpawnThread(OfxThreadFunctionV1 func,
					    unsigned int nThreads, void *args)
{
	return OfxStatus();
}

OfxStatus MultiThreadSuite::SfnGetNumCpUs(unsigned int *nCPUs)
{
	*nCPUs = QThread::idealThreadCount();
	return kOfxStatOK;
}

OfxStatus MultiThreadSuite::SfnGetThreadIdx(unsigned int *threadIndex)
{
	*threadIndex = t_ThreadId;
	return kOfxStatOK;
}

int MultiThreadSuite::SfnIsThreadSpawnedByMe(void)
{
	return t_IsSpawnedByOfx;
}

OfxStatus MultiThreadSuite::SfnMutexCreate(OfxMutexHandle *mutex,
					    int lockCount)
{
	const auto sem = new QSemaphore(lockCount);
	*mutex = reinterpret_cast<OfxMutexHandle>(sem);
	return kOfxStatOK;
}

OfxStatus MultiThreadSuite::SfnMutexDestroy(const OfxMutexHandle mutex)
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

OfxStatus MultiThreadSuite::SfnMutexLock(const OfxMutexHandle mutex)
{
	if (mutex == nullptr)
		return kOfxStatErrBadHandle;

	const auto sem = reinterpret_cast<QSemaphore *>(mutex);
	sem->acquire();

	return kOfxStatOK;
}

OfxStatus MultiThreadSuite::SfnMutexUnlock(const OfxMutexHandle mutex)
{
	if (mutex == nullptr)
		return kOfxStatErrBadHandle;

	const auto sem = reinterpret_cast<QSemaphore *>(mutex);
	sem->release();

	return kOfxStatOK;
}

OfxStatus MultiThreadSuite::SfnMutexTryLock(const OfxMutexHandle mutex)
{
	if (mutex == nullptr)
		return kOfxStatErrBadHandle;

	const auto sem = reinterpret_cast<QSemaphore *>(mutex);
	const bool acquired = sem->try_acquire();
	return acquired ? kOfxStatOK : kOfxStatFailed;
}
