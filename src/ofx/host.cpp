#include "ofx/host.h"
#include "ofx/suites/image-effect-suite.h"
#include "ofx/suites/memory-suite.h"
#include "ofx/suites/property-suite.h"
#include "ofx/suites/parameter-suite.h"
#include "ofx/suites/multithread-suite.h"
#include "openfx/ofxOpenGLRender.h"
#include "openfx/ofxParametricParam.h"
using namespace ofx;

Host::Host() {}

void Host::CaptureNewFrame(obs_source_frame *frame)
{
	frame_queue_.enqueue(frame);
}

std::optional<obs_source_frame *> Host::RenderFrame(double time)
{
	if (frame_queue_.isEmpty())
		return std::nullopt;
	return frame_queue_.dequeue();
}

OfxHost *Host::GetOfxStruct()
{
	return &ofx_host_;
}

const void *Host::SfnFetchSuite(OfxPropertySetHandle host,
				const char *suiteName, int suiteVersion)
{
	using namespace suites::v1;

#define CHECK_SUITE(type)                                               \
	do {                                                            \
		if (strcmp(suiteName, type::GetSuiteName()) == 0)       \
			return suiteVersion == 1 ? type::GetSuiteDesc() \
						 : nullptr;             \
	} while (0)

	CHECK_SUITE(PropertySuite);
	CHECK_SUITE(ParameterSuite);
	CHECK_SUITE(MultiThreadSuite);
	CHECK_SUITE(MemorySuite);
	CHECK_SUITE(ImageEffectSuite);

#undef CHECK_SUITE

	return nullptr;
}

void Host::InitializeHostPropertySet()
{
	property_set_.Set(kOfxPropName, "github.basher4.OfxHost");
	property_set_.Set(kOfxPropLabel, "OBS OpenFX Host");
	property_set_.Set(kOfxPropVersion, {0, 0, 1});
	property_set_.Set(kOfxPropVersionLabel, "0.0.1");
	property_set_.Set(kOfxImageEffectHostPropIsBackground, 0);
	property_set_.Set(kOfxImageEffectPropSupportsOverlays, 0);
	property_set_.Set(kOfxImageEffectPropSupportsMultiResolution,
				  0);
	property_set_.Set(kOfxImageEffectPropSupportsTiles, 0);
	property_set_.Set(kOfxImageEffectPropTemporalClipAccess, 0);
	property_set_.Set(kOfxImageEffectPropSupportedComponents,
				  {
					  kOfxImageComponentNone,
					  kOfxImageComponentRGB,
					  kOfxImageComponentRGBA,
					  kOfxImageComponentAlpha,
				  });
	property_set_.Set(kOfxImageEffectPropSupportedContexts,
				  {// kOfxImageEffectContextGenerator,
				   kOfxImageEffectContextFilter});
	property_set_.Set(kOfxImageEffectPropSupportsMultipleClipDepths,
				  0);
	property_set_.Set(kOfxImageEffectPropSupportsMultipleClipPARs,
				  0);
	property_set_.Set(kOfxImageEffectPropSetableFrameRate, 0);
	property_set_.Set(kOfxImageEffectPropSetableFielding, 0);
	property_set_.Set(kOfxParamHostPropSupportsCustomInteract, 0);
	property_set_.Set(kOfxParamHostPropSupportsStringAnimation, 0);
	property_set_.Set(kOfxParamHostPropSupportsChoiceAnimation, 0);
	property_set_.Set(kOfxParamHostPropSupportsBooleanAnimation, 0);
	property_set_.Set(kOfxParamHostPropSupportsCustomAnimation, 0);
	property_set_.Set(kOfxParamHostPropMaxParameters, -1);
	property_set_.Set(kOfxParamHostPropMaxPages, 0);
	property_set_.Set(kOfxParamHostPropPageRowColumnCount, {0, 0});
	property_set_.Set(kOfxPropHostOSHandle, nullptr);
	property_set_.Set(kOfxParamHostPropSupportsParametricAnimation,
				  0);
	property_set_.Set(
		kOfxImageEffectInstancePropSequentialRender,
		0); // FIXME: Not true but sounds like more work than needed.
	property_set_.Set(kOfxImageEffectPropOpenGLRenderSupported, 0);
	property_set_.Set(
		kOfxImageEffectPropRenderQualityDraft,
		0); // FIXME: This would be pretty simple to implement.
	property_set_.Set(kOfxImageEffectHostPropNativeOrigin,
				  kOfxHostNativeOriginTopLeft);
}
