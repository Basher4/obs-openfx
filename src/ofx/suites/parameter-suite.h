#pragma once

#include <unordered_map>
#include <QList>
#include <openfx/ofxCore.h>
#include <openfx/ofxParam.h>

#include "ofx/suites/property-suite.h"

namespace ofx::suites::v1 {
template<typename T> class Parameter {
	Parameter(const QString &name, const QString &type);

private:
	QString name_;
	PropertySet properties_;
	T value_;
};

class ParameterSet {
public:
	PropertySet *DefineParameter(const QString &name,
	                             const QString &type);
	std::optional<PropertySet *>
	GetParameterByName(const QString &name);

	const QStringList &GetDefinedParameters()
	{
		return parameters_in_order_;
	}

	operator OfxParamSetHandle()
	{
		return reinterpret_cast<OfxParamSetHandle>(this);
	}

private:
	std::unordered_map<QString, std::unique_ptr<PropertySet>> parameters_;
	QStringList parameters_in_order_;
};

class ParameterSuite {
public:
	static const OfxParameterSuiteV1 *GetSuiteDesc()
	{
		suite_.paramDefine = SfnParamDefine;
		suite_.paramGetHandle = SfnParamGetHandle;
		suite_.paramSetGetPropertySet = SfnParamSetGetPropertySet;
		suite_.paramGetPropertySet = SfnParamGetPropertySet;
		suite_.paramGetValue = SfnParamGetValue;
		suite_.paramGetValueAtTime = SfnParamGetValueAtTime;
		suite_.paramGetDerivative = SfnParamGetDerivative;
		suite_.paramGetIntegral = SfnParamGetIntegral;
		suite_.paramSetValue = SfnParamSetValue;
		suite_.paramSetValueAtTime = SfnParamSetValueAtTime;
		suite_.paramGetNumKeys = SfnParamGetNumKeys;
		suite_.paramGetKeyTime = SfnParamGetKeyTime;
		suite_.paramGetKeyIndex = SfnParamGetKeyIndex;
		suite_.paramDeleteKey = SfnParamDeleteKey;
		suite_.paramDeleteAllKeys = SfnParamDeleteAllKeys;
		suite_.paramCopy = SfnParamCopy;
		suite_.paramEditBegin = SfnParamEditBegin;
		suite_.paramEditEnd = SfnParamEditEnd;
		return &suite_;
	}
	static const char *GetSuiteName() { return kOfxParameterSuite; }

private:
	static OfxStatus SfnParamDefine(OfxParamSetHandle paramSet,
					const char *paramType, const char *name,
					OfxPropertySetHandle *propertySet);
	static OfxStatus SfnParamGetHandle(OfxParamSetHandle paramSet,
					   const char *name,
					   OfxParamHandle *param,
					   OfxPropertySetHandle *propertySet);
	static OfxStatus
	SfnParamSetGetPropertySet(OfxParamSetHandle paramSet,
				  OfxPropertySetHandle *propHandle);
	static OfxStatus
	SfnParamGetPropertySet(OfxParamHandle param,
			       OfxPropertySetHandle *propHandle);
	static OfxStatus SfnParamGetValue(OfxParamHandle paramHandle, ...);
	static OfxStatus SfnParamGetValueAtTime(OfxParamHandle paramHandle,
						OfxTime time, ...);
	static OfxStatus SfnParamGetDerivative(OfxParamHandle paramHandle,
					       OfxTime time, ...);
	static OfxStatus SfnParamGetIntegral(OfxParamHandle paramHandle,
					     OfxTime time1, OfxTime time2, ...);
	static OfxStatus SfnParamSetValue(OfxParamHandle paramHandle, ...);
	static OfxStatus SfnParamSetValueAtTime(OfxParamHandle paramHandle,
						OfxTime time, ...);
	static OfxStatus SfnParamGetNumKeys(OfxParamHandle paramHandle,
					    unsigned int *numberOfKeys);
	static OfxStatus SfnParamGetKeyTime(OfxParamHandle paramHandle,
					    unsigned int nthKey, OfxTime *time);
	static OfxStatus SfnParamGetKeyIndex(OfxParamHandle paramHandle,
					     OfxTime time, int direction,
					     int *index);
	static OfxStatus SfnParamDeleteKey(OfxParamHandle paramHandle,
					   OfxTime time);
	static OfxStatus SfnParamDeleteAllKeys(OfxParamHandle paramHandle);
	static OfxStatus SfnParamCopy(OfxParamHandle paramTo,
				      OfxParamHandle paramFrom,
				      OfxTime dstOffset,
				      const OfxRangeD *frameRange);
	static OfxStatus SfnParamEditBegin(OfxParamSetHandle paramSet,
					   const char *name);
	static OfxStatus SfnParamEditEnd(OfxParamSetHandle paramSet);

private:
	inline static OfxParameterSuiteV1 suite_;
};
};
