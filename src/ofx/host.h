#pragma once
#include <optional>

#include "obs.h"
#include "openfx/ofxCore.h"

namespace ofx {
class Host {
public:
	void CaptureNewFrame(obs_source_frame * frame);
	std::optional<obs_source_frame*> RenderFrame(double time);

	OfxHost *GetOfxStruct();
};
}
