#pragma once
#include <optional>
#include <vector>

#include <QPair>
#include <QString>

#include "openfx/ofxCore.h"
#include "ofx/suites/property-suite.h"
#include "ofx/image-effect/clip.h"

namespace ofx::image_effect {
class Effect {
public:
	Effect(OfxPlugin *ofx_plugin, OfxHost *host);

	bool FetchEffectParameters(const char *context);

	Clip *CreateClip(const QString &name);

	[[nodiscard]] QString GetPluginIdentifier() const;
	[[nodiscard]] QPair<int, int> GetPluginVersion() const;
	[[nodiscard]] suites::v1::PropertySet &GetPropertySet()
	{
		return property_set_;
	}
	[[nodiscard]] suites::v1::ParameterSet &GetParameterSet()
	{
		return parameter_set_;
	}

	[[nodiscard]] std::optional<Clip *>
	GetClipByName(const QString &name) const;

private:
	bool LoadAction() const;
	bool DescribeAction() const;
	bool DescribeInContextAction(const char *context) const;

private:
	OfxPlugin *ofx_plugin_;
	bool can_create_instance_ = false;

	suites::v1::PropertySet property_set_;
	suites::v1::ParameterSet parameter_set_;
	std::vector<std::unique_ptr<Clip>> clips_;
};
}
