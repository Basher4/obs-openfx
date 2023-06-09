#pragma once

#include <openfx/ofxCore.h>
#include <openfx/ofxImageEffect.h>

namespace ofx::suites::v1 {
class ImageEffectSuite {
public:
	static OfxImageEffectSuiteV1 *GetSuiteDesc()
	{
		m_Suite.getPropertySet = SFN_getPropertySet;
		m_Suite.getParamSet = SFN_getParamSet;
		m_Suite.clipDefine = SFN_clipDefine;
		m_Suite.clipGetHandle = SFN_clipGetHandle;
		m_Suite.clipGetPropertySet = SFN_clipGetPropertySet;
		m_Suite.clipGetImage = SFN_clipGetImage;
		m_Suite.clipReleaseImage = SFN_clipReleaseImage;
		m_Suite.clipGetRegionOfDefinition =
			SFN_clipGetRegionOfDefinition;
		m_Suite.abort = SFN_abort;
		m_Suite.imageMemoryAlloc = SFN_imageMemoryAlloc;
		m_Suite.imageMemoryFree = SFN_imageMemoryFree;
		m_Suite.imageMemoryLock = SFN_imageMemoryLock;
		m_Suite.imageMemoryUnlock = SFN_imageMemoryUnlock;

		return &m_Suite;
	}

private:
	static OfxStatus SFN_getPropertySet(OfxImageEffectHandle imageEffect,
					    OfxPropertySetHandle *propHandle);
	static OfxStatus SFN_getParamSet(OfxImageEffectHandle imageEffect,
					 OfxParamSetHandle *paramSet);
	static OfxStatus SFN_clipDefine(OfxImageEffectHandle imageEffect,
					const char *name,
					OfxPropertySetHandle *propertySet);
	static OfxStatus SFN_clipGetHandle(OfxImageEffectHandle imageEffect,
					   const char *name,
					   OfxImageClipHandle *clip,
					   OfxPropertySetHandle *propertySet);
	static OfxStatus
	SFN_clipGetPropertySet(OfxImageClipHandle clip,
			       OfxPropertySetHandle *propHandle);
	static OfxStatus SFN_clipGetImage(OfxImageClipHandle clip, OfxTime time,
					  const OfxRectD *region,
					  OfxPropertySetHandle *imageHandle);
	static OfxStatus SFN_clipReleaseImage(OfxPropertySetHandle imageHandle);
	static OfxStatus SFN_clipGetRegionOfDefinition(OfxImageClipHandle clip,
						       OfxTime time,
						       OfxRectD *bounds);
	static int SFN_abort(OfxImageEffectHandle imageEffect);

	static OfxStatus
	SFN_imageMemoryAlloc(OfxImageEffectHandle instanceHandle, size_t nBytes,
			     OfxImageMemoryHandle *memoryHandle);
	static OfxStatus SFN_imageMemoryFree(OfxImageMemoryHandle memoryHandle);
	static OfxStatus SFN_imageMemoryLock(OfxImageMemoryHandle memoryHandle,
					     void **returnedPtr);
	static OfxStatus
	SFN_imageMemoryUnlock(OfxImageMemoryHandle memoryHandle);

private:
	static OfxImageEffectSuiteV1 m_Suite;
};
}
