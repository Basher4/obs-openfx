#include "ofx-host/image-effect/ImageEffectInstance.h"
#include "ofx-host/image-effect/ImageEffectPlugin.h"
#include "logging.h"

#include "openfx/ofxCore.h"
#include "openfx/ofxImageEffect.h"

ofx::image_effect::ImageEffectPlugin::ImageEffectPlugin(
	const OfxPlugin *ofxPlugin)
	: ImageEffectBase(ofxPlugin)
{
}

bool ofx::image_effect::ImageEffectPlugin::Load(const OfxHost &host)
{
	OfxStatus err;

	// Load the plugin.
	err = m_Plugin->mainEntry(kOfxActionLoad, nullptr, nullptr, nullptr);
	if (err != kOfxStatOK && err != kOfxStatReplyDefault) {
		ofx_warning("%s failed load (%d)",
			    m_PluginIdent.toLocal8Bit().data(), err);
		m_ShouldBeIgnored = true;
		return false;
	}

	// Describe the plugin generally.
	err = m_Plugin->mainEntry(kOfxActionDescribe, this, nullptr, nullptr);
	if (err != kOfxStatOK) {
		ofx_warning("%s failed describe (%d)",
			    m_PluginIdent.toLocal8Bit().data(), err);
		m_ShouldBeIgnored = true;
		return false;
	}

	// Describe the plugin in General OFX context.
	suites::v1::PropertySet context;
	context.set_property(kOfxImageEffectPropContext,
			     kOfxImageEffectContextGeneral);
	const auto in_arg = reinterpret_cast<OfxPropertySetHandle>(&context);

	err = m_Plugin->mainEntry(kOfxImageEffectActionDescribeInContext, this,
				  in_arg, nullptr);
	if (err != kOfxStatOK) {
		ofx_warning("%s failed describe in context (%d)",
			    m_PluginIdent.toLocal8Bit().data(), err);
		m_ShouldBeIgnored = true;
		return false;
	}

	return true;
}

bool ofx::image_effect::ImageEffectPlugin::Unload()
{
	if (m_Instances.size() > 0) {
		ofx_error("Trying to unload effect with living instances");
		return false;
	}

	OfxStatus err = m_Plugin->mainEntry(kOfxActionUnload, nullptr, nullptr,
					    nullptr);
	if (err == kOfxStatErrFatal) {
		ofx_error("%s failed to unload (%d)",
			  m_PluginIdent.toLocal8Bit().data(), err);
		return false;
	}

	return true;
}

ofx::image_effect::ImageEffectInstance ofx::image_effect::ImageEffectPlugin::
CreateInstance()
{
	// TODO: Write this function
	return ImageEffectInstance(m_Plugin);
}
