#pragma once

#include <chrono>
#include <optional>
#include <queue>

#include <obs.h>

#include "openfx/ofxCore.h"

#include "PluginCache.h"
#include "PluginInstance.h"

namespace ofx {
class Host {
public:
	Host();
	~Host() = default;

	void OnNewFrame(obs_source_frame *frame);
	std::optional<obs_source_frame *> GetProcessedFrame();

	const std::vector<std::unique_ptr<PluginBinary>> &
	GetPluginBinaries() const
	{
		return m_PluginCache.GetAllBinaries();
	}
	void LoadPlugin(const std::string_view identifier);

private:
	static const void *SFN_FetchSuite(OfxPropertySetHandle host,
					  const char *suiteName,
					  int suiteVersion)
	{
		return reinterpret_cast<Host *>(host)->FetchSuite(suiteName,
								  suiteVersion);
	}

	const void *FetchSuite(const char *suiteName, int suiteVersion);

	// These two really should take plugin as an argument. But then I need to keep track
	// of the active plugin anyway. And I do I want to make another meta object to manage
	// plugin instances? That might be a good idea. I might need to do that anyway, to manage
	// property sets. But for now, this is it.
	void InitializeActivePlugin();
	void FinalizeLoadedPlugin();

private:
	OfxHost m_OfxHost;
	PluginCache m_PluginCache;
	const OfxPlugin *m_ActivePlugin;
	PluginInstance m_ActiveInstance;

	uint64_t m_PrevFrameTimestamp = 0;
	std::queue<obs_source_frame *> m_FrameQueue;
};
}
