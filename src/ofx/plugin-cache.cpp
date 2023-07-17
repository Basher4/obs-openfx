#include "ofx/plugin-cache.h"

#include <QDir>
#include <QDirIterator>

#include "logger.h"
#include "openfx/ofxImageEffect.h"
using namespace ofx;

const QString kOfxPluginEnvVar = "OFX_PLUGIN_PATH";

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
const QString kOfxPluginDefaultPath =
	R"(C:\Program Files\Common Files\OFX\Plugins)";
#ifdef _WIN64
const QString kOfxPluginArch = "Win64";
#else
const QString kOfxPluginArch = "Win32";
#endif
#elif __APPLE__
const QString kOfxPluginDefaultPath = "/Library/OFX/Plugins";
const char *kOfxPluginArch =
	"MacOS-x86-64"; // TODO: Should check for ARM but eh.
#elif __linux__ || __unix__
const QString kOfxPluginDefaultPath = "/usr/OFX/Plugins";
const char *kOfxPluginArch = "Linux-x86-64";
#endif

PluginCache::PluginCache(const QStringList &additional_paths)
{
	search_paths_.append(kOfxPluginDefaultPath);
	const QString ofx_env =
		qEnvironmentVariable(kOfxPluginEnvVar.toStdString().c_str());
	if (!ofx_env.isEmpty()) {
		search_paths_.append(ofx_env.split(";"));
	}
	search_paths_.append(additional_paths);

	SearchForPlugins();
}

PluginCache::~PluginCache()
{
	// TODO: Finalize and unload all plugins.
}

void PrintPropertySet(suites::v1::PropertySet &prop_set)
{
	using suites::v1::PropertySet;
	for (const auto &[name, type] : prop_set.GetDefinedProperties()) {
		switch (type) {
		case PropertySet::PropertyType::INT:
			ofx_info("\t%-48s: %d", qPrintable(name),
				 prop_set.GetInt(name).second);
			break;
		case PropertySet::PropertyType::DOUBLE:
			ofx_info("\t%-48s: %f", qPrintable(name),
				 prop_set.GetDouble(name).second);
			break;
		case PropertySet::PropertyType::STRING:
			ofx_info("\t%-48s: \"%s\"", qPrintable(name),
				 prop_set.GetString(name).second);
			break;
		case PropertySet::PropertyType::POINTER:
			ofx_info("\t%-48s: %p", qPrintable(name),
				 prop_set.GetPtr(name).second);
			break;
		}
	}
}

void PluginCache::InitializePlugins(OfxHost *host)
{
	for (const auto &binary : available_binaries_) {
		if (const bool should_load = binary->Initialize(host);
		    !should_load)
			continue;

		for (OfxPlugin *plugin : binary->GetAllPlugins()) {
			const auto effect =
				available_effects_
					.emplace_back(new image_effect::Effect(
						plugin, host))
					.get();
			effect->FetchEffectParameters(
				kOfxImageEffectContextFilter);

			ofx_info("---------- %s",
				 qPrintable(effect->GetPluginIdentifier()));
			ofx_info(">>> Registered properties:");
			PrintPropertySet(effect->GetPropertySet());

			ofx_info(">>> Registered parameters:");
			auto &param_set = effect->GetParameterSet();
			for (auto &param : param_set.GetDefinedParameters()) {
				ofx_info("%s", qPrintable(param));
				suites::v1::PropertySet *param_props =
					param_set.GetParameterByName(param)
						.value();
				PrintPropertySet(*param_props);
			}
			ofx_info("------------------------------------\n");
		}
	}

	initialized_ = true;
}

QList<image_effect::Effect *> PluginCache::GetAllEffects() const
{
	assert(initialized_);

	QList<image_effect::Effect *> result;
	result.reserve(available_effects_.size());

	for (const auto &plugin : available_effects_) {
		result.push_back(plugin.get());
	}

	return result;
}

image_effect::Effect *PluginCache::FindPluginByIdentifier(const QString &ident,
							  int ver_maj,
							  int ver_min) const
{
	for (auto &effect : available_effects_) {
		if (effect->GetPluginIdentifier() == ident) {
			auto [eff_maj, eff_min] = effect->GetPluginVersion();
			if ((eff_maj == ver_maj || ver_maj == -1) &&
			    (eff_min == ver_min || ver_min == -1)) {
				return effect.get();
			}
		}
	}

	return nullptr;
}

void PluginCache::SearchForPlugins()
{
	for (auto &path : search_paths_) {
		QDirIterator it(path, QStringList() << "*.ofx.bundle",
				QDir::Dirs);

		while (it.hasNext()) {
			auto subdir_name = it.next();
			if (subdir_name.endsWith("/.") ||
			    subdir_name.endsWith("/..")) {
				continue;
			}

			// Spec says that if a directory begins with @ it should be skipped.
			if (subdir_name[0] == '@') {
				continue;
			}

			QDir subdir(subdir_name);
			qDebug() << "Traversing directory"
				 << subdir.absolutePath();

			ParseOfxDirectory(subdir);
		}
	}
}

size_t PluginCache::ParseOfxDirectory(QDir dir)
{
	size_t plugin_count = 0;

	// Get canonical path of this directory.
	const QString canonical_path = dir.canonicalPath();
	if (canonical_path.isEmpty()) {
		return 0;
	}

	// Navigate to the directory with ofx binaries.
	dir.cd("Contents");
	dir.cd(kOfxPluginArch);

	// Set proper filters to iterate only through .ofx binaries.
	dir.setFilter(QDir::Files);
	dir.setNameFilters(QStringList() << "*.ofx");

	// Create an OFX::PluginBinary for every valid ofx plugin.
	QFileInfoList list = dir.entryInfoList();
	plugin_count += list.size();

	for (QFileInfo &file_info : list) {
		qDebug() << "Discovered plugin" << file_info.absoluteFilePath();

		auto binary_ptr = std::make_unique<PluginBinary>(
			file_info.absoluteFilePath());
		available_binaries_.emplace_back(
			new PluginBinary(file_info.absoluteFilePath()));
	}

	return plugin_count;
}

PluginCache::PluginBinary::PluginBinary(const QString &binary_path)
	: binary_path_(binary_path), library_(binary_path)
{
	// OfxSetHost isn't defined in older versions of the API (pre-2020).
	fp_ofx_set_host_ = library_.resolve("OfxSetHost");

	fp_ofx_get_plugin_ = library_.resolve("OfxGetPlugin");
	assert(fp_ofx_get_plugin_ != nullptr);

	fp_ofx_get_number_of_plugins_ =
		library_.resolve("OfxGetNumberOfPlugins");
	assert(fp_ofx_get_number_of_plugins_ != nullptr);
}

bool PluginCache::PluginBinary::Initialize(const OfxHost *host)
{
	using OfnSetHostT = OfxStatus (*)(const OfxHost *);
	if (initialized_) {
		return should_load_;
	}

	initialized_ = true;

	if (fp_ofx_set_host_ != nullptr) {
		const auto set_host =
			reinterpret_cast<OfnSetHostT>(fp_ofx_set_host_);
		if (const OfxStatus status = set_host(host);
		    status == kOfxStatFailed) {
			should_load_ = false;
			return false;
		}
	}

	return true;
}

QList<OfxPlugin *> PluginCache::PluginBinary::GetAllPlugins() const
{
	QList<OfxPlugin *> plugins;
	const auto fn_get_num_plugins =
		reinterpret_cast<int (*)()>(fp_ofx_get_number_of_plugins_);
	const auto fn_get_plugin =
		reinterpret_cast<OfxPlugin *(*)(int)>(fp_ofx_get_plugin_);

	const int num_plugins = fn_get_num_plugins();
	for (int i = 0; i < num_plugins; i++) {
		OfxPlugin *plugin = fn_get_plugin(i);
		plugins.append(plugin);
	}

	return plugins;
}
