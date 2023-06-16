#include "logger.h"
#include "ofx/image-effect/effect.h"
#include "ofx/suites/property-suite.h"
using namespace ofx;
using namespace ofx::image_effect;

#include "openfx/ofxImageEffect.h"

Effect::Effect(OfxPlugin *ofx_plugin, OfxHost *host) : ofx_plugin_(ofx_plugin)
{
	assert(ofx_plugin_ != nullptr);

	if (ofx_plugin_->setHost != nullptr) {
		ofx_plugin_->setHost(host);
	}
}

QString Effect::GetPluginIdentifier() const
{
	return {ofx_plugin_->pluginIdentifier};
}

QPair<int, int> Effect::GetPluginVersion() const
{
	return {
		ofx_plugin_->pluginVersionMajor,
		ofx_plugin_->pluginVersionMinor,
	};
}

bool Effect::FetchEffectParameters(const char *context)
{
	if (!LoadAction())
		return false;
	if (!DescribeAction())
		return false;
	if (!DescribeInContextAction(context))
		return false;

	can_create_instance_ = true;
	return true;
}

Clip *Effect::CreateClip(const QString &name)
{
	clips_.emplace_back(new Clip(name));
	return clips_.rbegin()->get();
}

std::optional<Clip *> Effect::GetClipByName(const QString &name) const
{
	for (const auto &clip : clips_) {
		if (name == clip->GetClipName()) {
			return clip.get();
		}
	}
	return std::nullopt;
}

bool Effect::LoadAction() const
{
	const OfxStatus err = ofx_plugin_->mainEntry(kOfxActionLoad, nullptr,
						     nullptr, nullptr);
	if (err == kOfxStatOK || err == kOfxStatReplyDefault)
		return true;

	ofx_warning("Loading plugin %s failed with status %d",
		    qPrintable(GetPluginIdentifier()), err);
	return false;
}

bool Effect::DescribeAction() const
{
	const OfxStatus err = ofx_plugin_->mainEntry(kOfxActionDescribe, this,
						     nullptr, nullptr);
	if (err == kOfxStatOK)
		return true;

	ofx_warning("Describe for %s failed with status %d",
		    qPrintable(GetPluginIdentifier()), err);
	return false;
}

bool Effect::DescribeInContextAction(const char *context) const
{
	suites::v1::PropertySet context_prop;
	context_prop.Set(kOfxImageEffectPropContext, context);

	const OfxStatus err =
		ofx_plugin_->mainEntry(kOfxImageEffectActionDescribeInContext,
				       this, context_prop, nullptr);
	if (err == kOfxStatOK)
		return true;

	ofx_warning("DescribeInContext(%s) for %s failed with status %d",
		    context, qPrintable(GetPluginIdentifier()), err);
	return false;
}
