#pragma once

#include <obs-module.h>

#define ofx_log(level, format, ...)         \
    blog(level, "[OfxHost] " format, ##__VA_ARGS__)

#define ofx_debug(format, ...) ofx_log(LOG_DEBUG, format, ##__VA_ARGS__)
#define ofx_info(format, ...) ofx_log(LOG_INFO, format, ##__VA_ARGS__)
#define ofx_warning(format, ...) ofx_log(LOG_WARNING, format, ##__VA_ARGS__)
#define ofx_error(format, ...) ofx_log(LOG_ERROR, format, ##__VA_ARGS__)
#define ofx_crash(format, ...) bcrash("[OfxHost] " format, ##__VA_ARGS__)
