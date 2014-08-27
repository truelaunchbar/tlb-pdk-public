#pragma once

#include "TxWnd.h"
#include "NotifySkin.h"

class CTimerMainBtn;
class CTimer;

#define NOTIFY_WINDOW	L"TLB_TIMER_NOTIFY"

struct DRAWDC
{
	HDC					hdc;
	HBITMAP				bmp;
	int					width;
	int					height;
	LPVOID				pixels;

	void clear()
	{
		if(hdc)
		{
			DeleteDC(hdc);
			hdc = NULL;
		}
		if(bmp)
		{
			DeleteObject(bmp);
			bmp = NULL;
		}
		width	= 0;
		height	= 0;
		pixels	= 0;
	}
};


class CNotifyWnd :	public CTxWnd,
					public ISkinCallBack
{
	CTimerMainBtn*	m_parent;
	DWORD			m_timerID;
	CNotifySkin		m_skin;
	DRAWDC			m_drawDC;
	BOOL			m_isTest;
	LPWSTR			m_title;
public:
	CNotifyWnd(CTimerMainBtn* parent, DWORD timerID, BOOL isTest = FALSE);
	virtual ~CNotifyWnd(void);

	BOOL	create(int x, int y);
	int		width()					{ return m_skin.width();	}
	int		height()				{ return m_skin.height();	}
	DWORD	getID()					{ return m_timerID;			}
	void	update();

	// public ISkinCallBack
	virtual void	OnElementClick(LPCWSTR id, int sliceID);
	virtual void	OnSliderValue(LPCWSTR id, int val, int sliceID);
	

private:
	virtual LRESULT OnMessage(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	virtual void	preRegisterClass(WNDCLASSEX* wcex);
	
	void	loadSkin();
	void	createDrawDC();
	void	updateWindow();
};
