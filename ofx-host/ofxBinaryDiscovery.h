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

#include "ofxBinary.h"

namespace OFX {
class BinaryDiscovery {
public:
	using BinaryList = std::list<std::unique_ptr<Binary>>;

private:
	const QString env_ofxPluginPath = "OFX_PLUGIN_PATH";

	// We need to initialize this dynamically because of Windows.
	QString archName;
	QString defaultPath;
	QStringList pathsToConsider;

public:
	BinaryDiscovery();

	void AddPath(const std::wstring &path);
	int GetAllBinaries(BinaryList &outPlugins);

private:
	int ParseOfxDirectory(QDir dir, BinaryList &outPlugins);
	void InitDefaultArchPath();
};
}
