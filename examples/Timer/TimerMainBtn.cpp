#include "globals.h"
#include "TimerMainBtn.h"
#include "PropertiesDlg.h"
#include "TimerBtn.h"
#include "NotifyWnd.h"
#include "AddTimerBtn.h"
#include "TimerPropDlg.h"
#include "xuistrings.h"
#include "resource.h"

CTimerMainBtn::CTimerMainBtn(void)
{
	InitializeCriticalSection(&m_sync);
	m_rowColsMode			= TxSkin::sliceByRow;
	m_RowsCols				= 1;
	m_activeTimers			= 0;
	m_nextID				= 1;
	m_changesHandle			= NULL;
	m_playSound				= TRUE;
	m_pos					= NWND_HPOS_CENTER | NWND_VPOS_CENTER;
	m_showWindow			= TRUE;
	m_showTimers			= TRUE;
	m_closeMenuOnStart		= TRUE;
	m_ctlPos				= CTL_SIDE_LEFT | CTL_POS_CENTER;
	m_skin.setCallback(this);
	m_skinCtl.setCallback(this);
}

CTimerMainBtn::~CTimerMainBtn(void)
{
	postMessage(WM_QUIT, 0, 0);
	CTxThread::Stop();

	lock();
	for(size_t i = 0; i < m_timers.size(); i++)
	{
		delete m_timers[i];
	}
	m_timers.clear();
	unlock();

	DeleteCriticalSection(&m_sync);
}

BOOL CTimerMainBtn::isMenu()
{
	return TRUE;
}

BOOL CTimerMainBtn::Save( IStream *data )
{
	CPluginStream stream;
	stream.BeginNode(L"settings");
		stream.SaveDWORD	(L"nextID",			m_nextID);
		stream.SaveDWORD	(L"playSound",		m_playSound);
		stream.SaveDWORD	(L"pos",			m_pos);
		stream.SaveDWORD	(L"showWindow",		m_showWindow);
		stream.SaveDWORD	(L"rowColsMode",	m_rowColsMode);
		stream.SaveDWORD	(L"RowsCols",		m_RowsCols);
		stream.SaveDWORD	(L"ctlPos",			m_ctlPos);
		stream.SaveDWORD	(L"showTimers",		m_showTimers);
		stream.SaveDWORD	(L"closeMenuOnStart",		m_closeMenuOnStart);
		stream.SaveString	(L"skinMenu",		m_skinFileMenu.c_str());
		stream.SaveString	(L"skinBtn",		m_skinFileBtn.c_str());
		stream.SaveString	(L"skinCtrls",		m_skinFileCtrls.c_str());
		stream.SaveString	(L"skinNotify",		m_skinFileNotify.c_str());
		stream.SaveString	(L"sound",			m_soundFile.c_str());
		stream.SaveString	(L"ilAddBtn",		m_iconLocationAddBtn.c_str());
		stream.SaveString	(L"icon",			m_iconLocation);
	stream.EndNode();

	stream.BeginNode(L"timers");
		stream.SaveDWORD(L"count", (DWORD) m_timers.size());
		lock();
		for(size_t i = 0; i < m_timers.size(); i++)
		{
			WCHAR node[100];
			wsprintf(node, L"timer#%d", (DWORD) i);
			stream.BeginNode(node);
				m_timers[i]->save(&stream);
			stream.EndNode();
		}
		unlock();
	stream.EndNode();

	stream.Save(data);
	
	return TRUE;
}

BOOL CTimerMainBtn::Load( IStream *data )
{
	CPluginStream stream;
	if(stream.Load(data))
	{
		if(stream.OpenNode(L"settings"))
		{
			m_nextID				= stream.GetDWORD(L"nextID",		1);
			m_playSound				= stream.GetDWORD(L"playSound",		1);
			m_showWindow			= stream.GetDWORD(L"showWindow",	1);
			m_rowColsMode			= stream.GetDWORD(L"rowColsMode",	TxSkin::sliceByRow);
			m_RowsCols				= stream.GetDWORD(L"RowsCols",		1);
			m_showTimers			= stream.GetDWORD(L"showTimers",	1);
			m_pos					= stream.GetDWORD(L"pos",			NWND_HPOS_CENTER | NWND_VPOS_CENTER);
			m_ctlPos				= stream.GetDWORD(L"ctlPos",		CTL_SIDE_LEFT | CTL_POS_CENTER);
			m_closeMenuOnStart		= stream.GetDWORD(L"closeMenuOnStart",		TRUE);
			m_skinFileMenu			= stream.GetString(L"skinMenu",		L"");
			m_skinFileBtn			= stream.GetString(L"skinBtn",		L"");
			m_skinFileCtrls			= stream.GetString(L"skinCtrls",	L"");
			m_skinFileNotify		= stream.GetString(L"skinnotify",	L"");
			m_soundFile				= stream.GetString(L"sound",		L"");
			m_iconLocationAddBtn	= stream.GetString(L"ilAddBtn",		L"");
			MAKE_STR(m_iconLocation ,stream.GetString(L"icon",			L""));
			stream.CloseNode();
		}
		if(stream.OpenNode(L"timers"))
		{
			CTimer* tm;
			DWORD cnt = stream.GetDWORD(L"count");
			for(DWORD i = 0; i < cnt; i++)
			{
				WCHAR node[100];
				wsprintf(node, L"timer#%d", (DWORD) i);
				if(stream.OpenNode(node))
				{
					tm = new CTimer;
					tm->load(&stream);
					m_timers.push_back(tm);
					stream.CloseNode();
				}
			}
			stream.CloseNode();
		}
	}
	m_skin.setSliceMode((TxSkin::sliceMode) m_rowColsMode, m_RowsCols);
	loadSkin();
	CTxThread::Run();
	return TRUE;
}

BOOL CTimerMainBtn::SupportGlassMenu()
{
	return TRUE;
}

BOOL CTimerMainBtn::openProperties()
{
	CPropertiesDlg dlg(&m_xui);
	dlg.m_soundFile			= m_soundFile;
	dlg.m_skinFileMenu		= m_skinFileMenu;
	dlg.m_skinFileBtn		= m_skinFileBtn;
	dlg.m_skinFileCtrls		= m_skinFileCtrls;
	dlg.m_skinFileNotify	= m_skinFileNotify;
	dlg.m_playSound			= m_playSound;
	dlg.m_pos				= m_pos;
	dlg.m_showWindow		= m_showWindow;
	dlg.m_RowsCols			= m_RowsCols;
	dlg.m_rowColsMode		= m_rowColsMode;
	dlg.m_ctlPos			= m_ctlPos;
	dlg.m_showTimers		= m_showTimers;
	dlg.m_closeMenuOnStart	= m_closeMenuOnStart;

	if(dlg.DoModal(m_container->GetToolbarWindow()) == IDOK)
	{
		m_closeMenuOnStart	= dlg.m_closeMenuOnStart;
		m_soundFile			= dlg.m_soundFile;
		m_skinFileMenu		= dlg.m_skinFileMenu;
		m_skinFileBtn		= dlg.m_skinFileBtn;
		m_skinFileNotify	= dlg.m_skinFileNotify;
		m_skinFileCtrls		= dlg.m_skinFileCtrls;

		m_playSound			= dlg.m_playSound;
		m_pos				= dlg.m_pos;
		m_showWindow		= dlg.m_showWindow;
		m_RowsCols			= dlg.m_RowsCols;
		m_rowColsMode		= dlg.m_rowColsMode;
		m_ctlPos			= dlg.m_ctlPos;
		m_showTimers		= dlg.m_showTimers;
		return TRUE;
	}
	return FALSE;
}

BOOL CTimerMainBtn::OnCreate()
{
	if(openProperties())
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CTimerMainBtn::OnProperties()
{
	if(openProperties())
	{
		loadSkin();
		m_skin.setSliceMode((TxSkin::sliceMode) m_rowColsMode, m_RowsCols);

		if(m_changesHandle)
		{
			SetEvent(m_changesHandle);
		}

		m_container->UpdateSize();
		m_container->SaveButton();
		return TRUE;
	}
	return FALSE;
}

UINT CTimerMainBtn::GetSupportedActions()
{
	return CTlbButton::GetSupportedActions() | CTMS_CHANGEICON | CTMS_RESETICON | CTMS_PROPERTIES | CTMS_HOTKEY;
}

UINT CTimerMainBtn::GetModeFlags()
{
	UINT add = 0;
	if(m_activeTimers && m_showTimers)
	{
		add = BTN_FLAG_PLANE;
	}
	return CTlbButton::GetModeFlags() | BTN_FLAG_SUPPORTPNGICONS | add;
}

UINT CTimerMainBtn::GetMenuFlags()
{
	if(m_activeTimers && m_showTimers)
	{
		return MENU_F_NOAUTOSHOW;
	}
	return 0;
}

HANDLE CTimerMainBtn::GetChangesHandle()
{
	if(!m_changesHandle)
	{
		m_changesHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	return m_changesHandle; 
}

void CTimerMainBtn::CloseChangesHandle( HANDLE hChanges )
{
	if(m_changesHandle)
	{
		CloseHandle(hChanges);
		m_changesHandle = NULL;
	}
}

BOOL CTimerMainBtn::FirstChildData( CHILDS_DATA* data )
{
	if(data->Name)
	{
		lstrcpy(data->Name, TEXT("addtimer"));
	}
	data->childData = (LPVOID) 1; 
	data->enumData = (LPVOID) 1; 
	return TRUE;
}

BOOL CTimerMainBtn::NextChildData( CHILDS_DATA* data )
{
	int idx = (int) data->enumData;
	if(idx >= (int) m_timers.size() + 1) 
	{
		return FALSE;
	}
	if(data->Name)
	{
		wsprintf(data->Name, TEXT("timer#%d"), m_timers[idx - 1]->m_id);
	}
	idx++;
	data->childData = (LPVOID) idx; 
	data->enumData = (LPVOID) idx; 
	return TRUE;
}

BOOL CTimerMainBtn::FreeChildData( CHILDS_DATA* data )
{
	return TRUE;
}

BOOL CTimerMainBtn::CreateChild( CREATE_CHILD_DATA* childData )
{
	int idx = (int) childData->childData;
	if(idx == 1)
	{
		childData->child = new CAddTimerBtn(this);
	} else if((idx - 2) < (int) m_timers.size())
	{
		childData->child = new CTimerBtn(this, m_timers[idx - 2]->m_id);
	}
	return TRUE;
}

CTimer* CTimerMainBtn::getTimer( DWORD id )
{
	CTimer* ret = NULL;
	lock();
	for(size_t i = 0; i < m_timers.size(); i++)
	{
		if(m_timers[i]->m_id == id)
		{
			ret = new CTimer(*m_timers[i]);
			break;
		}
	}
	unlock();
	return ret;
}

void CTimerMainBtn::startTimer( DWORD id )
{
	PostMessage(m_hWndTimer, WM_ONSTARTTIMER, (WPARAM) id, NULL);
}

void CTimerMainBtn::start()
{
	SetTimer(m_hWndTimer, 1, 100, NULL);
}

void CTimerMainBtn::stop()
{
	KillTimer(m_hWndTimer, 1);
}

void CTimerMainBtn::OnTimer()
{
	BOOL update = FALSE;
	BOOL found = FALSE;
	lock();
	for(size_t i = 0; i < m_timers.size(); i++)
	{
		if(m_timers[i]->m_startTime)
		{
			found = TRUE;

			m_timers[i]->m_curTime = GetTickCount();
			if(m_timers[i]->setSkinParams(&m_skin, m_timers[i]->m_id))
			{
				update = TRUE;
			}

			if(m_timers[i]->checkTimer())
			{
				m_timers[i]->m_startTime = 0;
				m_timers[i]->m_curTime = 0;
				raiseTimer(m_timers[i]->m_id);
				update = TRUE;

			}
		}
	}
	unlock();
	if(update)
	{
		if(m_changesHandle)
		{
			SetEvent(m_changesHandle);
		}
		m_container->Redraw();
	}
	if(!found)
	{
		stop();
	}
}

void CTimerMainBtn::raiseTimer( DWORD id )
{
	PostMessage(m_hWndTimer, WM_ONRUNTIMER, (WPARAM) id, NULL);
}

DWORD CTimerMainBtn::ThreadProc()
{
	CoInitialize(NULL);
	m_hWndTimer.setHandler(this, &CTimerMainBtn::WndProcTimer);
	m_hWndTimer.create(0, 0, 0, 0, 0, 0, 0, NULL, NULL, g_hInst);

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	m_hWndTimer.destroy();
	
	for(size_t i = 0; i < m_players.size(); i++)
	{
		delete m_players[i];
	}
	m_players.clear();

	for(size_t i = 0; i < m_notifyWnds.size(); i++)
	{
		delete m_notifyWnds[i];
	}
	m_notifyWnds.clear();

	return 0;
}

LRESULT CTimerMainBtn::WndProcTimer( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	switch (uMessage)
	{
	case WM_TIMER:
		OnTimer();
		break;
	case WM_ONRUNTIMER:
		onRunTimer((DWORD) wParam);
		break;
	case WM_ONADDTIMER:
		onAddTimer();
		break;
	case WM_ONSTARTTIMER:
		onStartTimer((DWORD) wParam);
		break;
	case WM_ONSDELELETIMER:
		onDeleteTimer((DWORD) wParam);
		break;
	case WM_ONSTOPALL:
		onStopAllTimers();
		break;
	}
	return DefWindowProc(hWnd, uMessage, wParam, lParam);
}

void CTimerMainBtn::onRunTimer( DWORD id )
{
	onTimerStopped(id);
	CTimer* tm = getTimer(id);
	if(tm)
	{
		if(m_playSound)
		{
			WCHAR defSound[MAX_PATH];

			if(m_soundFile.empty())
			{
				GetModuleFileName(g_hInst, defSound, MAX_PATH);
				PathRemoveFileSpec(defSound);
				PathAddBackslash(defSound);
				lstrcat(defSound, TEXT("sound.mp3"));
			} else
			{
				StringCchCopy(defSound, MAX_PATH, m_soundFile.c_str());
			}

			BOOL found = FALSE;
			for(size_t i = 0; i < m_players.size(); i++)
			{
				if(m_players[i]->getID() == id)
				{
					found = TRUE;
					if(tm->m_playSound)
					{
						if(tm->m_sound)
						{
							if(!m_players[i]->play(tm->m_sound))
							{
								m_players[i]->play(defSound);
							}
						} else
						{
							m_players[i]->play(defSound);
						}
					}
					break;
				}
			}
			if(!found)
			{
				if(tm->m_playSound)
				{
					CMCIPlayer* player = new CMCIPlayer(id);
					m_players.push_back(player);
					if(tm->m_sound)
					{
						if(!player->play(tm->m_sound))
						{
							player->play(defSound);
						}
					} else
					{
						player->play(defSound);
					}
				}
			}
		}

		if(m_showWindow)
		{
			RECT rcDesk;
			GetDesktopWorkRect(&rcDesk, m_container->GetToolbarWindow());
			int x = 0;
			int y = 0;

			CNotifyWnd* wnd = NULL;
			BOOL isNewWnd	= FALSE;
			BOOL needCreate = FALSE;

			for(size_t i = 0; i < m_notifyWnds.size(); i++)
			{
				if(m_notifyWnds[i]->getID() == id)
				{
					wnd = m_notifyWnds[i];
					if(!IsWindow(*wnd))
					{
						needCreate = TRUE;
					}
					break;
				}
			}

			if(!wnd)
			{
				needCreate	= TRUE;
				isNewWnd	= TRUE;
				wnd = new CNotifyWnd(this, id);
			} else
			{
				wnd->update();
			}

			if(needCreate)
			{
				switch(m_pos & NWND_HPOS_MASK)
				{
				case NWND_HPOS_CENTER:
					x = rcDesk.left + (rcDesk.right - rcDesk.left) / 2 - wnd->width() / 2;
					break;
				case NWND_HPOS_LEFT:
					x = rcDesk.left;
					break;
				default: // NWND_HPOS_RIGHT
					x = rcDesk.right - wnd->width();
					break;
				}

				switch(m_pos & NWND_VPOS_MASK)
				{
				case NWND_VPOS_CENTER:
					y = rcDesk.top + (rcDesk.bottom - rcDesk.top) / 2 - wnd->height() / 2;
					break;
				case NWND_VPOS_TOP:
					y = rcDesk.top;
					break;
				default: // NWND_VPOS_BOTTOM
					y = rcDesk.bottom - wnd->height();
					break;
				}
				wnd->create(x, y);
			} else
			{
				wnd->update();
			}
			if(isNewWnd)
			{
				m_notifyWnds.push_back(wnd);
			}
			ActivateThisWindow(*wnd);
		}

		if(tm->m_command)
		{
			TCHAR reader[MAX_PATH];
			TCHAR params[MAX_PATH];
			lstrcpy(reader, tm->m_command);
			PathRemoveArgs(reader);
			if(lstrlen(reader) != lstrlen(tm->m_command))
			{
				lstrcpy(params, tm->m_command + lstrlen(reader));
				PathRemoveBlanks(params);
				ShellExecute(NULL, NULL, reader, params, NULL, SW_NORMAL);
			} else
			{
				ShellExecute(NULL, NULL, reader, NULL, NULL, SW_NORMAL);
			}
		}

		delete tm;
	}
}

void CTimerMainBtn::saveButton()
{
	m_container->SaveButton();
}

void CTimerMainBtn::editTimer( DWORD timerID, CTimer* timer )
{
	lock();
	for(size_t i = 0; i < m_timers.size(); i++)
	{
		if(timerID == m_timers[i]->m_id)
		{
			*(m_timers[i]) = *timer;
			break;
		}
	}
	unlock();
	if(m_changesHandle)
	{
		SetEvent(m_changesHandle);
	}
}

void CTimerMainBtn::OnHotkey( UINT hkID )
{
	m_container->ShowMenu();
}

void CTimerMainBtn::stopPlayer( DWORD id )
{
	for(size_t i = 0; i < m_players.size(); i++)
	{
		if(m_players[i]->getID() == id)
		{
			m_players[i]->stop();
		}
	}
}

void CTimerMainBtn::addNewTimer()
{
	PostMessage(m_hWndTimer, WM_ONADDTIMER, NULL, NULL);
}

void CTimerMainBtn::onAddTimer()
{
	CTimer* timer = new CTimer;
	MAKE_STR(timer->m_name,		m_xui.getStringDef(XUI_STR_DEFTIMERNAME));
	MAKE_STR(timer->m_message,	m_xui.getStringDef(XUI_STR_DEFTIMERMESSAGE));
	timer->m_playSound = TRUE;
	CTimerPropDlg dlg(timer, &m_xui);
	if(dlg.DoModal(m_container->GetToolbarWindow()) == IDOK)
	{
		timer->m_id = m_nextID++;
		m_timers.push_back(timer);
		if(dlg.m_saveAndRun)
		{
			startTimer(timer->m_id);
		}
		m_container->SaveButton();
		if(m_changesHandle)
		{
			SetEvent(m_changesHandle);
		}
	}
	m_container->ShowMenu();
}

void CTimerMainBtn::addTimerIconLocation( LPCWSTR val )
{
	m_iconLocationAddBtn = val ? val : L"";
	m_container->SaveButton();
}

void CTimerMainBtn::loadSkin()
{
	m_skin.loadSkin(m_skinFileBtn);
	m_skinCtl.loadSkin(m_skinFileCtrls);
}

void CTimerMainBtn::onTimerStarted( DWORD id )
{
	m_activeTimers++;
	m_skin.addSlice(id);
	if(m_showTimers)
	{
		m_container->UpdateSize();
	}
}

void CTimerMainBtn::onTimerStopped( DWORD id )
{
	m_activeTimers--;
	m_skin.deleteSlice(id);
	if(m_showTimers)
	{
		m_container->UpdateSize();
	}
}

void CTimerMainBtn::GetSize( SIZE* sz, BOOL actual )
{
	if(m_activeTimers && m_showTimers)
	{
		sz->cx = m_skin.width();
		sz->cy = m_skin.height();
		switch(m_ctlPos & CTL_SIDE_MASK)
		{
		case CTL_SIDE_LEFT:
		case CTL_SIDE_RIGHT:
			sz->cx += m_skinCtl.width();
			sz->cy = max(m_skinCtl.height(), sz->cy);
			break;
		case CTL_SIDE_TOP:
		case CTL_SIDE_BOTTOM:
			sz->cy += m_skinCtl.height();
			sz->cx = max(m_skinCtl.width(), sz->cx);
			break;
		}
	} else
	{
		CTlbButton::GetSize(sz, actual);
	}
}

void CTimerMainBtn::OnDraw( HDC hDC, LPRECT rcItem )
{
	if(!m_activeTimers || !m_showTimers)
	{
		CTlbButton::OnDraw(hDC, rcItem);
	} else
	{
		RECT rcTimers;
		RECT rcCtl;
		getTimersRect(&rcTimers);
		m_skin.draw(hDC, rcItem->left + rcTimers.left, rcItem->top + rcTimers.top);
		if(getCtlRect(&rcCtl))
		{
			m_skinCtl.draw(hDC, rcItem->left + rcCtl.left, rcItem->top + rcCtl.top);
		}
	}
}

void CTimerMainBtn::GetButtonText( LPWSTR text )
{
	if(m_activeTimers && m_showTimers)
	{
		text[0] = 0;
	} else
	{
		CTlbButton::GetButtonText(text);
	}
}

BOOL CTimerMainBtn::OnMouseOver( long keys, int x, int y )
{
	if(!m_activeTimers || !m_showTimers)
	{
		return CTlbButton::OnMouseOver(keys, x, y);
	}
	RECT rcTimers;
	RECT rcCtl;
	getTimersRect(&rcTimers);
	if(getCtlRect(&rcCtl))
	{
		if(m_skinCtl.mouseOver(x - m_rc.left - rcCtl.left, y - m_rc.top - rcCtl.top))
		{
			m_container->Redraw();
		}
	}
	if(m_skin.mouseOver(x - m_rc.left - rcTimers.left, y - m_rc.top - rcTimers.top))
	{
		m_container->Redraw();
	}
	return TRUE;
}

BOOL CTimerMainBtn::OnMouseLeave( long keys )
{
	if(!m_activeTimers || !m_showTimers)
	{
		return CTlbButton::OnMouseLeave(keys);
	}
	RECT rcTimers;
	RECT rcCtl;
	getTimersRect(&rcTimers);
	if(getCtlRect(&rcCtl))
	{
		if(m_skinCtl.mouseLeave())
		{
			m_container->Redraw();
		}
	}
	if(m_skin.mouseLeave())
	{
		m_container->Redraw();
	}
	return TRUE;
}

BOOL CTimerMainBtn::OnMouseEnter( long keys, int x, int y )
{
	if(!m_activeTimers || !m_showTimers)
	{
		return CTlbButton::OnMouseEnter(keys, x, y);
	}
	RECT rcTimers;
	RECT rcCtl;
	getTimersRect(&rcTimers);
	if(getCtlRect(&rcCtl))
	{
		if(m_skinCtl.mouseOver(x - m_rc.left - rcCtl.left, y - m_rc.top - rcCtl.top))
		{
			m_container->Redraw();
		}
	}
	if(m_skin.mouseOver(x - m_rc.left - rcTimers.left, y - m_rc.top - rcTimers.top))
	{
		m_container->Redraw();
	}
	return TRUE;
}

BOOL CTimerMainBtn::OnLButtonUp( long key, int x, int y )
{
	if(!m_activeTimers || !m_showTimers)
	{
		return CTlbButton::OnLButtonUp(key, x, y);
	}
	RECT rcTimers;
	RECT rcCtl;
	getTimersRect(&rcTimers);
	if(getCtlRect(&rcCtl))
	{
		if(m_skinCtl.lButtonUp(x - m_rc.left - rcCtl.left, y - m_rc.top - rcCtl.top))
		{
			m_container->Redraw();
		}
	}
	if(m_skin.lButtonUp(x - m_rc.left - rcTimers.left, y - m_rc.top - rcTimers.top))
	{
		m_container->Redraw();
	}
	return TRUE;
}

BOOL CTimerMainBtn::OnLButtonDown( long key, int x, int y )
{
	if(!m_activeTimers || !m_showTimers)
	{
		return CTlbButton::OnLButtonDown(key, x, y);
	}
	RECT rcTimers;
	RECT rcCtl;
	getTimersRect(&rcTimers);
	if(getCtlRect(&rcCtl))
	{
		if(m_skinCtl.lButtonDown(x - m_rc.left - rcCtl.left, y - m_rc.top - rcCtl.top))
		{
			m_container->Redraw();
		}
	}
	if(m_skin.lButtonDown(x - m_rc.left - rcTimers.left, y - m_rc.top - rcTimers.top))
	{
		m_container->Redraw();
	}
	return TRUE;
}

void CTimerMainBtn::OnElementClick( LPCWSTR id, int sliceID )
{
	if(!StrCmpI(id, L"stop"))
	{
		startTimer(sliceID);
	} else if(!StrCmpI(id, L"menu"))
	{
		m_container->ShowMenu();
	} else if(!StrCmpI(id, L"stopall"))
	{
		PostMessage(m_hWndTimer, WM_ONSTOPALL, (WPARAM) id, NULL);
	}
}

void CTimerMainBtn::OnSliderValue( LPCWSTR id, int val, int sliceID )
{

}

BOOL CTimerMainBtn::OnStateChanged( int state, int* needRedraw )
{
	if(m_activeTimers && m_showTimers)
	{
		if(state)
		{
			m_container->SetState(0);
		}
		*needRedraw = FALSE;
		return TRUE;
	}
	return CTlbButton::OnStateChanged(state, needRedraw);
}

BOOL CTimerMainBtn::LoadIcon( INT iSize )
{
	if(!CTlbButton::LoadIcon( iSize ) )
	{
		m_hIcon = 
			(HICON) LoadImage(
			g_hInst, 
			MAKEINTRESOURCE( IDI_TIMER ), 
			IMAGE_ICON, 
			iSize, 
			iSize, 
			LR_DEFAULTCOLOR );

		m_loadedIconSize = iSize;
	}

	return m_hIcon ? TRUE : FALSE;
}

void CTimerMainBtn::getTimersRect( LPRECT rcSkin )
{
	switch(m_ctlPos & CTL_SIDE_MASK)
	{
	case CTL_SIDE_LEFT:
		rcSkin->left	= m_skinCtl.width();
		rcSkin->top		= 0;
		break;
	case CTL_SIDE_TOP:
		rcSkin->left	= 0;
		rcSkin->top		= m_skinCtl.height();
		break;
	default:
		rcSkin->left	= 0;
		rcSkin->top		= 0;
	}
	rcSkin->right	= rcSkin->left	+ m_skin.width();
	rcSkin->bottom	= rcSkin->top	+ m_skin.height();
}

BOOL CTimerMainBtn::getCtlRect( LPRECT rcSkin )
{
	switch(m_ctlPos & CTL_SIDE_MASK)
	{
	case CTL_SIDE_HIDE:
		return FALSE;
	case CTL_SIDE_RIGHT:
		rcSkin->left	= m_skin.width();
		switch(m_ctlPos & CTL_POS_MASK)
		{
		case CTL_POS_FAR:
			rcSkin->top = (m_rc.bottom - m_rc.top) - m_skinCtl.height();
			break;
		case CTL_POS_CENTER:
			rcSkin->top = (m_rc.bottom - m_rc.top) / 2 - m_skinCtl.height() / 2;
			break;
		default:
			rcSkin->top = 0;
		}
		break;
	case CTL_SIDE_LEFT:
		rcSkin->left	= 0;
		switch(m_ctlPos & CTL_POS_MASK)
		{
		case CTL_POS_FAR:
			rcSkin->top = (m_rc.bottom - m_rc.top) - m_skinCtl.height();
			break;
		case CTL_POS_CENTER:
			rcSkin->top = (m_rc.bottom - m_rc.top) / 2 - m_skinCtl.height() / 2;
			break;
		default:
			rcSkin->top = 0;
		}
		break;
	case CTL_SIDE_TOP:
		rcSkin->top		= 0;
		switch(m_ctlPos & CTL_POS_MASK)
		{
		case CTL_POS_FAR:
			rcSkin->left = (m_rc.right - m_rc.left) - m_skinCtl.width();
			break;
		case CTL_POS_CENTER:
			rcSkin->left = (m_rc.right - m_rc.left) / 2 - m_skinCtl.width() / 2;
			break;
		default:
			rcSkin->left = 0;
		}
		break;
	case CTL_SIDE_BOTTOM:
		rcSkin->top		= m_skin.height();
		switch(m_ctlPos & CTL_POS_MASK)
		{
		case CTL_POS_FAR:
			rcSkin->left = (m_rc.right - m_rc.left) - m_skinCtl.width();
			break;
		case CTL_POS_CENTER:
			rcSkin->left = (m_rc.right - m_rc.left) / 2 - m_skinCtl.width() / 2;
			break;
		default:
			rcSkin->left = 0;
		}
		break;
	default:
		rcSkin->left	= 0;
		rcSkin->top		= 0;
	}
	rcSkin->right	= rcSkin->left	+ m_skinCtl.width();
	rcSkin->bottom	= rcSkin->top	+ m_skinCtl.height();

	return TRUE;
}

void CTimerMainBtn::onStartTimer( DWORD id )
{
	CTimer* tm = NULL;
	lock();
	for(size_t i = 0; i < m_timers.size(); i++)
	{
		if(m_timers[i]->m_id == id)
		{
			tm = m_timers[i];
		}
	}
	if(tm)
	{
		if(!tm->m_startTime)
		{
			tm->m_startTime = GetTickCount();
			tm->m_curTime	= GetTickCount();
			onTimerStarted(id);
		} else
		{
			tm->m_startTime = 0;
			tm->m_curTime	= 0;
			if(m_changesHandle)
			{
				SetEvent(m_changesHandle);
			}
			onTimerStopped(id);
		}
		start();
	}
	unlock();
}

void CTimerMainBtn::deleteTimer( DWORD id )
{
	PostMessage(m_hWndTimer, WM_ONSDELELETIMER, (WPARAM) id, NULL);
}

void CTimerMainBtn::onDeleteTimer( DWORD id )
{
	BOOL found = FALSE;
	lock();
	for(size_t i = 0; i < m_timers.size(); i++)
	{
		if(m_timers[i]->m_id == id)
		{
			found = TRUE;
			delete m_timers[i];
			m_timers.erase(m_timers.begin() + i);
			break;
		}
	}
	unlock();
	if(found && m_changesHandle)
	{
		SetEvent(m_changesHandle);
	}
	if(found)
	{
		m_container->SaveButton();
	}
}

void CTimerMainBtn::openMenu()
{
	if(!m_changesHandle)
	{
		m_container->ShowMenu();
	}
}

BOOL CTimerMainBtn::SupportSortMenu()
{
	return TRUE;
}

BOOL CTimerMainBtn::QueryArrangeMenu( UINT idx, ARRANGE_MENU_DATA* qamData )
{
	switch(idx)
	{
	case AMID_TIME - 1:
		qamData->sortID = AMID_TIME;
		StringCchCopy(qamData->name, 100, m_xui.getStringDef(XUI_STR_ARRANGE_TIME));
		return TRUE;
	case AMID_NAME - 1:
		qamData->sortID = AMID_NAME;
		StringCchCopy(qamData->name, 100, m_xui.getStringDef(XUI_STR_ARRANGE_NAME));
		return TRUE;
	}
	return FALSE;
}

void CTimerMainBtn::CompareButtons( COMPARE_BUTTONS_DATA* cpBtns )
{
	CTimerBtn* btn1 = NULL;
	CTimerBtn* btn2 = NULL;
	cpBtns->btn1->QueryClass(CLSID_TIMERBTN, (LPVOID*) &btn1);
	cpBtns->btn2->QueryClass(CLSID_TIMERBTN, (LPVOID*) &btn2);
	if(btn1 && btn2)
	{
		CTimer* tm1 = getTimer(btn1->getID());
		CTimer* tm2 = getTimer(btn2->getID());
		if(tm1 && tm2)
		{
			switch(cpBtns->fnc)
			{
			case AMID_NAME:
				if(tm1->m_name && tm2->m_name)
				{
					cpBtns->result = StrCmpI(tm1->m_name, tm2->m_name);
					if(cpBtns->result == 0)
					{
						if(tm1->m_id > tm2->m_id)
						{
							cpBtns->result = -1;
						} else
						{
							cpBtns->result = 1;
						}
					}
				} else if(tm1->m_name && !tm2->m_name)
				{
					cpBtns->result = -1;
				} else
				{
					cpBtns->result = 1;
				}
				break;
			default:
				if(tm1->m_time > tm2->m_time)
				{
					cpBtns->result = 1;
				} else
				{
					cpBtns->result = -1;
				}
				break;
			}
		} else if(tm1 && !tm2)
		{
			cpBtns->result = 1;
		} else
		{
			cpBtns->result = -1;
		}
		if(tm1)	delete tm1;
		if(tm2)	delete tm2;
	} else if(btn1 && !btn2)
	{
		cpBtns->result = 1;
	} else if(!btn1 && btn2)
	{
		cpBtns->result = -1;
	}
}

void CTimerMainBtn::onStopAllTimers()
{
	BOOL update = FALSE;
	lock();
	for(size_t i = 0; i < m_timers.size(); i++)
	{
		if(m_timers[i]->m_startTime)
		{
			m_timers[i]->m_startTime	= 0;
			m_timers[i]->m_curTime		= 0;
			onTimerStopped(m_timers[i]->m_id);
			update = TRUE;
		}
	}
	if(update)
	{
		if(m_changesHandle)
		{
			SetEvent(m_changesHandle);
		}
		start();
	}
	unlock();
}

BOOL CTimerMainBtn::applyMargins()
{
	return FALSE;
}

void CTimerMainBtn::closeMenu()
{
	m_container->ShowMenu();
}

BOOL CTimerMainBtn::OnSetLCID( DWORD dwLCID, HMODULE hInstance )
{
	TCHAR szLCID[20];
	wsprintf(szLCID, TEXT("%d"), dwLCID);

	TCHAR langPath[MAX_PATH];
	LONG cbData = sizeof(langPath);
	RegQueryValue(HKEY_CLASSES_ROOT, TEXT("CLSID\\{FC712CA0-A945-11D4-A594-956F6349FC18}\\InprocServer32"), langPath, &cbData);
	PathRemoveFileSpec(langPath);
	PathAddBackslash(langPath);
	lstrcat(langPath, TEXT("langs\\"));
	lstrcat(langPath, szLCID);
	lstrcat(langPath, TEXT("\\timer.xml"));
	m_xui.clearStrings();
	m_xui.loadStrings(langPath);

	return TRUE;
}
