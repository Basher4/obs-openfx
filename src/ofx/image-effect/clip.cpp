#include "ofx/image-effect/clip.h"
using namespace ofx::image_effect;

Clip::Clip(const QString &name)
	:name_(name)
{
}

const QString & Clip::GetClipName() const
{
	return name_;
}

OfxPropertySetHandle Clip::GetPropertySet()
{
	return property_set_;
}

OfxParamSetHandle Clip::GetParameterSet()
{
	return parameter_set_;
}
