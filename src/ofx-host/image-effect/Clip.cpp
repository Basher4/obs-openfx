#include <ofx-host/image-effect/Clip.h>
using namespace ofx::image_effect;

OfxStatus ClipCollection::DefineClip(const char *name, const Clip **out)
{
	const QString key(name);
	if (m_ClipsMap.contains(key))
		return kOfxStatErrExists;

	*out = &(*m_ClipsMap.emplace(key));
	return kOfxStatOK;
}
