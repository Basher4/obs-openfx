#pragma once
#include <optional>

#include <QQueue>

#include "obs.h"
#include "openfx/ofxCore.h"
#include "ofx/suites/property-suite.h"

namespace ofx {
class Host {
public:
	Host();

public:
	void CaptureNewFrame(obs_source_frame *frame);
	std::optional<obs_source_frame *> RenderFrame(double time);

	OfxHost *GetOfxStruct();

private:
	static const void *SfnFetchSuite(OfxPropertySetHandle host,
					 const char *suiteName,
					 int suiteVersion);

	void InitializeHostPropertySet();

private:
	OfxHost ofx_host_ = {property_set_, SfnFetchSuite};
	suites::v1::PropertySet property_set_;

	QQueue<obs_source_frame *> frame_queue_;
};
}
