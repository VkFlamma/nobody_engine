#ifndef ENV_DEFINES_H
#define ENV_DEFINES_H

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
#include <windowsx.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE 
#endif

#endif // ENV_DEFINES_H
