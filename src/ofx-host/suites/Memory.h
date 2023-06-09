#pragma once

#include <obs.h>

#include <openfx/ofxCore.h>
#include <openfx/ofxMemory.h>

namespace ofx::suites::v1 {
class MemorySuite {
public:
	static OfxMemorySuiteV1* GetSuiteDesc() {
		m_Suite.memoryAlloc = SFN_OfxMalloc;
		m_Suite.memoryFree = SFN_OfxFree;
		return &m_Suite;
	}

private:
	static OfxStatus SFN_OfxMalloc(void* handle, size_t size, void** data) {
		if (size < 0)
			return kOfxStatErrMemory;

		*data = bmalloc(size);
		if (data == nullptr)
			return kOfxStatErrMemory;

		return kOfxStatOK;
	}

	static OfxStatus SFN_OfxFree(void* data)
	{
		bfree(data);
		return kOfxStatOK;
	}

private:
	static OfxMemorySuiteV1 m_Suite;
};
}
