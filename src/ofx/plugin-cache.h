#pragma once

#include <QList>
#include <QString>

#include "ofx/image-effect/plugin.h"
#include "openfx/ofxCore.h"

namespace ofx {
class PluginCache {
public:
	~PluginCache();

	void Rescan();
	void Rescan(QList<QString>& additional_paths);

	void InitializePlugins(OfxHost *host);

	const QList<image_effect::Plugin *> GetAllPlugins();

	image_effect::Plugin *FindPluginByIdentifier(const std::string &ident);

private:
	QList<QString> search_paths_;
	QList<std::unique_ptr<image_effect::Plugin>> available_plugins_;
};
}
