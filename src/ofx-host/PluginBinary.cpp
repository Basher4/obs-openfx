#include "PluginBinary.h"

namespace ofx {
PluginBinary::PluginBinary(const QString &binaryPath)
	: m_Library(binaryPath), m_BinaryPath(binaryPath)
{
	// OfxSetHost isn't defined in older versions of the API (pre-2020).
	fn_OfxSetHost = m_Library.resolve("OfxSetHost");

	fn_OfxGetPlugin = m_Library.resolve("OfxGetPlugin");
	assert(fn_OfxGetPlugin != nullptr);

	fn_OfxGetNumberOfPlugins = m_Library.resolve("OfxGetNumberOfPlugins");
	assert(fn_OfxGetNumberOfPlugins != nullptr);
}

bool PluginBinary::LoadBinary(const OfxHost &host)
{
	const OfxStatus status = OfxSetHost(host);
	if (status == kOfxStatFailed)
		return false;

	int numPlugins = OfxGetNumberOfPlugins();
	m_PluginCache.reserve(numPlugins);

	GetPluginsInBinary();

	return true;
}

OfxStatus PluginBinary::OfxSetHost(const OfxHost &host)
{
	using fnType = OfxStatus (*)(const OfxHost *);

	m_IsHostSet = true;
	if (fn_OfxSetHost == nullptr) {
		return kOfxStatErrUnsupported;
	}

	return reinterpret_cast<fnType>(fn_OfxSetHost)(&host);
}

int PluginBinary::OfxGetNumberOfPlugins() const
{
	using fnType = int (*)();

	assert(m_IsHostSet);
	return reinterpret_cast<fnType>(fn_OfxGetNumberOfPlugins)();
}

OfxPlugin *PluginBinary::OfxGetPlugin(int n) const
{
	using fnType = OfxPlugin *(*)(int);

	assert(m_IsHostSet);
	return reinterpret_cast<fnType>(fn_OfxGetPlugin)(n);
}

const std::vector<std::unique_ptr<image_effect::ImageEffectPlugin>> &
PluginBinary::GetPluginsInBinary()
{
	assert(m_IsHostSet);

	// Check if we have cached all plugins.
	if (!m_PluginCache.empty()) {
		return m_PluginCache;
	}

	// Otherwise populate the cache.
	int nPlugins = OfxGetNumberOfPlugins();
	for (int i = 0; i < nPlugins; i++) {
		OfxPlugin *ofx_plugin = OfxGetPlugin(i);
		m_PluginCache.emplace_back(
			new image_effect::ImageEffectPlugin(ofx_plugin));
	}

	return m_PluginCache;
}
}; // namespace
