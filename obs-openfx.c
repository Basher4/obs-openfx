#include "names.h"
#include "ofx-obs/ofx-source.h"

#include <obs-module.h>
#include <util/base.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-openfx", "en-US")

bool obs_module_load(void)
{
	ofx_filter_init();

	obs_register_source(&ofx_filter);
	return true;
}

const char *obs_module_name(void)
{
	return obs_module_text(LOC_MODULE_NAME);
}
