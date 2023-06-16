#include "ofx/suites/memory-suite.h"
using namespace ofx;
using namespace ofx::suites::v1;

OfxStatus MemorySuite::SfnOfxMalloc(void *handle, size_t size, void **data)
{
	if (size < 0)
		return kOfxStatErrMemory;

	*data = bmalloc(size);
	if (data == nullptr)
		return kOfxStatErrMemory;

	return kOfxStatOK;
}

OfxStatus MemorySuite::SfnOfxFree(void *data)
{
	bfree(data);
	return kOfxStatOK;
}
