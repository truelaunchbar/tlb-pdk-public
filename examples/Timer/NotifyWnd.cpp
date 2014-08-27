#include "globals.h"
#include "NotifyWnd.h"
#include "TimerMainBtn.h"
#include "timerclass.h"
#include <WindowsX.h>
#include "xuistrings.h"

CNotifyWnd::CNotifyWnd(CTimerMainBtn* parent, DWORD timerID, BOOL isTest)
{
	setClass(NOTIFY_WINDOW);
	ZeroMemory(&m_drawDC, sizeof(m_drawDC));
	m_parent	= parent;
	m_timerID	= timerID;
	m_isTest	= isTest;
	m_title		= NULL;
	m_skin.setCallback(this);

	update();
}

CNotifyWnd::~CNotifyWnd(void)
{
	FREE_CLEAR_STR(m_title);
	m_drawDC.clear();
}

LRESULT CNotifyWnd::OnMessage( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	switch(uMessage)
	{
	case WM_NCHITTEST:
		{
			POINT pt;
			pt.x	= GET_X_LPARAM(lParam);
			pt.y	= GET_Y_LPARAM(lParam);
			MapWindowPoints(NULL, hWnd, &pt, 1);
			if(!m_skin.getOverID( pt.x, pt.y ))
			{
				return HTCAPTION;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			TRACKMOUSEEVENT tev;
			ZeroMemory(&tev, sizeof(tev));
			tev.cbSize		= sizeof(TRACKMOUSEEVENT);
			tev.dwFlags		= TME_LEAVE;
			tev.hwndTrack	= hWnd;
			TrackMouseEvent(&tev);

			if( m_skin.mouseOver( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) ) )
			{
				updateWindow();
			}
		}
		break;
	case WM_LBUTTONDOWN:
		if( m_skin.lButtonDown( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) ) )
		{
			updateWindow();
		}
		break;
	case WM_LBUTTONUP:
		if( m_skin.lButtonUp( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) ) )
		{
			updateWindow();
		}
		break;
	case WM_MOUSELEAVE:
		{
			TRACKMOUSEEVENT tev;
			ZeroMemory(&tev, sizeof(tev));
			tev.cbSize		= sizeof(TRACKMOUSEEVENT);
			tev.dwFlags		= TME_LEAVE | TME_CANCEL;
			tev.hwndTrack	= hWnd;
			TrackMouseEvent(&tev);

			if( m_skin.mouseLeave() )
			{
				updateWindow();
			}
		}
		break;
	}
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

BOOL CNotifyWnd::create( int x, int y )
{
	BOOL ret = CTxWnd::create(WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST, m_title, WS_POPUP, x, y, m_skin.width(), m_skin.height(), NULL, NULL, g_hInst);
	createDrawDC();
	updateWindow();
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	return ret;
}

void CNotifyWnd::loadSkin()
{
	m_skin.loadSkin(m_parent->getNotifySkin());
}

void CNotifyWnd::OnElementClick( LPCWSTR id, int sliceID )
{
	if(!StrCmpI(id, L"close"))
	{
		if(m_parent && !m_isTest)
		{
			m_parent->stopPlayer(m_timerID);
		}
		DestroyWindow(m_hWnd);
	} else if(!StrCmpI(id, L"restart"))
	{
		DestroyWindow(m_hWnd);
		if(m_parent && !m_isTest)
		{
			m_parent->stopPlayer(m_timerID);
			m_parent->startTimer(m_timerID);
		}
	}
}

void CNotifyWnd::OnSliderValue( LPCWSTR id, int val, int sliceID )
{

}

void CNotifyWnd::createDrawDC()
{
	m_drawDC.clear();

	BITMAPINFO bmp_info; 
	bmp_info.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER); 
	bmp_info.bmiHeader.biWidth			= m_skin.width(); 
	bmp_info.bmiHeader.biHeight			= -m_skin.height(); 
	bmp_info.bmiHeader.biPlanes			= 1; 
	bmp_info.bmiHeader.biBitCount		= 32; 
	bmp_info.bmiHeader.biCompression	= BI_RGB; 
	bmp_info.bmiHeader.biSizeImage		= 0; 
	bmp_info.bmiHeader.biXPelsPerMeter	= 0; 
	bmp_info.bmiHeader.biYPelsPerMeter	= 0; 
	bmp_info.bmiHeader.biClrUsed		= 0; 
	bmp_info.bmiHeader.biClrImportant	= 0; 

	m_drawDC.hdc = CreateCompatibleDC(NULL); 

	m_drawDC.height	= m_skin.height();
	m_drawDC.width	= m_skin.width();

	m_drawDC.pixels = NULL; 
	m_drawDC.bmp = ::CreateDIBSection( 
		m_drawDC.hdc, 
		&bmp_info, 
		DIB_RGB_COLORS, 
		&m_drawDC.pixels, 
		0, 
		0 
		); 
	ZeroMemory(m_drawDC.pixels, m_drawDC.width * m_drawDC.height * 4);
	SelectObject(m_drawDC.hdc, m_drawDC.bmp);
}

void CNotifyWnd::updateWindow()
{
	if(!m_drawDC.hdc || !m_drawDC.pixels || !m_hWnd)
	{
		return;
	}
	ZeroMemory(m_drawDC.pixels, m_drawDC.width * m_drawDC.height * 4);

	RECT rcWnd;
	GetWindowRect(m_hWnd, &rcWnd);

	m_skin.draw(m_drawDC.hdc, 0, 0);

	POINT ptDst;
	POINT ptSrc	= {0, 0};
	ptDst.x	= rcWnd.left;
	ptDst.y	= rcWnd.top;
	SIZE wndSize = { m_drawDC.width, m_drawDC.height };

	BLENDFUNCTION bf;
	bf.BlendOp				= AC_SRC_OVER;
	bf.BlendFlags			= 0;
	bf.AlphaFormat			= AC_SRC_ALPHA;
	bf.SourceConstantAlpha	= 255;

	UpdateLayeredWindow(m_hWnd, NULL, &ptDst, &wndSize, m_drawDC.hdc, &ptSrc, 0, &bf, ULW_ALPHA);
}

void CNotifyWnd::preRegisterClass( WNDCLASSEX* wcex )
{
	wcex->hCursor = LoadCursor(NULL, IDC_ARROW);
}

void CNotifyWnd::update()
{
	loadSkin();

	CTimer* timer = NULL;

	if(!m_isTest && m_parent)
	{
		timer	= m_parent->getTimer(m_timerID);
	} else
	{
		timer = new CTimer();
		timer->m_id	= 0;
		MAKE_STR(timer->m_command,	L"notepad.exe");
		MAKE_STR(timer->m_message,	m_parent->getXUI()->getStringDef(XUI_STR_TESTMESSAGE));
		MAKE_STR(timer->m_name,		m_parent->getXUI()->getStringDef(XUI_STR_TESTNAME));
	}

	MAKE_STR(m_title, timer->m_name);

	timer->setSkinParams(&m_skin, 0);

	delete timer;
}
