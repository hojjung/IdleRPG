// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#if defined(_M_IX86)
#	define VD_X86
#	define VD_WINDOWS
#elif defined(_M_X64)
#   define VD_X64
#   define VD_WINDOWS
#endif

#ifdef _XBOX_ONE
#undef VD_WINDOWS
#define VD_XBOX
#endif // _XBOX_ONE

#if defined(__ORBIS__)
#undef VD_WINDOWS
#define VD_PS4
#endif

#if defined(NN_SDK_BUILD_DEBUG) || defined(NN_SDK_BUILD_DEVELOP) || defined(NN_SDK_BUILD_RELEASE)
#undef VD_WINDOWS
#define VD_SWITCH
#include <nn/nn_common.h>
#endif // NN_SDK_BUILD_DEBUG) || defined(NN_SDK_BUILD_DEVELOP) || defined(NN_SDK_BUILD_RELEASE)


#if defined(VD_WINDOWS) || defined(VD_XBOX) || defined(VD_PS4) /*|| defined(VD_SWITCH)*/ // Building as DLL for Windows, Xbox One, PS4 and Switch
#   ifdef VICODYNAMICS_EXPORTS
#       define VICODYNAMICS_API __declspec(dllexport)
#   else
#       define VICODYNAMICS_API __declspec(dllimport)
#   endif
#else
#   define VICODYNAMICS_API
#endif

/**
Calling convention
*/
#ifndef VD_CALL_CONV
#	if defined VD_WINDOWS
#		define VD_CALL_CONV __cdecl
#	else
#		define VD_CALL_CONV
#	endif
#endif

#define RESERVE_PARTICLE_COUNT 1024
#define DEFAULT_BROADPHASE_MAX_PARTICLES_PER_CELL 200

// Uncomment for a memory leak report (requires VLD bins in PATH: https://vld.codeplex.com/)
#define TEST_MEM_LEAK 0