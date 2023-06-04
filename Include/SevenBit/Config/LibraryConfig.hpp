#pragma once

#include "SevenBit/Config/CmakeDef.hpp"

#ifndef _7BIT_CONFIG_VERSION

#define _7BIT_CONFIG_VERSION "0.0.0"

#endif

#if !defined _7BIT_CONFIG_SHARED_LIB && !defined _7BIT_CONFIG_STATIC_LIB && !defined _7BIT_CONFIG_HEADER_ONLY_LIB

#define _7BIT_CONFIG_HEADER_ONLY_LIB

#endif

#ifdef _7BIT_CONFIG_HEADER_ONLY_LIB

#undef _7BIT_CONFIG_SHARED_LIB
#undef _7BIT_CONFIG_STATIC_LIB

#define _7BIT_CONFIG_ADD_IMPL
#define INLINE inline

#else

#define INLINE

#endif

#include "SevenBit/Config/Export.hpp"
