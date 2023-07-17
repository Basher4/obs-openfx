#include "ofx/suites/property-suite.h"
using namespace ofx::suites::v1;

void PropertySet::Set(const QString &name, const int value, const int index)
{
	InsertScalar(ints_, name, value, PropertyType::INT, index);
}

void PropertySet::Set(const QString &name, const double value, const int index)
{
	InsertScalar(doubles_, name, value, PropertyType::DOUBLE, index);
}

void PropertySet::Set(const QString &name, const std::string &value,
		      const int index)
{
	InsertScalar(strings_, name, value, PropertyType::STRING, index);
}

void PropertySet::Set(const QString &name, void *value, const int index)
{
	InsertScalar(pointers_, name, value, PropertyType::POINTER, index);
}

void PropertySet::Set(const QString &name, std::initializer_list<int> values)
{
	InsertList(ints_, name, PropertyType::INT, values);
}

void PropertySet::Set(const QString &name, std::initializer_list<double> values)
{
	InsertList(doubles_, name, PropertyType::DOUBLE, values);
}

void PropertySet::Set(const QString &name,
		      std::initializer_list<std::string> values)
{
	InsertList(strings_, name, PropertyType::STRING, values);
}

void PropertySet::Set(const QString &name, std::initializer_list<void *> values)
{
	InsertList(pointers_, name, PropertyType::POINTER, values);
}

QPair<OfxStatus, int> PropertySet::GetInt(const QString &name, int index)
{
	return GetProperty(ints_, name, index);
}

QPair<OfxStatus, double> PropertySet::GetDouble(const QString &name, int index)
{
	return GetProperty(doubles_, name, index);
}

QPair<OfxStatus, const char *> PropertySet::GetString(const QString &name,
						      int index)
{
	if (!strings_.contains(name))
		return {kOfxStatErrValue, {}};
	if (strings_[name].size() < index)
		return {kOfxStatErrBadIndex, {}};
	return {kOfxStatOK, strings_[name][index].c_str()};
}

QPair<OfxStatus, void *> PropertySet::GetPtr(const QString &name, int index)
{
	return GetProperty(pointers_, name, index);
}

void PropertySet::Reset(const QString &name)
{
	if (MaybeReset(strings_, name))
		return;
	if (MaybeReset(pointers_, name))
		return;
	if (MaybeReset(doubles_, name))
		return;
	if (MaybeReset(ints_, name))
		return;
}

std::optional<int> PropertySet::GetDimension(const QString &name)
{
	if (strings_.contains(name))
		return static_cast<int>(strings_[name].size());
	if (pointers_.contains(name))
		return static_cast<int>(pointers_[name].size());
	if (doubles_.contains(name))
		return static_cast<int>(doubles_[name].size());
	if (ints_.contains(name))
		return static_cast<int>(ints_[name].size());
	return std::nullopt;
}

const QList<QPair<QString, PropertySet::PropertyType>> &
PropertySet::GetDefinedProperties() const
{
	return order_of_definition_;
}

template<typename T>
void PropertySet::InsertScalar(QHash<QString, QList<T>> &hashset,
			       const QString &name, const T value,
			       PropertyType type, int index)
{
	if (hashset.empty() || !hashset.contains(name))
		order_of_definition_.emplace_back(name, type);

	if (index < 0)
		index = static_cast<int>(hashset[name].size());
	hashset[name].insert(index, value);
}

template<typename T>
void PropertySet::InsertList(QHash<QString, QList<T>> &hashset,
			     const QString &name, PropertyType type,
			     std::initializer_list<T> value)
{
	if (hashset.empty() || !hashset.contains(name))
		order_of_definition_.emplace_back(name, type);

	hashset[name].append(value);
}

template<typename T>
QPair<OfxStatus, T>
PropertySet::GetProperty(const QHash<QString, QList<T>> &hashset,
			 const QString &name, int index)
{
	if (!hashset.contains(name))
		return {kOfxStatErrValue, {}};
	if (hashset[name].size() < index)
		return {kOfxStatErrBadIndex, {}};
	return {kOfxStatOK, hashset[name][index]};
}

template<typename T>
bool PropertySet::MaybeReset(QHash<QString, QList<T>> &hashset,
			     const QString &name)
{
	for (auto [key, val] : hashset.asKeyValueRange()) {
		if (key.compare(name) == 0) {
			val.clear();
			return true;
		}
	}

	return false;
}

OfxPropertySuiteV1 *PropertySuite::GetSuiteDesc()
{
	suite_.propSetPointer = SfnPropSetPointer;
	suite_.propSetString = SfnPropSetString;
	suite_.propSetDouble = SfnPropSetDouble;
	suite_.propSetInt = SfnPropSetInt;
	suite_.propSetPointerN = SfnPropSetPointerN;
	suite_.propSetStringN = SfnPropSetStringN;
	suite_.propSetDoubleN = SfnPropSetDoubleN;
	suite_.propSetIntN = SfnPropSetIntN;
	suite_.propGetPointer = SfnPropGetPointer;
	suite_.propGetString = SfnPropGetString;
	suite_.propGetDouble = SfnPropGetDouble;
	suite_.propGetInt = SfnPropGetInt;
	suite_.propGetPointerN = SfnPropGetPointerN;
	suite_.propGetStringN = SfnPropGetStringN;
	suite_.propGetDoubleN = SfnPropGetDoubleN;
	suite_.propGetIntN = SfnPropGetIntN;
	suite_.propReset = SfnPropReset;
	suite_.propGetDimension = SfnPropGetDimension;
	return &suite_;
}

OfxStatus PropertySuite::SfnPropSetPointer(OfxPropertySetHandle properties,
					   const char *property, int index,
					   void *value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	prop_set->Set(property, value, index);
	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropSetString(OfxPropertySetHandle properties,
					  const char *property, int index,
					  const char *value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	prop_set->Set(property, value, index);
	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropSetDouble(OfxPropertySetHandle properties,
					  const char *property, int index,
					  double value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	prop_set->Set(property, value, index);
	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropSetInt(OfxPropertySetHandle properties,
				       const char *property, int index,
				       int value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	prop_set->Set(property, value, index);
	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropSetPointerN(OfxPropertySetHandle properties,
					    const char *property, int count,
					    void *const *value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	for (int i = 0; i < count; i++)
		prop_set->Set(property, value[i]);
	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropSetStringN(OfxPropertySetHandle properties,
					   const char *property, int count,
					   const char *const *value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	for (int i = 0; i < count; i++)
		prop_set->Set(property, value[i]);
	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropSetDoubleN(OfxPropertySetHandle properties,
					   const char *property, int count,
					   const double *value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	for (int i = 0; i < count; i++)
		prop_set->Set(property, value[i]);
	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropSetIntN(OfxPropertySetHandle properties,
					const char *property, int count,
					const int *value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	for (int i = 0; i < count; i++)
		prop_set->Set(property, value[i]);
	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropGetPointer(OfxPropertySetHandle properties,
					   const char *property, int index,
					   void **value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	auto [status, res] = prop_set->GetPtr(property, index);
	if (status == kOfxStatOK)
		*value = res;
	return status;
}

OfxStatus PropertySuite::SfnPropGetString(OfxPropertySetHandle properties,
					  const char *property, int index,
					  char **value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	auto [status, res] = prop_set->GetString(property, index);
	if (status == kOfxStatOK)
		*value = const_cast<char *>(res); // TODO: Is this ok?
	return status;
}

OfxStatus PropertySuite::SfnPropGetDouble(OfxPropertySetHandle properties,
					  const char *property, int index,
					  double *value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	auto [status, res] = prop_set->GetDouble(property, index);
	if (status == kOfxStatOK)
		*value = res;
	return status;
}

OfxStatus PropertySuite::SfnPropGetInt(OfxPropertySetHandle properties,
				       const char *property, int index,
				       int *value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	auto [status, res] = prop_set->GetInt(property, index);
	if (status == kOfxStatOK)
		*value = res;
	return status;
}

OfxStatus PropertySuite::SfnPropGetPointerN(OfxPropertySetHandle properties,
					    const char *property, int count,
					    void **value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);

	for (int i = 0; i < count; i++) {
		auto [status, res] = prop_set->GetPtr(property, i);
		if (status != kOfxStatOK)
			return status;
		value[i] = res;
	}

	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropGetStringN(OfxPropertySetHandle properties,
					   const char *property, int count,
					   char **value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);

	for (int i = 0; i < count; i++) {
		auto [status, res] = prop_set->GetString(property, i);
		if (status != kOfxStatOK)
			return status;
		value[i] = const_cast<char *>(res);
	}

	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropGetDoubleN(OfxPropertySetHandle properties,
					   const char *property, int count,
					   double *value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);

	for (int i = 0; i < count; i++) {
		auto [status, res] = prop_set->GetDouble(property, i);
		if (status != kOfxStatOK)
			return status;
		value[i] = res;
	}

	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropGetIntN(OfxPropertySetHandle properties,
					const char *property, int count,
					int *value)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);

	for (int i = 0; i < count; i++) {
		auto [status, res] = prop_set->GetInt(property, i);
		if (status != kOfxStatOK)
			return status;
		value[i] = res;
	}

	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropReset(OfxPropertySetHandle properties,
				      const char *property)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	prop_set->Reset(property);
	return kOfxStatOK;
}

OfxStatus PropertySuite::SfnPropGetDimension(OfxPropertySetHandle properties,
					     const char *property, int *count)
{
	const auto prop_set = reinterpret_cast<PropertySet *>(properties);
	if (const auto dim = prop_set->GetDimension(property);
	    dim.has_value()) {
		*count = dim.value();
		return kOfxStatOK;
	}

	return kOfxStatErrValue;
}
