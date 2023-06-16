#include "ofx/suites/parameter-suite.h"
using namespace ofx::suites::v1;

template<typename T>
Parameter<T>::Parameter(const QString &name, const QString &type) : name_(name)
{
}

PropertySet *ParameterSet::DefineParameter(const QString &name,
                                           const QString &type)
{
	parameters_in_order_.append(name);
	return parameters_.emplace(name, new PropertySet).first->second.get();
}

std::optional<PropertySet *>
ParameterSet::GetParameterByName(const QString &name)
{
	if (parameters_.find(name) != parameters_.end())
		return parameters_[name].get();
	return std::nullopt;
}

OfxStatus ParameterSuite::SfnParamDefine(OfxParamSetHandle paramSet,
					 const char *paramType,
					 const char *name,
					 OfxPropertySetHandle *propertySet)
{
	auto *params = reinterpret_cast<ParameterSet *>(paramSet);
	*propertySet = *params->DefineParameter(name, paramType);
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SfnParamGetHandle(OfxParamSetHandle paramSet,
					    const char *name,
					    OfxParamHandle *param,
					    OfxPropertySetHandle *propertySet)
{
	auto *params = reinterpret_cast<ParameterSet *>(paramSet);
	*propertySet = *params->GetParameterByName(name).value();
	*param = reinterpret_cast<OfxParamHandle>(*propertySet);
	return kOfxStatOK;
}

OfxStatus
ParameterSuite::SfnParamSetGetPropertySet(OfxParamSetHandle paramSet,
					  OfxPropertySetHandle *propHandle)
{
	return kOfxStatErrFatal;
}

OfxStatus
ParameterSuite::SfnParamGetPropertySet(OfxParamHandle param,
				       OfxPropertySetHandle *propHandle)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamGetValue(OfxParamHandle paramHandle, ...)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamGetValueAtTime(OfxParamHandle paramHandle,
						 OfxTime time, ...)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamGetDerivative(OfxParamHandle paramHandle,
						OfxTime time, ...)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamGetIntegral(OfxParamHandle paramHandle,
					      OfxTime time1, OfxTime time2, ...)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamSetValue(OfxParamHandle paramHandle, ...)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamSetValueAtTime(OfxParamHandle paramHandle,
						 OfxTime time, ...)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamGetNumKeys(OfxParamHandle paramHandle,
					     unsigned int *numberOfKeys)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamGetKeyTime(OfxParamHandle paramHandle,
					     unsigned int nthKey, OfxTime *time)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamGetKeyIndex(OfxParamHandle paramHandle,
					      OfxTime time, int direction,
					      int *index)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamDeleteKey(OfxParamHandle paramHandle,
					    OfxTime time)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamDeleteAllKeys(OfxParamHandle paramHandle)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamCopy(OfxParamHandle paramTo,
				       OfxParamHandle paramFrom,
				       OfxTime dstOffset,
				       const OfxRangeD *frameRange)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamEditBegin(OfxParamSetHandle paramSet,
					    const char *name)
{
	return kOfxStatErrFatal;
}

OfxStatus ParameterSuite::SfnParamEditEnd(OfxParamSetHandle paramSet)
{
	return kOfxStatErrFatal;
}
