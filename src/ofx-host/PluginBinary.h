#pragma once

#include <vector>
#include <string>
#include <QLibrary>

#include <openfx/ofxCore.h>

namespace ofx {
// Class representing OFX Plugin binary.
class PluginBinary {
public:
	PluginBinary(const QString &binaryPath);
	~PluginBinary() = default;

	const QString GetBinaryPath() const { return m_BinaryPath; }
	const QString GetPluginIdentifier() const
	{
		return m_BinaryPath.split(".")[0];
	}

	void LoadBinary(const OfxHost *host);
	const std::vector<OfxPlugin *> &GetPluginInfo();

private:
	OfxStatus OfxSetHost(const OfxHost *host);
	int OfxGetNumberOfPlugins();
	OfxPlugin *OfxGetPlugin(int n);

protected:
	bool m_IsHostSet = false;

	QLibrary m_Library;
	QString m_BinaryPath;

	QFunctionPointer fn_OfxSetHost;
	QFunctionPointer fn_OfxGetNumberOfPlugins;
	QFunctionPointer fn_OfxGetPlugin;

	std::vector<OfxPlugin *> m_PluginCache;
};
};
