#include "ofxBinaryDiscovery.h"

#include <qdebug.h>
#include <qbytearray.h>
#include <qdir.h>
#include <qdiriterator.h>
#include <qstring.h>

#include "ofxBinary.h"

namespace OFX {
BinaryDiscovery::BinaryDiscovery()
{
	InitDefaultArchPath();
	assert(!defaultPath.isEmpty());
	assert(!archName.isEmpty());

	QString ofxEnv =
		qEnvironmentVariable(env_ofxPluginPath.toStdString().c_str());
	if (!ofxEnv.isEmpty()) {
		pathsToConsider = ofxEnv.split(";");
	}

	pathsToConsider.append(defaultPath);
}

void BinaryDiscovery::AddPath(const std::wstring &path)
{
	pathsToConsider.append(QString::fromStdWString(path));
}

int BinaryDiscovery::GetAllBinaries(BinaryList &outPlugins)
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

			QDir subdir(subdirName);
			qDebug() << "Traversing directory" << subdir.absolutePath();
			numPlugins += ParseOfxDirectory(subdir, outPlugins);
		}
	}

	return numPlugins;
}

int BinaryDiscovery::ParseOfxDirectory(QDir dir, BinaryList &outPlugins)
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

		// Navigate to the directory with ofx binaries.
		subdir.cd("Contents");
		subdir.cd(archName);

		// Set proper filters to iterate only through .ofx binaries.
		subdir.setFilter(QDir::Files);
		subdir.setNameFilters(QStringList() << "*.ofx");

		// Create an OFX::Binary for every valid ofx plugin.
		QFileInfoList list = subdir.entryInfoList();
		pluginCount += list.size();

		for (QFileInfo fileInfo : list) {
			qDebug() << "Discovered plugin" << fileInfo.absoluteFilePath();
			auto binaryPtr = new Binary(fileInfo.absoluteFilePath());
		}
	}

	return pluginCount;
}

void BinaryDiscovery::InitDefaultArchPath()
{
	// Sorry, but you really shouldn't be using 32bit systems in 2020.
#ifdef Q_OS_WIN
	archName = "Win64";
	defaultPath = qEnvironmentVariable("CommonProgramFiles");
	defaultPath.append("\\OFX");
#elif Q_OS_MAC
	// Host should also consider universal binaries for OSX but I have no way
	// to test this. So we'll only have this for now.
	archName = "MacOS-x86-64";
	defaultPath = "/Library/OFX";
#else // Linux
	archName = "Linux-x86-64";
	defaultPath = "/usr/OFX";
#endif
}
}
