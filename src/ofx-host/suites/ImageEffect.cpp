#include "ofx-host/suites/ImageEffect.h"

#include "ofx-host/image-effect/ImageEffectInstance.h"

using namespace ofx;
using namespace ofx::suites::v1;

OfxStatus ImageEffectSuite::SFN_getPropertySet(OfxImageEffectHandle imageEffect,
					       OfxPropertySetHandle *propHandle)
{
	if (imageEffect == nullptr)
		return kOfxStatErrBadHandle;

	const auto inst = reinterpret_cast<image_effect::ImageEffectInstance *>(imageEffect);
	*propHandle =
		reinterpret_cast<OfxPropertySetHandle>(&inst->m_PropertySet);

	return kOfxStatOK;
}

OfxStatus ImageEffectSuite::SFN_getParamSet(OfxImageEffectHandle imageEffect,
					    OfxParamSetHandle *paramSet)
{
	if (imageEffect == nullptr)
		return kOfxStatErrBadHandle;

	const auto inst = reinterpret_cast<PluginInstance *>(imageEffect);
	*paramSet = reinterpret_cast<OfxParamSetHandle>(&inst->m_ParameterSet);

	return kOfxStatOK;
}

OfxStatus ImageEffectSuite::SFN_clipDefine(OfxImageEffectHandle imageEffect,
					   const char *name,
					   OfxPropertySetHandle *propertySet)
{
	if (imageEffect == nullptr)
		return kOfxStatErrBadHandle;

	const auto inst = reinterpret_cast<PluginInstance *>(imageEffect);
	inst->
}
