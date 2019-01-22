/* Header convenient for compilers with precompiled headers. */
#ifndef __PCH_H
#define __PCH_H

/** Standard headers used a lot */
#include <string.h>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <iostream>

/* OS Timing APIs */
#ifdef __APPLE__
#include <QuartzCore/QuartzCore.h>
#elif _WIN32
#include <windows.h>
#include <mmsystem.h>
// conflict with ErrorList::GetMessage
#undef GetMessage
#endif

/** no exceptions:
 * _NOEXCEPT is handled by XCode
 *  it must be neutralized for Visual Studio
 *  it must be replaced by the deprecated throw() for GCC/Linux even in C++ 11
 */
#ifdef _WIN32
  #define _NOEXCEPT
#else
  #ifndef __APPLE__
  #define _NOEXCEPT noexcept
  #endif
#endif


#endif

