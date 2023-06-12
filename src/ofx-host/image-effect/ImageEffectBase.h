#pragma once

#include "openfx/ofxCore.h"
#include "openfx/ofxImageEffect.h"

#include "ofx-host/suites/Param.h"
#include "ofx-host/suites/Property.h"

#include <QString>

namespace ofx::image_effect {
class ImageEffectBase {
public:
	ImageEffectBase(const OfxPlugin *plugin);

	[[nodiscard]] const OfxPlugin *GetOfxPlugin() const { return m_Plugin; }
	[[nodiscard]] const char *GetPluginIdent() const
	{
		return m_PluginIdent.toLocal8Bit().data();
	}
	[[nodiscard]] const QString &GetPluginIdentQstring() const
	{
		return m_PluginIdent;
	}

	[[nodiscard]] OfxPropertySetHandle GetPropertySet()
	{
		return reinterpret_cast<OfxPropertySetHandle>(&m_PropertySet);
	}
	[[nodiscard]] OfxParamSetHandle GetParameterSet()
	{
		return reinterpret_cast<OfxParamSetHandle>(&m_ParameterSet);
	}

protected:
	const OfxPlugin *m_Plugin;
	const QString m_PluginIdent;

	suites::v1::ParameterSet m_ParameterSet;
	suites::v1::PropertySet m_PropertySet;
};
}
