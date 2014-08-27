#include "globals.h"
#include "TimerBtn.h"
#include "TimerMainBtn.h"
#include "TimerPropDlg.h"
#include "xuistrings.h"

// {422FF806-E819-451a-857E-C95D1EF93F95}
GUID CLSID_TIMERBTN = 
{ 0x422ff806, 0xe819, 0x451a, { 0x85, 0x7e, 0xc9, 0x5d, 0x1e, 0xf9, 0x3f, 0x95 } };


CTimerBtn::CTimerBtn(CTimerMainBtn* parent, DWORD timerID)
{
	m_parent	= parent;
	m_timerID	= timerID;

	m_skin.setCallback(this);
	loadSkin();

	CTimer* tm = m_parent->getTimer(m_timerID);
	if(tm)
	{
		tm->setSkinParams(&m_skin, 0);
		delete tm;
	}
	updateName();
}

CTimerBtn::~CTimerBtn(void)
{
}

void CTimerBtn::GetChildUID( LPWSTR uid )
{
	wsprintf(uid, TEXT("timer#%d"), m_timerID);
}

void CTimerBtn::GetSize( SIZE* sz, BOOL actual )
{
	sz->cx = m_skin.width();
	sz->cy = m_skin.height();
}

void CTimerBtn::OnDraw( HDC hDC, LPRECT rcItem )
{
	m_skin.draw(hDC, rcItem->left, rcItem->top);
}

void CTimerBtn::OnElementClick( LPCWSTR id, int sliceID )
{
	if(!StrCmpI(id, L"startstop"))
	{
		m_parent->startTimer(m_timerID);
		if(m_parent->closeOnStart())
		{
			m_parent->closeMenu();
		}
	} else if(!StrCmpI(id, L"edit"))
	{
		OnProperties();
	} else if(!StrCmpI(id, L"delete"))
	{
		OnDelete();
		m_parent->openMenu();
	}
}

void CTimerBtn::OnSliderValue( LPCWSTR id, int val, int sliceID )
{

}

BOOL CTimerBtn::OnMouseOver( long keys, int x, int y )
{
	if(m_skin.mouseOver(x - m_rc.left, y - m_rc.top))
	{
		m_container->Redraw();
	}
	return TRUE;
}

BOOL CTimerBtn::OnMouseLeave( long keys )
{
	if(m_skin.mouseLeave())
	{
		m_container->Redraw();
	}
	return TRUE;
}

BOOL CTimerBtn::OnMouseEnter( long keys, int x, int y )
{
	if(m_skin.mouseOver(x - m_rc.left, y - m_rc.top))
	{
		m_container->Redraw();
	}
	return TRUE;
}

BOOL CTimerBtn::OnLButtonUp( long key, int x, int y )
{
	if(m_skin.lButtonUp(x - m_rc.left, y - m_rc.top))
	{
		m_container->Redraw();
	}
	return TRUE;
}

BOOL CTimerBtn::OnLButtonDown( long key, int x, int y )
{
	if(m_skin.lButtonDown(x - m_rc.left, y - m_rc.top))
	{
		m_container->Redraw();
	}
	return TRUE;
}

UINT CTimerBtn::GetModeFlags( void )
{
	return BTN_FLAG_PLANE;
}

UINT CTimerBtn::ReadChanges()
{
	CTimer* tm = m_parent->getTimer(m_timerID);
	if(!tm)
	{
		return 2;
	}

	UINT ret = 0;
	if(tm->setSkinParams(&m_skin, 0))
	{
		ret |= READ_CHANGES_REDRAW;
	}
	if(StrCmpI(m_parent->getMenuSkin().c_str(), m_oldSkin.c_str()))
	{
		ret |= READ_CHANGES_RECALC;
		loadSkin();
	}

	delete tm;
	return ret;
}

UINT CTimerBtn::GetSupportedActions()
{
	return CTMS_PROPERTIES | CTMS_HOTKEY;
}

BOOL CTimerBtn::OnProperties()
{
	CTimer* tm = m_parent->getTimer(m_timerID);
	if(tm)
	{
		CTimerPropDlg dlg(tm, m_parent->getXUI());
		if(dlg.DoModal(m_container->GetToolbarWindow()) == IDOK)
		{
			m_parent->editTimer(m_timerID, tm);
			m_parent->saveButton();
			m_container->Redraw();
			if(dlg.m_saveAndRun)
			{
				m_parent->startTimer(m_timerID);
			}
		}
		delete tm;
	}
	return TRUE;
}

void CTimerBtn::OnHotkey( UINT hkID )
{
	m_parent->startTimer(m_timerID);
}

void CTimerBtn::loadSkin()
{
	m_oldSkin = m_parent->getMenuSkin();
	m_skin.loadSkin(m_parent->getMenuSkin());
}

BOOL CTimerBtn::OnStateChanged( int state, int* needRedraw )
{
	if(state)
	{
		m_container->SetState(0);
	}
	*needRedraw = FALSE;
	return TRUE;
}

void CTimerBtn::updateName()
{
	CTimer* tm = m_parent->getTimer(m_timerID);
	if(tm)
	{
		WCHAR time[100];
		tm->m_startTime = 0;
		tm->formatTime(time);
		StringCchPrintf(m_Name, 255, L"%s: %s (%s)", m_parent->getXUI()->getStringDef(XUI_STR_TIMERHOTKEY), tm->m_name ? tm->m_name : L"", time);
		delete tm;
	}
}

void CTimerBtn::GetButtonText( LPWSTR text )
{
	text[0] = 0;
}

BOOL CTimerBtn::OnDelete()
{
	if(MessageBox(NULL, m_parent->getXUI()->getStringDef(XUI_ASK_DELTIMER), m_parent->getXUI()->getStringDef(XUI_STR_APPNAME), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		m_parent->deleteTimer(m_timerID);
		return TRUE;
	}
	return FALSE;
}

BOOL CTimerBtn::QueryClass( CLSID classID, LPVOID* lpClass )
{
	if(classID == CLSID_TIMERBTN)
	{
		*lpClass = this;
		return TRUE;
	}
	return FALSE;
}

HICON CTimerBtn::GetTipIcon( int tipID )
{
	return (HICON) 0xFFFFFFFF;
}

int CTimerBtn::GetTipsIconSize( int tipID )
{
	return 48;
}

void CTimerBtn::DrawTipsIcon( HDC hDC, POINT pt, int tipID )
{
	m_parent->getXUI()->DrawImage(hDC, pt.x, pt.y, 48, 48, L"logo.png");
}

LPWSTR CTimerBtn::GetTipCaption( int tipID )
{
	LPWSTR text = NULL;
	CTimer* tm = m_parent->getTimer(m_timerID);
	if(tm)
	{
		std::wstring tip;
		if(!tm->m_name)
		{
			tip = m_parent->getXUI()->getStringDef(XUI_STR_APPNAME);
		} else
		{
			tip = tm->m_name;
		}

		text = (LPWSTR) CoTaskMemAlloc((tip.length() + 1) * sizeof(WCHAR));
		lstrcpy(text, tip.c_str());
		delete tm;
	}

	return text;
}

LPWSTR CTimerBtn::GetTipText( int tipID )
{
	LPWSTR text = NULL;
	CTimer* tm = m_parent->getTimer(m_timerID);
	if(tm)
	{
		WCHAR str[255];
		std::wstring tip;
		
		tm->formatTime(str);
		tip += m_parent->getXUI()->getStringDef(XUI_STR_TIP_TIME);
		tip += L":<t><b>";
		tip += str;
		tip += L"</b>";

		if(tm->m_sound)
		{
			tip += L"<br>";
			tip += m_parent->getXUI()->getStringDef(XUI_STR_TIP_SOUND);
			tip += L":<t><b>";
			tip += tm->m_sound;
			tip += L"</b>";
		}

		if(tm->m_command)
		{
			tip += L"<br>";
			tip += m_parent->getXUI()->getStringDef(XUI_STR_TIP_CMD);
			tip += L":<t><b>";
			tip += tm->m_command;
			tip += L"</b>";
		}

		if(tm->m_message)
		{
			tip += L"<br><hr><br>";
			tip += tm->m_message;
		}

		text = (LPWSTR) CoTaskMemAlloc((tip.length() + 1) * sizeof(WCHAR));
		lstrcpy(text, tip.c_str());
		delete tm;
	}

	return text;
}
