#pragma once

#include <openfx/ofxCore.h>
#include <openfx/ofxParam.h>

#include <QHash>

namespace ofx::suites::v1 {
class ParameterSet {
	
};

class ParameterSuite {
public:
	static const OfxParameterSuiteV1 *GetSuiteDesc()
	{
		m_Suite.paramDefine = SFN_paramDefine;
		m_Suite.paramGetHandle = SFN_paramGetHandle;
		m_Suite.paramSetGetPropertySet = SFN_paramSetGetPropertySet;
		m_Suite.paramGetPropertySet = SFN_paramGetPropertySet;
		m_Suite.paramGetValue = SFN_paramGetValue;
		m_Suite.paramGetValueAtTime = SFN_paramGetValueAtTime;
		m_Suite.paramGetDerivative = SFN_paramGetDerivative;
		m_Suite.paramGetIntegral = SFN_paramGetIntegral;
		m_Suite.paramSetValue = SFN_paramSetValue;
		m_Suite.paramSetValueAtTime = SFN_paramSetValueAtTime;
		m_Suite.paramGetNumKeys = SFN_paramGetNumKeys;
		m_Suite.paramGetKeyTime = SFN_paramGetKeyTime;
		m_Suite.paramGetKeyIndex = SFN_paramGetKeyIndex;
		m_Suite.paramDeleteKey = SFN_paramDeleteKey;
		m_Suite.paramDeleteAllKeys = SFN_paramDeleteAllKeys;
		m_Suite.paramCopy = SFN_paramCopy;
		m_Suite.paramEditBegin = SFN_paramEditBegin;
		m_Suite.paramEditEnd = SFN_paramEditEnd;
		return &m_Suite;
	}

private:
	static OfxStatus SFN_paramDefine(OfxParamSetHandle paramSet,
					 const char *paramType,
					 const char *name,
					 OfxPropertySetHandle *propertySet);

	static OfxStatus SFN_paramGetHandle(OfxParamSetHandle paramSet,
					    const char *name,
					    OfxParamHandle *param,
					    OfxPropertySetHandle *propertySet);

	static OfxStatus
	SFN_paramSetGetPropertySet(OfxParamSetHandle paramSet,
				   OfxPropertySetHandle *propHandle);

	static OfxStatus
	SFN_paramGetPropertySet(OfxParamHandle param,
				OfxPropertySetHandle *propHandle);

	static OfxStatus SFN_paramGetValue(OfxParamHandle paramHandle, ...);

	static OfxStatus SFN_paramGetValueAtTime(OfxParamHandle paramHandle,
						 OfxTime time, ...);

	static OfxStatus SFN_paramGetDerivative(OfxParamHandle paramHandle,
						OfxTime time, ...);

	static OfxStatus SFN_paramGetIntegral(OfxParamHandle paramHandle,
					      OfxTime time1, OfxTime time2,
					      ...);

	static OfxStatus SFN_paramSetValue(OfxParamHandle paramHandle, ...);

	static OfxStatus SFN_paramSetValueAtTime(OfxParamHandle paramHandle,
						 OfxTime time, ...);

	static OfxStatus SFN_paramGetNumKeys(OfxParamHandle paramHandle,
					     unsigned int *numberOfKeys);

	static OfxStatus SFN_paramGetKeyTime(OfxParamHandle paramHandle,
					     unsigned int nthKey,
					     OfxTime *time);

	static OfxStatus SFN_paramGetKeyIndex(OfxParamHandle paramHandle,
					      OfxTime time, int direction,
					      int *index);

	static OfxStatus SFN_paramDeleteKey(OfxParamHandle paramHandle,
					    OfxTime time);

	static OfxStatus SFN_paramDeleteAllKeys(OfxParamHandle paramHandle);

	static OfxStatus SFN_paramCopy(OfxParamHandle paramTo,
				       OfxParamHandle paramFrom,
				       OfxTime dstOffset,
				       const OfxRangeD *frameRange);

	static OfxStatus SFN_paramEditBegin(OfxParamSetHandle paramSet,
					    const char *name);

	static OfxStatus SFN_paramEditEnd(OfxParamSetHandle paramSet);

private:
	static OfxParameterSuiteV1 m_Suite;
};
};
