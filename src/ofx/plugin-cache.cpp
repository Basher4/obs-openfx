#include "ofx/plugin-cache.h"
using namespace ofx;

PluginCache::~PluginCache()
{
	// TODO: Finalize and unload all plugins.
}

void PluginCache::Rescan()
{
}

void PluginCache::Rescan(QList<QString>& additional_paths)
{
}

void PluginCache::InitializePlugins(OfxHost *host)
{
	// This will take care of initializing plugins, up to describe in context.
	// Then it is the role of the host to do everything else.
}

const QList<image_effect::Plugin *> PluginCache::GetAllPlugins()
{
	return {};
}

image_effect::Plugin * PluginCache::FindPluginByIdentifier(
	const std::string &ident)
{
	return nullptr;
}
