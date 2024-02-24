#pragma once

#include "SevenBit/Conf/CmakeDef.hpp"

#ifndef _7BIT_CONF_VERSION_MAJOR
#define _7BIT_CONF_VERSION_MAJOR 0
#endif

#ifndef _7BIT_CONF_VERSION_MINOR
#define _7BIT_CONF_VERSION_MINOR 0
#endif

#ifndef _7BIT_CONF_VERSION_PATCH
#define _7BIT_CONF_VERSION_PATCH 0
#endif

#define _7BIT_CONF_VERSION_AS_NUMBER                                                                                   \
    (_7BIT_CONF_VERSION_MAJOR * 10000 + _7BIT_CONF_VERSION_MINOR * 100 + _7BIT_CONF_VERSION_PATCH)

#define _7BIT_CONF_MACRO_TO_STRING(macro) _7BIT_CONF_TO_STRING(macro)
#define _7BIT_CONF_TO_STRING(value) #value

#define _7BIT_CONF_VERSION                                                                                             \
    _7BIT_CONF_MACRO_TO_STRING(_7BIT_CONF_VERSION_MAJOR)                                                               \
    "." _7BIT_CONF_MACRO_TO_STRING(_7BIT_CONF_VERSION_MINOR) "." _7BIT_CONF_MACRO_TO_STRING(_7BIT_CONF_VERSION_PATCH)

#if defined _7BIT_CONF_STATIC_LIB

#undef _7BIT_CONF_SHARED_LIB
#undef _7BIT_CONF_HEADER_ONLY_LIB

#define INLINE

#elif defined _7BIT_CONF_SHARED_LIB

#undef _7BIT_CONF_STATIC_LIB
#undef _7BIT_CONF_HEADER_ONLY_LIB

#define INLINE

#else

#ifndef _7BIT_CONF_HEADER_ONLY_LIB
#define _7BIT_CONF_HEADER_ONLY_LIB
#endif

#undef _7BIT_CONF_SHARED_LIB
#undef _7BIT_CONF_STATIC_LIB

#define _7BIT_CONF_ADD_IMPL
#define INLINE inline

#endif

#include "SevenBit/Conf/Export.hpp"
