#pragma once

#include "openfx/ofxCore.h"
#include "openfx/ofxImageEffect.h"

namespace ofx::suites::v1 {
class ImageEffectSuite {
public:
	static OfxImageEffectSuiteV1 *GetSuiteDesc();
	static const char *GetSuiteName() { return kOfxImageEffectSuite; }

private:
	ImageEffectSuite() = default;

private:
	static OfxStatus SfnGetPropertySet(OfxImageEffectHandle imageEffect,
					   OfxPropertySetHandle *propHandle);
	static OfxStatus SfnGetParamSet(OfxImageEffectHandle imageEffect,
					OfxParamSetHandle *paramSet);
	static OfxStatus SfnClipDefine(OfxImageEffectHandle imageEffect,
				       const char *name,
				       OfxPropertySetHandle *propertySet);
	static OfxStatus SfnClipGetHandle(OfxImageEffectHandle imageEffect,
					  const char *name,
					  OfxImageClipHandle *clip,
					  OfxPropertySetHandle *propertySet);
	static OfxStatus
	SfnClipGetPropertySet(OfxImageClipHandle clip,
			      OfxPropertySetHandle *propHandle);
	static OfxStatus SfnClipGetImage(OfxImageClipHandle clip, OfxTime time,
					 const OfxRectD *region,
					 OfxPropertySetHandle *imageHandle);
	static OfxStatus SfnClipReleaseImage(OfxPropertySetHandle imageHandle);
	static OfxStatus SfnClipGetRegionOfDefinition(OfxImageClipHandle clip,
						      OfxTime time,
						      OfxRectD *bounds);
	static int SfnAbort(OfxImageEffectHandle imageEffect);

	static OfxStatus
	SfnImageMemoryAlloc(OfxImageEffectHandle instanceHandle, size_t nBytes,
			    OfxImageMemoryHandle *memoryHandle);
	static OfxStatus SfnImageMemoryFree(OfxImageMemoryHandle memoryHandle);
	static OfxStatus SfnImageMemoryLock(OfxImageMemoryHandle memoryHandle,
					    void **returnedPtr);
	static OfxStatus
	SfnImageMemoryUnlock(OfxImageMemoryHandle memoryHandle);

private:
	inline static OfxImageEffectSuiteV1 suite_;
};
}
