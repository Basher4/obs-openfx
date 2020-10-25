#pragma once

#include <list>
#include <string>
#include <QLibrary>

#include "../openfx/ofxCore.h"

namespace OFX {
// Class representing OFX Plugin binary.
class Binary {
protected:
	bool isHostSet = false;

	QLibrary library;
	QString binaryPath;

	QFunctionPointer fn_OfxSetHost;
	QFunctionPointer fn_OfxGetNumberOfPlugins;
	QFunctionPointer fn_OfxGetPlugin;

	std::list<OfxPlugin *> pluginCache;

public:
	Binary(const QString &binaryPath);
	~Binary() = default;

	// Path to the file.
	const QString &getBinaryPath() const { return binaryPath; }

	// Exported functions.
	OfxStatus OfxSetHost(const OfxHost *host);
	int OfxGetNumberOfPlugins();
	OfxPlugin *OfxGetPlugin(int n);

	const std::list<OfxPlugin *> &GetAllPlugins();
};
};
