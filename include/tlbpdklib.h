#pragma once

#ifdef TLB_USE_ZLIB

#include "..\src\zlib\zlib.h"
 
#pragma comment(lib, "zlib.lib")

#endif

#ifdef TLB_USE_TXDIB

#include "..\src\txdib\txdib.h"
#include "..\src\freeimage\FreeImage.h"

#pragma comment(lib, "txdib.lib")

#endif

#ifdef TLB_USE_XUILIB

#include "..\src\xuilib\src\xuilib.h"

#pragma comment(lib, "xuilib.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "comctl32.lib")

#endif

#ifdef TLB_USE_HTTPREADER

#include "..\src\httpReader\HttpReader.h"
#pragma comment(lib, "httpReader.lib")

#endif // TLB_USE_HTTPREADER

#ifdef TLB_USE_TLBSKIN

#include "..\src\tlbskinslib\tlbskinslib.h"
#pragma comment(lib, "tlbskinslib.lib")

#endif // TLB_USE_TXSKIN

#ifdef TLB_USE_CAIRO

#include "..\src\cairo\src\cairo.h"
#include "..\src\cairo\src\cairo-win32.h"

#pragma comment(lib, "cairo.lib")

#endif // TLB_USE_CAIRO

#ifndef TLB_NO_TLBPDK

#include "..\src\tlbpdklib\tlbpdklib.h"

#pragma comment(lib, "tlbpdk.lib")
#pragma comment(lib, "tlbpdklib.lib")
#pragma comment(lib, "version.lib")

#endif

#ifdef TLB_USE_SIMPLEDIB

#include "..\src\simpledib\dib.h"

#pragma comment(lib, "simpledib.lib")

#endif

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Msimg32.lib")

