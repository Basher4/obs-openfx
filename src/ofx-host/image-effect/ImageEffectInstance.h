#pragma once

#include "ofx-host/image-effect/Clip.h"
#include "ofx-host/image-effect/ImageEffectBase.h"
#include "ofx-host/suites/Param.h"
#include "ofx-host/suites/Property.h"

namespace ofx::image_effect {
class ImageEffectInstance : public ImageEffectBase {
public:
	ImageEffectInstance(const OfxPlugin *plugin);

public:
	ClipCollection m_Clips;
};
}
