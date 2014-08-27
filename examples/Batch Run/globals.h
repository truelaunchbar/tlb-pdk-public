#pragma once

#define TLB_USE_TXDIB
#define TLB_USE_XUILIB

#include <Windows.h>
#include <tchar.h>
#include <tlbpdklib.h>
#include <shlwapi.h>
#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>

#define FREE_CLEAR_STR(str) if(str) delete str; str = NULL;
#define MAKE_STR(str, cpstr) FREE_CLEAR_STR(str); if(cpstr) { str = new WCHAR[lstrlen(cpstr) + 1]; lstrcpy(str, cpstr); }

extern void Version2Str(DWORD versionMS, DWORD versionLS, LPTSTR str);
extern void GetFileVersion(LPCTSTR fileName, DWORD& versionMS, DWORD& versionLS);

struct FILE_TO_RUN
{
	LPTSTR	fileName;
	LPTSTR	params;
	LPTSTR	workingDir;
};