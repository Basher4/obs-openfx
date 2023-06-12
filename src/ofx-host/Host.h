#pragma once

#include <obs.h>
#include <optional>
#include <QQueue>

#include "openfx/ofxCore.h"

#include "ofx-host/PluginCache.h"
#include "ofx-host/suites/Param.h"
#include "ofx-host/suites/Property.h"
#include "ofx-host/image-effect/ImageEffectInstance.h"
#include "ofx-host/image-effect/ImageEffectPlugin.h"

namespace ofx {
class Host {
public:
	Host();
	~Host() = default;

	void OnNewFrame(obs_source_frame *frame);
	std::optional<obs_source_frame *> GetProcessedFrame();

	void SetActiveInstance(const image_effect::ImageEffectInstance *plugin);

	const OfxHost &GetOfxHostStruct() const { return m_OfxHost; }

private:
	static const void *sfn_fetch_suite(OfxPropertySetHandle handle,
					   const char *suiteName,
					   int suiteVersion)
	{
		const auto *inst = reinterpret_cast<Host *>(handle);
		return inst->FetchSuite(suiteName, suiteVersion);
	}
	const void *FetchSuite(const char *suiteName, int suiteVersion) const;

	void InitializeOfxHostProperties();

	void InitializeActivePlugin();
	void FinalizeLoadedPlugin();

private:
	OfxHost m_OfxHost{};

	suites::v1::PropertySet m_HostProperties;

	image_effect::ImageEffectInstance *m_ActiveInstance{};

	uint64_t m_PrevFrameTimestamp = 0;
	QQueue<obs_source_frame *> m_FrameQueue;
};
}
