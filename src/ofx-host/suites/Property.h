#pragma once

#include <initializer_list>
#include <string_view>
#include <QHash>
#include <QList>

#include <openfx/ofxCore.h>
#include <openfx/ofxProperty.h>

namespace ofx::suites::v1 {
class PropertySet {
public:
	QHash<QString, QList<int>> ints;
	QHash<QString, QList<double>> doubles;
	QHash<QString, QList<QString>> strings;
	QHash<QString, QList<void *>> pointers;

	void set_property(const QString &name, const int value)
	{
		const QList vec{value};
		ints.insert(name, vec);
	}
	void set_property(const QString &name, const double value)
	{
		const QList vec{value};
		doubles.insert(name, vec);
	}
	void set_property(const QString &name, const QString &value)
	{
		const QList vec{QString(value)};
		strings.insert(name, vec);
	}
	void set_property(const QString &name, void *value)
	{
		const QList vec{value};
		pointers.insert(name, vec);
	}

	void set_property(const QString &name,
			    std::initializer_list<int> values)
	{
		const QList vec(values);
		ints.insert(name, vec);
	}
	void set_property(const QString &name,
			    std::initializer_list<double> values)
	{
		const QList vec(values);
		doubles.insert(name, vec);
	}

	void set_property(const QString &name,
			    std::initializer_list<QString> values)
	{
		const QList vec(values);
		strings.insert(name, vec);
	}
	void set_property(const QString &name,
			    std::initializer_list<void *> values)
	{
		const QList vec(values);
		pointers.insert(name, vec);
	}
};

class PropertySuite {
public:
	static OfxPropertySuiteV1 *get_suite_desc()
	{
		m_Suite.propSetPointer = sfn_propSetPointer;
		m_Suite.propSetString = sfn_propSetString;
		m_Suite.propSetDouble = sfn_propSetDouble;
		m_Suite.propSetInt = sfn_propSetInt;
		m_Suite.propSetPointerN = sfn_propSetPointerN;
		m_Suite.propSetStringN = sfn_propSetStringN;
		m_Suite.propSetDoubleN = sfn_propSetDoubleN;
		m_Suite.propSetIntN = sfn_propSetIntN;
		m_Suite.propGetPointer = sfn_propGetPointer;
		m_Suite.propGetString = sfn_propGetString;
		m_Suite.propGetDouble = sfn_propGetDouble;
		m_Suite.propGetInt = sfn_propGetInt;
		m_Suite.propGetPointerN = sfn_propGetPointerN;
		m_Suite.propGetStringN = sfn_propGetStringN;
		m_Suite.propGetDoubleN = sfn_propGetDoubleN;
		m_Suite.propGetIntN = sfn_propGetIntN;
		m_Suite.propReset = sfn_propReset;
		m_Suite.propGetDimension = sfn_propGetDimension;
		return &m_Suite;
	}

private:
	static OfxStatus sfn_propSetPointer(OfxPropertySetHandle properties,
					    const char *property, int index,
					    void *value);
	static OfxStatus sfn_propSetString(OfxPropertySetHandle properties,
					   const char *property, int index,
					   const char *value);
	static OfxStatus sfn_propSetDouble(OfxPropertySetHandle properties,
					   const char *property, int index,
					   double value);
	static OfxStatus sfn_propSetInt(OfxPropertySetHandle properties,
					const char *property, int index,
					int value);
	static OfxStatus sfn_propSetPointerN(OfxPropertySetHandle properties,
					     const char *property, int count,
					     void *const *value);
	static OfxStatus sfn_propSetStringN(OfxPropertySetHandle properties,
					    const char *property, int count,
					    const char *const *value);
	static OfxStatus sfn_propSetDoubleN(OfxPropertySetHandle properties,
					    const char *property, int count,
					    const double *value);
	static OfxStatus sfn_propSetIntN(OfxPropertySetHandle properties,
					 const char *property, int count,
					 const int *value);
	static OfxStatus sfn_propGetPointer(OfxPropertySetHandle properties,
					    const char *property, int index,
					    void **value);
	static OfxStatus sfn_propGetString(OfxPropertySetHandle properties,
					   const char *property, int index,
					   char **value);
	static OfxStatus sfn_propGetDouble(OfxPropertySetHandle properties,
					   const char *property, int index,
					   double *value);
	static OfxStatus sfn_propGetInt(OfxPropertySetHandle properties,
					const char *property, int index,
					int *value);
	static OfxStatus sfn_propGetPointerN(OfxPropertySetHandle properties,
					     const char *property, int count,
					     void **value);
	static OfxStatus sfn_propGetStringN(OfxPropertySetHandle properties,
					    const char *property, int count,
					    char **value);
	static OfxStatus sfn_propGetDoubleN(OfxPropertySetHandle properties,
					    const char *property, int count,
					    double *value);
	static OfxStatus sfn_propGetIntN(OfxPropertySetHandle properties,
					 const char *property, int count,
					 int *value);
	static OfxStatus sfn_propReset(OfxPropertySetHandle properties,
				       const char *property);
	static OfxStatus sfn_propGetDimension(OfxPropertySetHandle properties,
					      const char *property, int *count);

private:
	static OfxPropertySuiteV1 m_Suite;
};
}
