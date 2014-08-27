#pragma once

#define TLB_USE_TXDIB
#define TLB_USE_TXSKIN
#define TLB_USE_XUILIB

#include <windows.h>
#define STRSAFE_NO_DEPRECATE
#include <tchar.h>
#include <strsafe.h>
#include <shlwapi.h>
#include <tlbpdklib.h>

#define FREE_CLEAR_STR(str) if(str) delete str; str = NULL;
#define MAKE_STR(str, cpstr) FREE_CLEAR_STR(str); if(cpstr) { str = new WCHAR[lstrlen(cpstr) + 1]; lstrcpy(str, cpstr); }

extern void Version2Str(DWORD versionMS, DWORD versionLS, LPTSTR str);
extern void GetFileVersion(LPCTSTR fileName, DWORD& versionMS, DWORD& versionLS);
extern void GetDesktopWorkRect(RECT* rcDsk, HWND hWnd);
extern void ActivateThisWindow(HWND wndActivate);

#define NWND_HPOS_LEFT		0x01
#define NWND_HPOS_RIGHT		0x02
#define NWND_HPOS_CENTER	0x03

#define NWND_HPOS_MASK		0x0F

#define NWND_VPOS_TOP		0x10
#define NWND_VPOS_BOTTOM	0x20
#define NWND_VPOS_CENTER	0x30

#define NWND_VPOS_MASK		0xF0


#define CTL_SIDE_HIDE		0x00
#define CTL_SIDE_LEFT		0x01
#define CTL_SIDE_TOP		0x02
#define CTL_SIDE_RIGHT		0x03
#define CTL_SIDE_BOTTOM		0x04

#define CTL_SIDE_MASK		0x0F

#define CTL_POS_NEAR		0x10
#define CTL_POS_CENTER		0x20
#define CTL_POS_FAR			0x30

#define CTL_POS_MASK		0xF0


#define INI_MENU_SKIN			L"menu"
#define INI_MENUBTN_SKIN		L"button"
#define INI_CONTROLS_SKIN		L"controls"
#define INI_NOTIFY_SKIN			L"notify"


extern GUID CLSID_TIMERBTN;
extern GUID CLSID_ADDBTN;

namespace tlbpdklib
{
	extern void tlbDrawText( HDC hdc, LPCTSTR text, int c, LPRECT rcText, UINT params, HFONT hFont = NULL, COLORREF color = 0xFFFFFFFF, INT vertical = 0, BOOL drawGlow = FALSE, COLORREF glowColor = 0, INT glowSize = 4, BOOL isTransparentDC = FALSE);
}
