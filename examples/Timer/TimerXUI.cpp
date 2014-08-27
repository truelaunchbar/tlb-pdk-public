#include "globals.h"
#include "TimerXUI.h"

CTimerXUI::CTimerXUI(void) : CXUIEngine(g_hInst)
{
}

CTimerXUI::~CTimerXUI(void)
{
}

void CTimerXUI::DrawImage( HDC hdc, int x, int y, int width, int height, LPWSTR imgUrl )
{
	CTxDIB img;
	img.load(FindResource(m_hInst, imgUrl, TEXT("XUILIB")), m_hInst);
	if(img.isValid())
	{
		img.draw(hdc, x, y, width, height);
	}
}
