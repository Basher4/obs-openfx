#include <chrono>

#include <obs-module.h>
#include <util/base.h>

#include "ofx-host/Host.h"

#include "logging.h"
#include "names.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-openfx", "en-US")
MODULE_EXPORT const char *obs_module_description(void)
{
	return "OpenFX Host";
}

MODULE_EXPORT const char *obs_module_name(void)
{
	return obs_module_text(LOC_MODULE_NAME);
}

static obs_source_frame *filter_video(void *data, obs_source_frame *frame)
{
	ofx::Host *host = reinterpret_cast<ofx::Host *>(data);

	host->OnNewFrame(frame);
	return host->GetProcessedFrame().value_or(nullptr);
}

bool obs_module_load(void)
{
	struct obs_source_info ofx_filter = {};

	ofx_filter.id = "ofx_host";
	ofx_filter.type = OBS_SOURCE_TYPE_FILTER;
	ofx_filter.output_flags = OBS_SOURCE_ASYNC_VIDEO;
	ofx_filter.get_name = [](void *) { return "OpenFX Host"; };
	ofx_filter.create = [](obs_data_t *settings, obs_source_t *context) {
		return reinterpret_cast<void *>(new ofx::Host());
	};
	ofx_filter.destroy = [](void *host) {
		delete reinterpret_cast<ofx::Host *>(host);
	};
	ofx_filter.update = [](void *data, obs_data_t *settings) {};
	ofx_filter.filter_video = filter_video;
	ofx_filter.filter_remove = [](void *data, obs_source_t *source) {};

	obs_register_source(&ofx_filter);
	return true;
}

void obs_module_unload(void)
{
	return;
}
