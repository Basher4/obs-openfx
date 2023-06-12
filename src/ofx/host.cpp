#include "ofx/host.h"
using namespace ofx;

void Host::CaptureNewFrame(obs_source_frame *frame)
{
}

std::optional<obs_source_frame *> Host::RenderFrame(double time)
{
	return std::nullopt;
}

OfxHost * Host::GetOfxStruct()
{
	return nullptr;
}
