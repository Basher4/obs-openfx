#pragma once

#include <QDir>
#include <QLibrary>
#include <QList>
#include <QString>

#include "ofx/image-effect/effect.h"
#include "openfx/ofxCore.h"

namespace ofx {
class PluginCache {
public:
	PluginCache(const QStringList &additional_paths = {});

	~PluginCache();

	void InitializePlugins(OfxHost *host);

	QList<image_effect::Effect *> GetAllEffects() const;
	image_effect::Effect *FindPluginByIdentifier(const QString &ident,
						     int ver_maj = -1,
						     int ver_min = -1) const;

private: // ----- Private functions
	void SearchForPlugins();
	size_t ParseOfxDirectory(QDir dir);

private: // ----- Private types
	class PluginBinary {

	public:
		explicit PluginBinary(const QString &binary_path);

		bool Initialize(const OfxHost *host);
		QList<OfxPlugin *> GetAllPlugins() const;

	private:
		bool initialized_ = false;
		bool should_load_ = true;
		QString binary_path_;

		QLibrary library_;
		QFunctionPointer fp_ofx_get_number_of_plugins_;
		QFunctionPointer fp_ofx_get_plugin_;
		QFunctionPointer fp_ofx_set_host_;
	};

private: // ----- Private variables
	bool initialized_ = false;
	QStringList search_paths_;
	std::vector<std::unique_ptr<PluginBinary>> available_binaries_;
	std::vector<std::unique_ptr<image_effect::Effect>> available_effects_;
};
}
