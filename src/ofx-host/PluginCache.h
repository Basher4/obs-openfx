// This class finds all OFX binaries for the specific host system.
// From documentation:
//	A list of directories is supplied in the “OFX_PLUGIN_PATH” environment variable,
//	these are examined, then the default location is examined.
// Default locations:
//	Windows:

#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <Qdir>

#include "ofx-host/Host.h"
#include "ofx-host/image-effect/ImageEffectPlugin.h"
#include "ofx-host/PluginBinary.h"

namespace ofx {
/**
 * \brief Container to hold references to all available plugins in the system.
 */
class PluginCache {
public:
	PluginCache(const OfxHost &host);

	void AddPath(const std::wstring &path);
	void Rescan();

	std::optional<const image_effect::ImageEffectPlugin *>
	GetPluginByIdentifier(const char *ident, int major_version = -1,
			      int minor_version = -1) const;

	const std::vector<image_effect::ImageEffectPlugin *> GetAllPlugins() const;

private:
	int ParseOfxDirectory(QDir dir);
	void InitializeBinaries();

private:
	const OfxHost &m_OfxHost;
	QStringList m_PathsToConsider;

	std::vector<PluginBinary> m_PluginBinaries;
	std::unordered_map<QString, image_effect::ImageEffectPlugin *>
		m_IdentToPluginMap;
};
}
