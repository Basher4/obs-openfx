#include "ofxBinaryDiscovery.h"

#include <qbytearray.h>
#include <qdir.h>
#include <qdiriterator.h>
#include <qstring.h>

#include "ofxBinary.h"

namespace OFX {
BinaryDiscovery::BinaryDiscovery()
{
	QString ofxEnv = qEnvironmentVariable(env_ofxPluginPath.c_str());
	pathsToConsider = ofxEnv.split(";");
	pathsToConsider.append(QString::fromStdWString(defaultPath));
}

void BinaryDiscovery::AddPath(const std::wstring &path)
{
	pathsToConsider.append(QString::fromStdWString(path));
}

int BinaryDiscovery::GetAllBinaries(
	std::list<std::unique_ptr<Binary>> &outPlugins)
{
	int numPlugins = 0;

	for (QString path : pathsToConsider) {
		// Iterate through all ofx.bundle subdirectories.
		QDirIterator it(path, QStringList() << "*.ofx.bundle", QDir::Dirs);

		while (it.hasNext()) {
			QDir subdir(it.next());
			numPlugins += ParseOfxDirectory(subdir, outPlugins);
		}
	}

	return numPlugins;
}

int BinaryDiscovery::ParseOfxDirectory(
	QDir dir, std::list<std::unique_ptr<Binary>> &outPlugins)
{
	// Navigate to the directory with ofx binaries.
	dir.cd("Contents");
	dir.cd(QString::fromStdString(archName));

	// Set proper filters to iterate only through .ofx binaries.
	dir.setFilter(QDir::Files);
	dir.setNameFilters(QStringList() << "*.ofx");

	// Create an OFX::Binary for every valid ofx plugin.
	QFileInfoList list = dir.entryInfoList();
	for (QFileInfo fileInfo : list) {
		std::unique_ptr<Binary> binaryPtr(
			new Binary(fileInfo.absolutePath().toStdWString()));
		outPlugins.push_back(binaryPtr);
	}

	return list.size();
}
}
