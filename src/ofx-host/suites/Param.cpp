#include <ofx-host/suites/Param.h>
using namespace ofx::suites::v1;

OfxStatus ParameterSuite::SFN_paramDefine(OfxParamSetHandle paramSet,
					  const char *paramType,
					  const char *name,
					  OfxPropertySetHandle *propertySet)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramGetHandle(OfxParamSetHandle paramSet,
					     const char *name,
					     OfxParamHandle *param,
					     OfxPropertySetHandle *propertySet)
{
	return kOfxStatOK;
}

OfxStatus
ParameterSuite::SFN_paramSetGetPropertySet(OfxParamSetHandle paramSet,
					   OfxPropertySetHandle *propHandle)
{
	return kOfxStatOK;
}

OfxStatus
ParameterSuite::SFN_paramGetPropertySet(OfxParamHandle param,
					OfxPropertySetHandle *propHandle)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramGetValue(OfxParamHandle paramHandle, ...)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramGetValueAtTime(OfxParamHandle paramHandle,
						  OfxTime time, ...)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramGetDerivative(OfxParamHandle paramHandle,
						 OfxTime time, ...)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramGetIntegral(OfxParamHandle paramHandle,
					       OfxTime time1, OfxTime time2,
					       ...)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramSetValue(OfxParamHandle paramHandle, ...)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramSetValueAtTime(OfxParamHandle paramHandle,
						  OfxTime time, ...)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramGetNumKeys(OfxParamHandle paramHandle,
					      unsigned int *numberOfKeys)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramGetKeyTime(OfxParamHandle paramHandle,
					      unsigned int nthKey,
					      OfxTime *time)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramGetKeyIndex(OfxParamHandle paramHandle,
					       OfxTime time, int direction,
					       int *index)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramDeleteKey(OfxParamHandle paramHandle,
					     OfxTime time)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramDeleteAllKeys(OfxParamHandle paramHandle)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramCopy(OfxParamHandle paramTo,
					OfxParamHandle paramFrom,
					OfxTime dstOffset,
					const OfxRangeD *frameRange)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramEditBegin(OfxParamSetHandle paramSet,
					     const char *name)
{
	return kOfxStatOK;
}

OfxStatus ParameterSuite::SFN_paramEditEnd(OfxParamSetHandle paramSet)
{
	return kOfxStatOK;
}
