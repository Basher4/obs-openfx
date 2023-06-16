#pragma once

#include <obs.h>

#include <openfx/ofxCore.h>
#include <openfx/ofxMemory.h>

namespace ofx::suites::v1 {
class MemorySuite {
public:
	static OfxMemorySuiteV1 *GetSuiteDesc()
	{
		suite_.memoryAlloc = SfnOfxMalloc;
		suite_.memoryFree = SfnOfxFree;
		return &suite_;
	}
	static const char *GetSuiteName() { return kOfxMemorySuite; }

private:
	static OfxStatus SfnOfxMalloc(void *handle, size_t size, void **data);
	static OfxStatus SfnOfxFree(void *data);

private:
	inline static OfxMemorySuiteV1 suite_;
};
}
