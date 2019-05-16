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
//#include <QuartzCore/QuartzCore.h>
#elif _WIN32
#include <windows.h>
#include <mmsystem.h>
// conflict with ErrorList::GetMessage
#undef GetMessage
#endif


#endif

