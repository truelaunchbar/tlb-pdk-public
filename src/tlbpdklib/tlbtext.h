#pragma once

namespace tlbpdklib
{
	extern void tlbDrawText( HDC hdc, LPCTSTR text, int c, LPRECT rcText, UINT params, HFONT hFont = NULL, COLORREF color = 0xFFFFFFFF, INT vertical = 0, BOOL drawGlow = FALSE, COLORREF glowColor = 0, INT glowSize = 4, BOOL isTransparentDC = FALSE);
}
