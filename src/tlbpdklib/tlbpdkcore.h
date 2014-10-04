#pragma once

#include <windows.h>
#include <comcat.h>
#include <ole2.h>
#include "tlbif.h"
#include <olectl.h>
#include <tlbpdk.h>
#include <vector>

#define TLB_PDK_CORE

//Register
extern BOOL RegisterPlugin(HINSTANCE g_hInst, CLSID clsid, LPSTR lpszTitle);
extern BOOL UnregisterPlugin(CLSID clsid);
extern CATID CATID_TLBBUTTONS;
extern UINT g_DllRefCount;

class CTlbButton;

//You must implement these functions
extern LPCLSID		GetCLSID();
extern LPSTR		GetPluginName();
extern void			InitDLL();
extern void			UninitDLL();
extern CTlbButton*	CreatePlugin();
extern HICON ExtractIconFromFile(LPCTSTR flName, DWORD index, DWORD IconSize);

#define FREE_CLEAR_STR(str) if(str) delete str; str = NULL;
#define MAKE_STR(str, cpstr) FREE_CLEAR_STR(str); if(cpstr) { str = new WCHAR[lstrlen(cpstr) + 1]; lstrcpy(str, cpstr); }
