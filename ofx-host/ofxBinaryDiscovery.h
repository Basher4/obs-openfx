// This class finds all OFX binaries for the specific host system.
// From documentation:
//	A list of directories is supplied in the “OFX_PLUGIN_PATH” environment variable,
//	these are examined, then the default location is examined.
// Default locations:
//	Windows:

#pragma once

#include <list>
#include <memory>>
#include <string>

#include <qdir.h>

#include "ofxBinary.h"

namespace OFX {
class BinaryDiscovery {
private:
	const std::string env_ofxPluginPath = "OFX_PLUGIN_PATH";

// Sorry, but you really shouldn't be using 32bit systems in 2020.
#ifdef Q_OS_WIN
	const std::string archName = "Win64";
	const std::wstring defaultPath = L"%CommonProgramFiles%\\OFX\\Plugins";
#elif Q_OS_MAC
	// Host should also consider universal binaries for OSX but I have no way
	// to test this. So we'll only have this for now.
	const std::string archName = "MacOS-x86-64";
	const std::wstring defaultPath = L"/Library/OFX/Plugins";
#else // Linux
	const std::string archName = "Linux-x86-64";
	const std::wstring defaultPath = L"/usr/OFX/Plugins";
#endif

	QStringList pathsToConsider;

public:
	BinaryDiscovery();

	void AddPath(const std::wstring &path);
	int GetAllBinaries(std::list<std::unique_ptr<Binary>> &outPlugins);

private:
	int ParseOfxDirectory(QDir dir, std::list<std::unique_ptr<Binary>> &outPlugins);
};
}
