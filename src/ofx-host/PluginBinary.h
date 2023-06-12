#pragma once

#include <memory>
#include <vector>
#include <QLibrary>

#include <openfx/ofxCore.h>
#include <ofx-host/image-effect/ImageEffectPlugin.h>

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

	bool LoadBinary(const OfxHost& host);
	const std::vector<std::unique_ptr<image_effect::ImageEffectPlugin>> &GetPluginsInBinary();

private:
	OfxStatus OfxSetHost(const OfxHost &host);
	int OfxGetNumberOfPlugins() const;
	OfxPlugin *OfxGetPlugin(int n) const;

protected:
	bool m_IsHostSet = false;

	QLibrary m_Library;
	QString m_BinaryPath;

	QFunctionPointer fn_OfxSetHost;
	QFunctionPointer fn_OfxGetNumberOfPlugins;
	QFunctionPointer fn_OfxGetPlugin;

	std::vector<std::unique_ptr<image_effect::ImageEffectPlugin>> m_PluginCache;
};
};
