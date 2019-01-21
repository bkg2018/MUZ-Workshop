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

/** MUZ headers */
#include "MUZ-Assembler/Assembler.h"
#include "MUZ-Common/FileUtils.h"

/* OS Timing APIs */
#ifdef __APPLE__
#include <QuartzCore/QuartzCore.h>
#elif _WIN32
#include <windows.h>
#include <mmsystem.h>
#endif

#endif