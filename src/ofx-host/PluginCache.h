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
#include <qdir.h>

#include "PluginBinary.h"

namespace ofx {
class PluginCache {
public:
	PluginCache();

	void AddPath(const std::wstring &path);
	void Rescan();
	const std::vector<std::unique_ptr<PluginBinary>> &GetAllBinaries() const
	{
		return m_PluginBinaries;
	}

	void InitializeAllPlugins(OfxHost &host);

	std::optional<const OfxPlugin *>
	GetPluginByIdentifier(const char *ident, int majorVersion = -1,
			      int minorVersion = -1) const;

private:
	int ParseOfxDirectory(QDir dir);

private:
	QStringList m_PathsToConsider;
	std::vector<std::unique_ptr<PluginBinary>> m_PluginBinaries;
	QMultiMap<QString, OfxPlugin *> m_IdentToPluginMap;
};
}
