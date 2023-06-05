#pragma once

#ifndef _7BIT_CONFIG_SHARED_LIB
/* #undef _7BIT_CONFIG_SHARED_LIB */
#endif

#ifndef _7BIT_CONFIG_STATIC_LIB
#define _7BIT_CONFIG_STATIC_LIB
#endif

#ifndef _7BIT_CONFIG_HEADER_ONLY_LIB
/* #undef _7BIT_CONFIG_HEADER_ONLY_LIB */
#endif


#define _7BIT_CONFIG_VERSION_MAJOR 1
/* #undef _7BIT_CONFIG_VERSION_MINOR */
/* #undef _7BIT_CONFIG_VERSION_PATCH */

#define _7BIT_CONFIG_VERSION "1.0.0"

#ifndef _7BIT_CONFIG_VERSION_MAJOR
#define _7BIT_CONFIG_VERSION_MAJOR 0
#endif

#ifndef _7BIT_CONFIG_VERSION_MINOR
#define _7BIT_CONFIG_VERSION_MINOR 0
#endif

#ifndef _7BIT_CONFIG_VERSION_PATCH
#define _7BIT_CONFIG_VERSION_PATCH 0
#endif

#define _7BIT_CONFIG_VERSION_AS_NUMBER                                                                                           \
    (_7BIT_CONFIG_VERSION_MAJOR * 10000 + _7BIT_CONFIG_VERSION_MINOR * 100 + _7BIT_CONFIG_VERSION_PATCH)