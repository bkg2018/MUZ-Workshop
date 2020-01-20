/* Header convenient for compilers with precompiled headers. */
#ifndef __PCH_H
#define __PCH_H

#ifdef _WIN32
#pragma warning(push,3)
#endif

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
#include <sys/stat.h>
#define _mkdir(x) mkdir(x,0x777)
#elif _WIN32
#include <windows.h>
#include <mmsystem.h>
#include <direct.h>
#pragma warning(pop)
// conflict with ErrorList::GetMessage
#undef GetMessage
#endif

#endif

