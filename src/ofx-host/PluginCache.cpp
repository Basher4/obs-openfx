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

PluginCache::PluginCache(const OfxHost &host) : m_OfxHost(host)
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

	InitializeBinaries();
}

std::optional<const image_effect::ImageEffectPlugin *>
PluginCache::GetPluginByIdentifier(const char *ident, int major_version,
				   int minor_version) const
{
	QString key(ident);
	auto it = m_IdentToPluginMap.find(key);

	std::optional<const image_effect::ImageEffectPlugin *> pluginOpt =
		std::nullopt;
	int maxFoundMajorVersion = -1;
	int maxFoundMinorVersion = -1;

	while (it != m_IdentToPluginMap.end() && it->first == key) {
		int majorVer = it->second->GetOfxPlugin()->pluginVersionMajor;
		int minorVer = it->second->GetOfxPlugin()->pluginVersionMinor;

		if (major_version != -1 && major_version != majorVer)
			continue;

		if (maxFoundMajorVersion < majorVer) {
			maxFoundMajorVersion = majorVer;
			maxFoundMinorVersion = minorVer;
			pluginOpt = std::make_optional(it->second);
		}

		if (maxFoundMinorVersion < minorVer) {
			maxFoundMinorVersion = minorVer;
			pluginOpt = std::make_optional(it->second);
		}
	}

	return pluginOpt;
}

const std::vector<image_effect::ImageEffectPlugin *>
PluginCache::GetAllPlugins() const
{
	std::vector<image_effect::ImageEffectPlugin *> result;
	result.reserve(m_IdentToPluginMap.size());
	
	for (auto kvp: m_IdentToPluginMap) {
		result.push_back(kvp.second);
	}
	
	return result;
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
		m_PluginBinaries.emplace_back(fileInfo.absoluteFilePath());
	}

	return pluginCount;
}

void PluginCache::InitializeBinaries()
{
	for (auto &binary : m_PluginBinaries) {
		bool should_load = binary.LoadBinary(m_OfxHost);
		if (!should_load)
			continue;

		for (const auto &plugin : binary.GetPluginsInBinary()) {

			m_IdentToPluginMap.emplace(
				plugin->GetPluginIdentQstring(), plugin.get());
			plugin->Load(m_OfxHost);
		}
	}
}
}; // namespace
