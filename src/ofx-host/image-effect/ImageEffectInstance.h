#pragma once

#include <ofx-host/suites/Param.h>
#include <ofx-host/suites/Property.h>
#include <ofx-host/image-effect/Clip.h>

namespace ofx::image_effect {
class ImageEffectInstance {
public:
	ClipCollection m_Clips;
	suites::v1::PropertySet m_PropertySet;
	suites::v1::ParameterSet m_ParameterSet;
};
}
