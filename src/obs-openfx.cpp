#include <chrono>

#include <obs-module.h>
#include <util/base.h>

#include "ofx-host/Host.h"

#include "logging.h"
#include "names.h"

const char *SETTING_AVAILABLE_PLUGINS =
	"ofx.availPlugins";

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

static obs_properties_t *get_properties(void *data)
{
	ofx::Host *host = reinterpret_cast<ofx::Host *>(data);
	obs_properties_t *props = obs_properties_create();
	obs_properties_t *pluginGrp = obs_properties_create();

	obs_property_t *plugins_list = obs_properties_add_list(
		props, SETTING_AVAILABLE_PLUGINS,
		"Available Plugins: ", OBS_COMBO_TYPE_LIST,
		OBS_COMBO_FORMAT_STRING);

	obs_property_set_modified_callback2(
		plugins_list,
		[](void *priv, obs_properties_t *props, obs_property_t *property,
		   obs_data_t *settings) -> bool {
			ofx::Host *ofxHost = reinterpret_cast<ofx::Host*>(priv);
			const char *plugin_ident = obs_data_get_string(
				settings, SETTING_AVAILABLE_PLUGINS);

			ofxHost->LoadPlugin(plugin_ident);

			//TODO: Build the plugin UI and hook it up to the host.

			return true;
		}, data);

	for (const auto &binary : host->GetPluginBinaries()) {
		for (const auto &plugin : binary->GetPluginInfo()) {
			QString info = QString::asprintf(
				"[%s] %s (%d.%d)", plugin->pluginApi,
				plugin->pluginIdentifier,
				plugin->pluginVersionMajor,
				plugin->pluginVersionMinor);

			obs_property_list_add_string(plugins_list,
						     info.toLocal8Bit().data(),
						     plugin->pluginIdentifier);
		}
	}

	obs_properties_add_group(props, "pluginGrp", "Plugin interface",
				 OBS_GROUP_NORMAL, pluginGrp);

	return props;
}

bool obs_module_load(void)
{
	struct obs_source_info ofx_filter = {};

	ofx_filter.id = "ofx_host";
	ofx_filter.type = OBS_SOURCE_TYPE_FILTER;
	ofx_filter.output_flags = OBS_SOURCE_ASYNC_VIDEO;
	ofx_filter.get_name = [](void *) { return "OpenFX Host"; };
	ofx_filter.get_properties = get_properties;
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
