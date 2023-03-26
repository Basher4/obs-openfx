#include <qdebug.h>
#include <qbytearray.h>
#include <qdir.h>
#include <qdiriterator.h>
#include <qstring.h>

#include "BinaryDiscovery.h"
#include "PluginBinary.h"

namespace ofx {

const QString OFX_PLUGIN_ENVAR = "OFX_PLUGIN_PATH";

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
const QString OFX_PLUGIN_DEFAULT_PATH =
	"C:\\Program Files\\Common Files\\OFX\\Plugins";
#ifdef _WIN64
const QString OFX_PLUGIN_ARCH = "Win64";
#else
const QString OFX_PLUGIN_ARCH = "Win32";
#endif
#elif __APPLE__
const QString OFX_PLUGIN_DEFAULT_PATH = "/Library/OFX/Plugins";
const char *OFX_PLUGIN_ARCH = "MacOS-x86-64";
#elif __linux__ || __unix__
const QString OFX_PLUGIN_DEFAULT_PATH = "/usr/OFX/Plugins";
const char *OFX_PLUGIN_ARCH = "Linux-x86-64";
#endif

BinaryDiscovery::BinaryDiscovery()
{
	QString ofxEnv =
		qEnvironmentVariable(OFX_PLUGIN_ENVAR.toStdString().c_str());
	if (!ofxEnv.isEmpty()) {
		pathsToConsider = ofxEnv.split(";");
	}
	pathsToConsider.append(OFX_PLUGIN_DEFAULT_PATH);
}

void BinaryDiscovery::AddPath(const std::wstring &path)
{
	pathsToConsider.append(QString::fromStdWString(path));
}

int BinaryDiscovery::GetAllBinaries(
	std::list<std::unique_ptr<PluginBinary>> &outPlugins)
{
	int numPlugins = 0;

	for (QString path : pathsToConsider) {
		QDirIterator it(path);

		while (it.hasNext()) {
			auto subdirName = it.next();
			if (subdirName.endsWith("/.") ||
			    subdirName.endsWith("/..")) {
				continue;
			}

			// Spec says that if a directory begins with @ it should be skipped.
			if (subdirName[0] == '@') {
				continue;
			}

			QDir subdir(subdirName);
			qDebug() << "Traversing directory"
				 << subdir.absolutePath();
			numPlugins += ParseOfxDirectory(subdir, outPlugins);
		}
	}

	return numPlugins;
}

int BinaryDiscovery::ParseOfxDirectory(
	QDir dir, std::list<std::unique_ptr<PluginBinary>> &outPlugins)
{
	int pluginCount = 0;

	// Get canonical path of this directory.
	QString canonicalPath = dir.canonicalPath();
	if (canonicalPath.isEmpty()) {
		return 0;
	}

	// Iterate through all ofx.bundle subdirectories.
	QDirIterator it(canonicalPath, QStringList() << "*.ofx.bundle",
			QDir::Dirs);

	while (it.hasNext()) {
		QDir subdir(it.next());

		// Spec says that if a directory begins with @ it should be skipped.
		if (subdir[0] == '@') {
			continue;
		}

		// Navigate to the directory with ofx binaries.
		subdir.cd("Contents");
		subdir.cd(OFX_PLUGIN_ARCH);

		// Set proper filters to iterate only through .ofx binaries.
		subdir.setFilter(QDir::Files);
		subdir.setNameFilters(QStringList() << "*.ofx");

		// Create an OFX::PluginBinary for every valid ofx plugin.
		QFileInfoList list = subdir.entryInfoList();
		pluginCount += list.size();

		for (QFileInfo fileInfo : list) {
			qDebug() << "Discovered plugin"
				 << fileInfo.absoluteFilePath();
			auto binaryPtr =
				new PluginBinary(fileInfo.absoluteFilePath());
		}
	}

	return pluginCount;
}
}; // namespace
