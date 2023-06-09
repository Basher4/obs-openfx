#include <qdebug.h>
#include <qbytearray.h>
#include <qdir.h>
#include <qdiriterator.h>
#include <qstring.h>

#include "PluginCache.h"
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

PluginCache::PluginCache()
{
	QString ofxEnv =
		qEnvironmentVariable(OFX_PLUGIN_ENVAR.toStdString().c_str());
	if (!ofxEnv.isEmpty()) {
		m_PathsToConsider = ofxEnv.split(";");
	}
	m_PathsToConsider.append(OFX_PLUGIN_DEFAULT_PATH);
}

void PluginCache::AddPath(const std::wstring &path)
{
	m_PathsToConsider.append(QString::fromStdWString(path));
}

void PluginCache::Rescan()
{
	m_PluginBinaries.clear();

	for (QString path : m_PathsToConsider) {
		QDirIterator it(path, QStringList() << "*.ofx.bundle",
				QDir::Dirs);

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
			ParseOfxDirectory(subdir);
		}
	}
}

void PluginCache::InitializeAllPlugins(OfxHost &host)
{
	for (const auto &binary : m_PluginBinaries) {
		binary->LoadBinary(&host);

		for (const auto &plugin : binary->GetPluginInfo()) {
			m_IdentToPluginMap.insert(
				QString(plugin->pluginIdentifier), plugin);
		}
	}
}

std::optional<const OfxPlugin *>
PluginCache::GetPluginByIdentifier(const char *ident, int majorVersion,
				   int minorVersion) const
{
	QString key(ident);
	auto it = m_IdentToPluginMap.find(key);

	OfxPlugin *plugin = nullptr;
	int maxFoundMajorVersion = -1;
	int maxFoundMinorVersion = -1;

	while (it != m_IdentToPluginMap.end() && it.key() == key) {
		int plugMajVer = it.value()->pluginVersionMajor;
		int plugMinVer = it.value()->pluginVersionMinor;

		if (majorVersion != -1 && majorVersion != plugMajVer)
			continue;

		if (maxFoundMajorVersion < plugMajVer) {
			maxFoundMajorVersion = plugMajVer;
			maxFoundMinorVersion = plugMinVer;
			plugin = it.value();
		}

		if (maxFoundMinorVersion < plugMinVer) {
			maxFoundMinorVersion = plugMinVer;
			plugin = it.value();
		}
	}

	if (plugin == nullptr)
		return std::nullopt;
	return std::make_optional(plugin);
}

int PluginCache::ParseOfxDirectory(QDir dir)
{
	int pluginCount = 0;

	// Get canonical path of this directory.
	QString canonicalPath = dir.canonicalPath();
	if (canonicalPath.isEmpty()) {
		return 0;
	}

	// Navigate to the directory with ofx binaries.
	dir.cd("Contents");
	dir.cd(OFX_PLUGIN_ARCH);

	// Set proper filters to iterate only through .ofx binaries.
	dir.setFilter(QDir::Files);
	dir.setNameFilters(QStringList() << "*.ofx");

	// Create an OFX::PluginBinary for every valid ofx plugin.
	QFileInfoList list = dir.entryInfoList();
	pluginCount += list.size();

	for (QFileInfo fileInfo : list) {
		qDebug() << "Discovered plugin" << fileInfo.absoluteFilePath();
		m_PluginBinaries.emplace_back(
			new PluginBinary(fileInfo.absoluteFilePath()));
	}

	return pluginCount;
}
}; // namespace
