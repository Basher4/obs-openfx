#include "ofxBinary.h"

namespace OFX {
Binary::Binary(const QString& binaryPath) :
	binaryPath(binaryPath),
	library(binaryPath)
{
	// OfxSetHost isn't defined in older versions of the API (pre-2020).
	fn_OfxSetHost = library.resolve("OfxSetHost");

	fn_OfxGetPlugin = library.resolve("OfxGetPlugin");
	assert(fn_OfxGetPlugin != nullptr);

	fn_OfxGetNumberOfPlugins = library.resolve("OfxGetNumberOfPlugins");
	assert(fn_OfxGetNumberOfPlugins != nullptr);
}

OfxStatus Binary::OfxSetHost(const OfxHost *host)
{
	using fnType = OfxStatus (*)(const OfxHost *);

	isHostSet = true;
	if (fn_OfxSetHost == nullptr) {
		return kOfxStatErrUnsupported;
	}

	return reinterpret_cast<fnType>(fn_OfxSetHost)(host);
}

int Binary::OfxGetNumberOfPlugins()
{
	using fnType = int (*)();

	assert(isHostSet);
	return reinterpret_cast<fnType>(fn_OfxGetNumberOfPlugins)();
}

OfxPlugin *Binary::OfxGetPlugin(int n)
{
	using fnType = OfxPlugin *(*)(int);

	assert(isHostSet);
	return reinterpret_cast<fnType>(fn_OfxGetPlugin)(n);
}

const std::list<OfxPlugin*> &Binary::GetAllPlugins()
{
	assert(isHostSet);

	// Check if we have cached all plugins.
	if (!pluginCache.empty()) {
		return pluginCache;
	}

	// Otherwise populate the cache.
	int nPlugins = OfxGetNumberOfPlugins();
	for (int i = 0; i < nPlugins; i++) {
		pluginCache.push_back(OfxGetPlugin(i));
	}

	return pluginCache;
}
};
