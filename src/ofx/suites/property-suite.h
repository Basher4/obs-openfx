#pragma once

#include <initializer_list>
#include <optional>
#include <unordered_map>
#include <string>

#include <QHash>
#include <QList>
#include <QPair>
#include <QStringList>

#include <openfx/ofxCore.h>
#include <openfx/ofxProperty.h>

namespace ofx::suites::v1 {
class PropertySet {
public:
	void Set(const QString &name, int value, int index = -1);
	void Set(const QString &name, double value, int index = -1);
	void Set(const QString &name, const std::string &value, int index = -1);
	void Set(const QString &name, void *value, int index = -1);
	void Set(const QString &name, std::initializer_list<int> values);
	void Set(const QString &name, std::initializer_list<double> values);
	void Set(const QString &name,
		 std::initializer_list<std::string> values);
	void Set(const QString &name, std::initializer_list<void *> values);

	QPair<OfxStatus, int> GetInt(const QString &name, int index = 0);
	QPair<OfxStatus, double> GetDouble(const QString &name, int index = 0);
	QPair<OfxStatus, const char *> GetString(const QString &name,
						 int index = 0);
	QPair<OfxStatus, void *> GetPtr(const QString &name, int index = 0);

	void Reset(const QString &name);
	std::optional<int> GetDimension(const QString &name);
	const QStringList &GetDefinedProperties() const;

	operator OfxPropertySetHandle()
	{
		return reinterpret_cast<OfxPropertySetHandle>(this);
	}

	operator OfxPropertySetHandle() const
	{
		return reinterpret_cast<OfxPropertySetHandle>(
			const_cast<PropertySet *>(this));
	}

private:
	template<typename T>
	void InsertScalar(QHash<QString, QList<T>> &hashset,
			  const QString &name, const T value, int index = -1);

	template<typename T>
	void InsertList(QHash<QString, QList<T>> &hashset, const QString &name,
			std::initializer_list<T> value);

	template<typename T>
	QPair<OfxStatus, T> GetProperty(const QHash<QString, QList<T>> &hashset,
					const QString &name, int index);

	template<typename T>
	bool MaybeReset(QHash<QString, QList<T>> &hashset, const QString &name);

private:
	QHash<QString, QList<int>> ints_{};
	QHash<QString, QList<double>> doubles_{};
	QHash<QString, QList<std::string>> strings_{};
	QHash<QString, QList<void *>> pointers_{};
	QStringList order_of_definition_;
};

class PropertySuite {
public:
	static OfxPropertySuiteV1 *GetSuiteDesc();
	static const char *GetSuiteName() { return kOfxPropertySuite; }

private:
	static OfxStatus SfnPropSetPointer(OfxPropertySetHandle properties,
					   const char *property, int index,
					   void *value);
	static OfxStatus SfnPropSetString(OfxPropertySetHandle properties,
					  const char *property, int index,
					  const char *value);
	static OfxStatus SfnPropSetDouble(OfxPropertySetHandle properties,
					  const char *property, int index,
					  double value);
	static OfxStatus SfnPropSetInt(OfxPropertySetHandle properties,
				       const char *property, int index,
				       int value);
	static OfxStatus SfnPropSetPointerN(OfxPropertySetHandle properties,
					    const char *property, int count,
					    void *const *value);
	static OfxStatus SfnPropSetStringN(OfxPropertySetHandle properties,
					   const char *property, int count,
					   const char *const *value);
	static OfxStatus SfnPropSetDoubleN(OfxPropertySetHandle properties,
					   const char *property, int count,
					   const double *value);
	static OfxStatus SfnPropSetIntN(OfxPropertySetHandle properties,
					const char *property, int count,
					const int *value);
	static OfxStatus SfnPropGetPointer(OfxPropertySetHandle properties,
					   const char *property, int index,
					   void **value);
	static OfxStatus SfnPropGetString(OfxPropertySetHandle properties,
					  const char *property, int index,
					  char **value);
	static OfxStatus SfnPropGetDouble(OfxPropertySetHandle properties,
					  const char *property, int index,
					  double *value);
	static OfxStatus SfnPropGetInt(OfxPropertySetHandle properties,
				       const char *property, int index,
				       int *value);
	static OfxStatus SfnPropGetPointerN(OfxPropertySetHandle properties,
					    const char *property, int count,
					    void **value);
	static OfxStatus SfnPropGetStringN(OfxPropertySetHandle properties,
					   const char *property, int count,
					   char **value);
	static OfxStatus SfnPropGetDoubleN(OfxPropertySetHandle properties,
					   const char *property, int count,
					   double *value);
	static OfxStatus SfnPropGetIntN(OfxPropertySetHandle properties,
					const char *property, int count,
					int *value);
	static OfxStatus SfnPropReset(OfxPropertySetHandle properties,
				      const char *property);
	static OfxStatus SfnPropGetDimension(OfxPropertySetHandle properties,
					     const char *property, int *count);

private:
	inline static OfxPropertySuiteV1 suite_;
};
}
