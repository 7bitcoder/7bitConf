#pragma once

#include "SevenBit/Conf/CmakeDef.hpp"

#ifndef _7BIT_CONF_VERSION

#define _7BIT_CONF_VERSION "0.0.0"

#endif

#if !defined _7BIT_CONF_SHARED_LIB && !defined _7BIT_CONF_STATIC_LIB && !defined _7BIT_CONF_HEADER_ONLY_LIB

#define _7BIT_CONF_HEADER_ONLY_LIB

#endif

#ifdef _7BIT_CONF_HEADER_ONLY_LIB

#undef _7BIT_CONF_SHARED_LIB
#undef _7BIT_CONF_STATIC_LIB

#define _7BIT_CONF_ADD_IMPL
#define INLINE inline

#else

#define INLINE

#endif

#include "SevenBit/Conf/Export.hpp"
