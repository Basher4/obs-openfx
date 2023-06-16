#include <obs-module.h>

#include "logger.h"
#include "ofx/host.h"
#include "ofx/plugin-cache.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-openfx", "en-US")

MODULE_EXPORT const char *obs_module_description(void)
{
	return "OpenFX Host";
}

MODULE_EXPORT const char *obs_module_name(void)
{
	return "OpenFX Host"; // obs_module_text(LOC_MODULE_NAME);
}

ofx::Host *g_host;
ofx::PluginCache *g_plugin_cache;

static obs_source_frame *filter_video(void *data, obs_source_frame *frame)
{
	g_host->CaptureNewFrame(frame);
	return g_host->RenderFrame(0).value_or(nullptr);
}

static obs_properties_t *get_properties(void *data)
{
	//const auto host = static_cast<ofx::Host *>(data);
	//obs_properties_t *props = obs_properties_create();
	//obs_properties_t *pluginGrp = obs_properties_create();

	//obs_property_t *plugins_list = obs_properties_add_list(
	//	props, SETTING_AVAILABLE_PLUGINS,
	//	"Available Plugins: ", OBS_COMBO_TYPE_LIST,
	//	OBS_COMBO_FORMAT_STRING);

	//obs_property_set_modified_callback(
	//	plugins_list,
	//	[](obs_properties_t *props, obs_property_t *property, obs_data_t *settings) -> bool {
	//		const char *plugin_ident = obs_data_get_string(settings, SETTING_AVAILABLE_PLUGINS);
	//		auto *plugin = reinterpret_cast<
	//			ofx::image_effect::ImageEffectPlugin *>(
	//			plugAddr);

	//		//TODO: Build the plugin UI and hook it up to the host.

	//		return true;
	//	},
	//	data);

	//for (const auto plugin : g_PluginCache->GetAllEffects()) {
	//	const OfxPlugin *ofx_plugin = plugin->GetOfxPlugin();
	//	QString info = QString::asprintf(
	//		"[%s] %s (%d.%d)", ofx_plugin->pluginApi,
	//		ofx_plugin->pluginIdentifier,
	//		ofx_plugin->pluginVersionMajor,
	//		ofx_plugin->pluginVersionMinor);

	//	obs_property_list_add_int(plugins_list,
	//	                          info.toLocal8Bit().data(),
	//	                          reinterpret_cast<long long>(plugin));
	//}

	//obs_properties_add_group(props, "pluginGrp", "Effect interface",
	//			 OBS_GROUP_NORMAL, pluginGrp);

	//return props;
	return nullptr;
}

const char *stringToPtr(const QString &str)
{
	return str.toLocal8Bit().data();
}

bool obs_module_load(void)
{
	g_plugin_cache = new ofx::PluginCache;
	g_host = new ofx::Host;

	g_plugin_cache->InitializePlugins(g_host->GetOfxStruct());

	obs_source_info ofx_filter = {};
	ofx_filter.id = "github.basher4.ofx_host";
	ofx_filter.type = OBS_SOURCE_TYPE_FILTER;
	ofx_filter.output_flags = OBS_SOURCE_ASYNC_VIDEO;
	ofx_filter.get_name = [](void *) { return "OpenFX Host"; };
	ofx_filter.get_properties = get_properties;
	ofx_filter.create = [](obs_data_t *settings, obs_source_t *context) {
		return static_cast<void *>(nullptr);
	};
	ofx_filter.destroy = [](void *host) {};
	ofx_filter.update = [](void *data, obs_data_t *settings) {};
	ofx_filter.filter_video = filter_video;
	ofx_filter.filter_remove = [](void *data, obs_source_t *source) {};

	obs_register_source(&ofx_filter);
	return true;
}

void obs_module_unload(void)
{
	delete g_host;
	delete g_plugin_cache;
}
