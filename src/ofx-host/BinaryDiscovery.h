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
class BinaryDiscovery {
private:
	QStringList pathsToConsider;

public:
	BinaryDiscovery();

	void AddPath(const std::wstring &path);
	int GetAllBinaries(std::list<std::unique_ptr<PluginBinary>> &outPlugins);

private:
	int ParseOfxDirectory(QDir dir, std::list<std::unique_ptr<PluginBinary>> &outPlugins);
};
}
