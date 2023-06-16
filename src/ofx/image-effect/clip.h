#pragma once
#include <QString>

#include "ofx/suites/parameter-suite.h"
#include "ofx/suites/property-suite.h"
#include "openfx/ofxCore.h"
#include "openfx/ofxImageEffect.h"
#include "openfx/ofxParam.h"

namespace ofx::image_effect {
class Clip {
public:
	explicit Clip(const QString &name);

	[[nodiscard]] const QString &GetClipName() const;
	[[nodiscard]] OfxPropertySetHandle GetPropertySet();
	[[nodiscard]] OfxParamSetHandle GetParameterSet();

	operator OfxImageClipHandle()
	{
		return reinterpret_cast<OfxImageClipHandle>(this);
	}

private:
	QString name_;
	suites::v1::ParameterSet parameter_set_;
	suites::v1::PropertySet property_set_;
};
}
