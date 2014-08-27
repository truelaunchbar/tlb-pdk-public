#pragma once

#include "TimerXUI.h"
#include "timerclass.h"
#include "TxThread.h"
#include <vector>
#include "MCIPlayer.h"
#include "TxWnd.h"
#include "MainBtnSkin.h"
#include "CtlSkin.h"

class CNotifyWnd;

#define WM_ONRUNTIMER			(WM_USER + 1000)
#define WM_ONADDTIMER			(WM_USER + 1001)
#define WM_ONSTARTTIMER			(WM_USER + 1002)
#define WM_ONSDELELETIMER		(WM_USER + 1003)
#define WM_ONSTOPALL			(WM_USER + 1004)


#define AMID_TIME		1
#define AMID_NAME		2

class CTimerMainBtn :	public CTlbButton,
						public CTxThread,
						public ISkinCallBack
{
	CTimerXUI					m_xui;
	std::vector<CTimer*>		m_timers;
	std::vector<CMCIPlayer*>	m_players;
	std::vector<CNotifyWnd*>	m_notifyWnds;
	DWORD						m_nextID;
	HANDLE						m_changesHandle;
	TTxWnd<CTimerMainBtn>		m_hWndTimer;
	std::wstring				m_soundFile;
	BOOL						m_playSound;
	DWORD						m_pos;
	BOOL						m_showWindow;
	std::wstring				m_iconLocationAddBtn;
	CMainBtnSkin				m_skin;
	CCtlSkin					m_skinCtl;
	UINT						m_ctlPos;
	BOOL						m_activeTimers;
	int							m_rowColsMode;
	int							m_RowsCols;
	BOOL						m_showTimers;
	BOOL						m_closeMenuOnStart;
	CRITICAL_SECTION			m_sync;

	std::wstring				m_skinFileMenu;
	std::wstring				m_skinFileBtn;
	std::wstring				m_skinFileCtrls;
	std::wstring				m_skinFileNotify;
public:
	CTimerMainBtn(void);
	virtual ~CTimerMainBtn(void);

	std::wstring&	getMenuSkin()	{ return m_skinFileMenu;	}
	std::wstring&	getNotifySkin()	{ return m_skinFileNotify;	}
	CTimer*			getTimer(DWORD id);
	void			startTimer(DWORD id);
	void			deleteTimer(DWORD id);
	CXUIEngine*		getXUI()	{ return &m_xui; }
	void			saveButton();
	void			editTimer(DWORD timerID, CTimer* timer);
	void			stopPlayer(DWORD id);
	void			addNewTimer();
	LPCWSTR			addTimerIconLocation()	{ return m_iconLocationAddBtn.c_str(); }
	void			addTimerIconLocation(LPCWSTR val);
	void			openMenu();
	void			lock()		{ EnterCriticalSection(&m_sync); }
	void			unlock()	{ LeaveCriticalSection(&m_sync); }
	BOOL			closeOnStart()	{ return m_closeMenuOnStart; }
	void			closeMenu();

	virtual BOOL	isMenu();
	virtual BOOL	Save(IStream *data);
	virtual BOOL	Load(IStream *data);
	virtual BOOL	SupportGlassMenu();
	virtual BOOL	OnCreate();
	virtual BOOL	OnProperties();
	virtual void	OnHotkey(UINT hkID);
	virtual BOOL	applyMargins();

	virtual BOOL	OnMouseOver(long keys, int x, int y);
	virtual BOOL	OnMouseLeave(long keys);
	virtual BOOL	OnMouseEnter(long keys, int x, int y);
	virtual BOOL	OnLButtonUp(long key, int x, int y);
	virtual BOOL	OnLButtonDown(long key, int x, int y);

	virtual UINT	GetSupportedActions();
	virtual UINT	GetModeFlags();
	virtual UINT	GetMenuFlags();
	virtual void	GetSize(SIZE* sz, BOOL actual);
	virtual void	OnDraw(HDC hDC, LPRECT rcItem);
	virtual void	GetButtonText(LPWSTR text);
	virtual BOOL	OnStateChanged(int state, int* needRedraw);
	virtual BOOL	LoadIcon(INT iSize);

	virtual HANDLE	GetChangesHandle();
	virtual void	CloseChangesHandle(HANDLE hChanges);
	virtual BOOL	FirstChildData(CHILDS_DATA* data);
	virtual BOOL	NextChildData(CHILDS_DATA* data);
	virtual BOOL	FreeChildData(CHILDS_DATA* data);
	virtual BOOL	CreateChild(CREATE_CHILD_DATA* childData);
	virtual BOOL	OnSetLCID(DWORD dwLCID, HMODULE hInstance);

	virtual BOOL	QueryArrangeMenu(UINT idx, ARRANGE_MENU_DATA* qamData);
	virtual BOOL	SupportSortMenu();
	virtual void	CompareButtons(COMPARE_BUTTONS_DATA* cpBtns);

	virtual DWORD	ThreadProc();

	// public ISkinCallBack
	virtual void	OnElementClick(LPCWSTR id, int sliceID);
	virtual void	OnSliderValue(LPCWSTR id, int val, int sliceID);

private:
	virtual LRESULT WndProcTimer(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

	BOOL	openProperties();
	void	start();
	void	stop();
	void	OnTimer();
	void	raiseTimer(DWORD id);
	void	onRunTimer(DWORD id);
	void	onDeleteTimer(DWORD id);
	void	onStartTimer(DWORD id);
	void	onStopAllTimers();
	void	onAddTimer();
	void	loadSkin();
	void	onTimerStarted(DWORD id);
	void	onTimerStopped(DWORD id);
	void	getTimersRect(LPRECT rcSkin);
	BOOL	getCtlRect(LPRECT rcSkin);
};
