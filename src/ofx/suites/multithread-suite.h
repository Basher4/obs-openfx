#pragma once

#include <openfx/ofxCore.h>
#include <openfx/ofxMultiThread.h>

namespace ofx::suites::v1 {
class MultiThreadSuite {
public:
	static const OfxMultiThreadSuiteV1 *GetSuiteDesc()
	{
		m_Suite.multiThread = SfnSpawnThread;
		m_Suite.multiThreadNumCPUs = SfnGetNumCpUs;
		m_Suite.multiThreadIndex = SfnGetThreadIdx;
		m_Suite.multiThreadIsSpawnedThread = SfnIsThreadSpawnedByMe;
		m_Suite.mutexCreate = SfnMutexCreate;
		m_Suite.mutexDestroy = SfnMutexDestroy;
		m_Suite.mutexLock = SfnMutexLock;
		m_Suite.mutexUnLock = SfnMutexUnlock;
		m_Suite.mutexTryLock = SfnMutexTryLock;
		return &m_Suite;
	}
	static const char *GetSuiteName() { return kOfxMultiThreadSuite; }

private:
	static OfxStatus SfnSpawnThread(OfxThreadFunctionV1 func,
					 unsigned int nThreads, void *args);
	static OfxStatus SfnGetNumCpUs(unsigned int *nCPUs);
	static OfxStatus SfnGetThreadIdx(unsigned int *threadIndex);
	static int SfnIsThreadSpawnedByMe(void);

	static OfxStatus SfnMutexCreate(OfxMutexHandle *mutex, int lockCount);
	static OfxStatus SfnMutexDestroy(const OfxMutexHandle mutex);
	static OfxStatus SfnMutexLock(const OfxMutexHandle mutex);
	static OfxStatus SfnMutexUnlock(const OfxMutexHandle mutex);
	static OfxStatus SfnMutexTryLock(const OfxMutexHandle mutex);

private:
	inline static OfxMultiThreadSuiteV1 m_Suite;
};
};
