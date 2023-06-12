#include "ofx-host/image-effect/ImageEffectBase.h"

ofx::image_effect::ImageEffectBase::ImageEffectBase(const OfxPlugin *plugin)
		:m_Plugin(plugin), m_PluginIdent(plugin->pluginIdentifier)
{
}
