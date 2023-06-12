#pragma once

#include <vector>
#include <memory>

#include "openfx/ofxCore.h"
#include "ofx-host/image-effect/ImageEffectBase.h"
#include "ofx-host/image-effect/ImageEffectInstance.h"
#include "ofx-host/suites/Property.h"

namespace ofx::image_effect {
class ImageEffectPlugin : public ImageEffectBase {
public:
	ImageEffectPlugin(const OfxPlugin *ofxPlugin);

	bool Load(const OfxHost &host);
	bool Unload();

	ImageEffectInstance CreateInstance();
	void DestroyInstance(ImageEffectInstance &instance);

	[[nodiscard]] bool ShouldIgnore() const { return m_ShouldBeIgnored; }

private:
	bool m_ShouldBeIgnored = false;
	std::vector<ImageEffectInstance> m_Instances;
};
}
