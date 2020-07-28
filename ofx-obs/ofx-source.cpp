#include "ofx-source.h"

#include <obs-module.h>
#include <obs-source.h>

struct obs_source_info ofx_filter;

void ofx_filter_init(void)
{
	ofx_filter.id = "ofx_host";
	ofx_filter.type = OBS_SOURCE_TYPE_FILTER;
	ofx_filter.output_flags = OBS_SOURCE_VIDEO;
}
