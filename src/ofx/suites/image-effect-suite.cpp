#include "ofx/image-effect/clip.h"
#include "ofx/image-effect/effect.h"
#include "ofx/suites/image-effect-suite.h"
using namespace ofx::suites::v1;

OfxImageEffectSuiteV1 *ImageEffectSuite::GetSuiteDesc()
{
	suite_.getPropertySet = SfnGetPropertySet;
	suite_.getParamSet = SfnGetParamSet;
	suite_.clipDefine = SfnClipDefine;
	suite_.clipGetHandle = SfnClipGetHandle;
	suite_.clipGetPropertySet = SfnClipGetPropertySet;
	suite_.clipGetImage = SfnClipGetImage;
	suite_.clipReleaseImage = SfnClipReleaseImage;
	suite_.clipGetRegionOfDefinition = SfnClipGetRegionOfDefinition;
	suite_.abort = SfnAbort;
	suite_.imageMemoryAlloc = SfnImageMemoryAlloc;
	suite_.imageMemoryFree = SfnImageMemoryFree;
	suite_.imageMemoryLock = SfnImageMemoryLock;
	suite_.imageMemoryUnlock = SfnImageMemoryUnlock;
	return &suite_;
}

OfxStatus ImageEffectSuite::SfnGetPropertySet(OfxImageEffectHandle imageEffect,
					      OfxPropertySetHandle *propHandle)
{
	auto *plugin = reinterpret_cast<image_effect::Effect *>(imageEffect);
	*propHandle = plugin->GetPropertySet();
	return kOfxStatOK;
}

OfxStatus ImageEffectSuite::SfnGetParamSet(OfxImageEffectHandle imageEffect,
					   OfxParamSetHandle *paramSet)
{
	auto *plugin = reinterpret_cast<image_effect::Effect *>(imageEffect);
	*paramSet = plugin->GetParameterSet();
	return kOfxStatOK;
}

OfxStatus ImageEffectSuite::SfnClipDefine(OfxImageEffectHandle imageEffect,
					  const char *name,
					  OfxPropertySetHandle *propertySet)
{
	auto *plugin = reinterpret_cast<image_effect::Effect *>(imageEffect);
	image_effect::Clip *clip = plugin->CreateClip(name);
	*propertySet = clip->GetPropertySet();
	return kOfxStatOK;
}

OfxStatus ImageEffectSuite::SfnClipGetHandle(OfxImageEffectHandle imageEffect,
					     const char *name,
					     OfxImageClipHandle *clip,
					     OfxPropertySetHandle *propertySet)
{
	const auto *plugin =
		reinterpret_cast<image_effect::Effect *>(imageEffect);
	auto ofx_clip = plugin->GetClipByName(name);

	if (!ofx_clip.has_value())
		return kOfxStatErrValue;

	if (ofx_clip != nullptr)
		*clip = reinterpret_cast<OfxImageClipHandle>(ofx_clip.value());
	if (propertySet != nullptr)
		*propertySet = ofx_clip.value()->GetPropertySet();
	return kOfxStatOK;
}

OfxStatus
ImageEffectSuite::SfnClipGetPropertySet(OfxImageClipHandle clip,
					OfxPropertySetHandle *propHandle)
{
	auto *ofx_clip = reinterpret_cast<image_effect::Clip *>(clip);
	if (propHandle != nullptr)
		*propHandle = ofx_clip->GetPropertySet();
	return kOfxStatOK;
}

OfxStatus ImageEffectSuite::SfnClipGetImage(OfxImageClipHandle clip,
					    OfxTime time,
					    const OfxRectD *region,
					    OfxPropertySetHandle *imageHandle)
{
	return kOfxStatErrFatal;
}

OfxStatus
ImageEffectSuite::SfnClipReleaseImage(OfxPropertySetHandle imageHandle)
{
	return kOfxStatErrFatal;
}

OfxStatus
ImageEffectSuite::SfnClipGetRegionOfDefinition(OfxImageClipHandle clip,
					       OfxTime time, OfxRectD *bounds)
{
	return kOfxStatErrFatal;
}

int ImageEffectSuite::SfnAbort(OfxImageEffectHandle imageEffect)
{
	return kOfxStatErrFatal;
}

OfxStatus
ImageEffectSuite::SfnImageMemoryAlloc(OfxImageEffectHandle instanceHandle,
				      size_t nBytes,
				      OfxImageMemoryHandle *memoryHandle)
{
	return kOfxStatErrFatal;
}

OfxStatus
ImageEffectSuite::SfnImageMemoryFree(OfxImageMemoryHandle memoryHandle)
{
	return kOfxStatErrFatal;
}

OfxStatus
ImageEffectSuite::SfnImageMemoryLock(OfxImageMemoryHandle memoryHandle,
				     void **returnedPtr)
{
	return kOfxStatErrFatal;
}

OfxStatus
ImageEffectSuite::SfnImageMemoryUnlock(OfxImageMemoryHandle memoryHandle)
{
	return kOfxStatErrFatal;
}
