#include "PluginBinary.h"

namespace ofx {
PluginBinary::PluginBinary(const QString &binaryPath)
	: m_BinaryPath(binaryPath), m_Library(binaryPath)
{
	// OfxSetHost isn't defined in older versions of the API (pre-2020).
	fn_OfxSetHost = m_Library.resolve("OfxSetHost");

	fn_OfxGetPlugin = m_Library.resolve("OfxGetPlugin");
	assert(fn_OfxGetPlugin != nullptr);

	fn_OfxGetNumberOfPlugins = m_Library.resolve("OfxGetNumberOfPlugins");
	assert(fn_OfxGetNumberOfPlugins != nullptr);
}

void PluginBinary::LoadBinary(const OfxHost *host)
{
	OfxSetHost(host);	// Atm I don't really care about the result.

	int numPlugins = OfxGetNumberOfPlugins();
	m_PluginCache.reserve(numPlugins);

	GetPluginInfo();
}

OfxStatus PluginBinary::OfxSetHost(const OfxHost* host)
{
	using fnType = OfxStatus (*)(const OfxHost *);

	m_IsHostSet = true;
	if (fn_OfxSetHost == nullptr) {
		return kOfxStatErrUnsupported;
	}

	return reinterpret_cast<fnType>(fn_OfxSetHost)(host);
}

int PluginBinary::OfxGetNumberOfPlugins()
{
	using fnType = int (*)();

	assert(m_IsHostSet);
	return reinterpret_cast<fnType>(fn_OfxGetNumberOfPlugins)();
}

OfxPlugin *PluginBinary::OfxGetPlugin(int n)
{
	using fnType = OfxPlugin *(*)(int);

	assert(m_IsHostSet);
	return reinterpret_cast<fnType>(fn_OfxGetPlugin)(n);
}

const std::vector<OfxPlugin *> &PluginBinary::GetPluginInfo()
{
	assert(m_IsHostSet);

	// Check if we have cached all plugins.
	if (!m_PluginCache.empty()) {
		return m_PluginCache;
	}

	// Otherwise populate the cache.
	int nPlugins = OfxGetNumberOfPlugins();
	for (int i = 0; i < nPlugins; i++) {
		m_PluginCache.push_back(OfxGetPlugin(i));
	}

	return m_PluginCache;
}
}; // namespace
