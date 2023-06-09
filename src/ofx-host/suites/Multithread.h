#pragma once

#include <openfx/ofxCore.h>
#include <openfx/ofxMultiThread.h>

namespace ofx::suites::v1 {
class MultiThreadSuite {
public:
	static const OfxMultiThreadSuiteV1 *GetSuiteDesc()
	{
		m_Suite.multiThread = SFN_SpawnThread;
		m_Suite.multiThreadNumCPUs = SFN_GetNumCPUs;
		m_Suite.multiThreadIndex = SFN_GetThreadIdx;
		m_Suite.multiThreadIsSpawnedThread = SFN_IsThreadSpawnedByMe;
		m_Suite.mutexCreate = SFN_MutexCreate;
		m_Suite.mutexDestroy = SFN_MutexDestroy;
		m_Suite.mutexLock = SFN_MutexLock;
		m_Suite.mutexUnLock = SFN_MutexUnlock;
		m_Suite.mutexTryLock = SFN_MutexTryLock;
		return &m_Suite;
	}

private:
	static OfxStatus SFN_SpawnThread(OfxThreadFunctionV1 func,
					 unsigned int nThreads, void *args);
	static OfxStatus SFN_GetNumCPUs(unsigned int *nCPUs);
	static OfxStatus SFN_GetThreadIdx(unsigned int *threadIndex);
	static int SFN_IsThreadSpawnedByMe(void);

	static OfxStatus SFN_MutexCreate(OfxMutexHandle *mutex, int lockCount);
	static OfxStatus SFN_MutexDestroy(const OfxMutexHandle mutex);
	static OfxStatus SFN_MutexLock(const OfxMutexHandle mutex);
	static OfxStatus SFN_MutexUnlock(const OfxMutexHandle mutex);
	static OfxStatus SFN_MutexTryLock(const OfxMutexHandle mutex);

private:
	static OfxMultiThreadSuiteV1 m_Suite;
};
};
