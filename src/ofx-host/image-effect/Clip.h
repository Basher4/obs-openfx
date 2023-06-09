#pragma once

#include <QHash>

#include <ofx-host/suites/Param.h>
#include <ofx-host/suites/Property.h>

namespace ofx::image_effect {
class Clip {
public:
	ofx::suites::v1::ParameterSet m_ClipParameters;
};

class ClipCollection {
public:
	ClipCollection() = default;

	OfxStatus DefineClip(const char *name, const Clip **out);

private:
	QHash<QString, Clip> m_ClipsMap;
};
};
