#include "ofx-host/suites/ImageEffect.h"

#include "ofx-host/image-effect/ImageEffectBase.h"
#include "ofx-host/image-effect/ImageEffectInstance.h"
#include "ofx-host/image-effect/ImageEffectPlugin.h"

using namespace ofx;
using namespace ofx::suites::v1;

OfxStatus ImageEffectSuite::SFN_getPropertySet(OfxImageEffectHandle imageEffect,
					       OfxPropertySetHandle *propHandle)
{
	if (imageEffect == nullptr)
		return kOfxStatErrBadHandle;

	const auto inst =
		reinterpret_cast<image_effect::ImageEffectBase *>(imageEffect);
	*propHandle = inst->GetPropertySet();

	return kOfxStatOK;
}

OfxStatus ImageEffectSuite::SFN_getParamSet(OfxImageEffectHandle imageEffect,
					    OfxParamSetHandle *paramSet)
{
	if (imageEffect == nullptr)
		return kOfxStatErrBadHandle;

	const auto inst =
		reinterpret_cast<image_effect::ImageEffectBase *>(imageEffect);
	*paramSet = inst->GetParameterSet();

	return kOfxStatOK;
}

OfxStatus ImageEffectSuite::SFN_clipDefine(OfxImageEffectHandle imageEffect,
					   const char *name,
					   OfxPropertySetHandle *propertySet)
{
	if (imageEffect == nullptr)
		return kOfxStatErrBadHandle;
	
	return kOfxStatErrFatal;
}
