#pragma once



#if defined(Q_OS_WINDOWS)
#pragma warning(push)
#pragma warning(disable : 4100 4189 4267)
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-requires"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "src/simdjson/simdjson.h"

#if defined(Q_OS_WINDOWS)
#pragma warning(pop)
#elif defined(__clang__)
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
