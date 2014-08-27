#pragma once
#include "MCIPlayer.h"
#include "TimerSkin.h"
#include "MainBtnSkin.h"
#include "NotifySkin.h"
#include "CtlSkin.h"

class CPropertiesDlg :	public CXUIDialog,
						public TxSkin::IEnumSkinsCallBack
{
	TMCIPlayer<CPropertiesDlg>	m_player;
	BOOL						m_bPlayerStopped;
	std::vector<std::wstring>	m_skinsMenu;
	std::vector<std::wstring>	m_skinsBtn;
	std::vector<std::wstring>	m_skinsCtrls;
	std::vector<std::wstring>	m_skinsNotify;

	CMainBtnSkin	m_skinBtn;
	CCtlSkin		m_skinCtrls;
	CTimerSkin		m_skinMenu;
	CNotifySkin		m_skinNotify;

	CXUIComboBox*	m_ctlSkinMenu;
	CXUIComboBox*	m_ctlSkinBtn;
	CXUIComboBox*	m_ctlSkinCtrls;
	CXUIComboBox*	m_ctlSkinNotify;

	CXUIFreeDraw*	m_ctlDrawMenu;
	CXUIFreeDraw*	m_ctlDrawBtn;
	CXUIFreeDraw*	m_ctlDrawNotify;
	LPWSTR			m_strSndFile;
public:
	std::wstring	m_soundFile;
	BOOL			m_playSound;
	DWORD			m_pos;
	BOOL			m_showWindow;
	int				m_rowColsMode;
	int				m_RowsCols;
	UINT			m_ctlPos;
	BOOL			m_showTimers;
	BOOL			m_closeMenuOnStart;
	std::wstring	m_skinFileMenu;
	std::wstring	m_skinFileBtn;
	std::wstring	m_skinFileNotify;
	std::wstring	m_skinFileCtrls;

	CPropertiesDlg(CXUIEngine* engine);
	virtual ~CPropertiesDlg(void);

	virtual void OnInitDialog();

	virtual BOOL OnEndDialog(UINT code);

	virtual void onSkinFileFound(LPCWSTR skinFile, LPWSTR shortPath);

	XUI_BEGIN_DATA_MAP
		XUI_DATA_MAP_PSTR		(L"soundFile",				m_strSndFile)
		XUI_DATA_MAP_INT		(L"playSound",				m_playSound)
		XUI_DATA_MAP_INTMASK	(L"hpos", NWND_HPOS_MASK,	m_pos)
		XUI_DATA_MAP_INTMASK	(L"vpos", NWND_VPOS_MASK,	m_pos)
		XUI_DATA_MAP_INTMASK	(L"ctlSide", CTL_SIDE_MASK,	m_ctlPos)
		XUI_DATA_MAP_INTMASK	(L"ctlPos", CTL_POS_MASK,	m_ctlPos)
		XUI_DATA_MAP_INT		(L"rowColsMode",			m_rowColsMode)
		XUI_DATA_MAP_INT		(L"RowsCols",				m_RowsCols)
		XUI_DATA_MAP_INT		(L"showWindow",				m_showWindow)
		XUI_DATA_MAP_INT		(L"closeOnStart",			m_closeMenuOnStart)
	XUI_END_DATA_MAP

	XUI_BEGIN_BIND_MAP
		XUI_BIND_ITEM(L"menuSkin",	 		L"combobox", m_ctlSkinMenu)
		XUI_BIND_ITEM(L"mainBtnSkin",		L"combobox", m_ctlSkinBtn)
		XUI_BIND_ITEM(L"notifySkin",		L"combobox", m_ctlSkinNotify)
		XUI_BIND_ITEM(L"ctrlsSkin",			L"combobox", m_ctlSkinCtrls)

		XUI_BIND_ITEM(L"drawMenuSkin",	 	L"freedraw", m_ctlDrawMenu)
		XUI_BIND_ITEM(L"drawMainBtnSkin",	L"freedraw", m_ctlDrawBtn)
		XUI_BIND_ITEM(L"drawNotifySkin",	L"freedraw", m_ctlDrawNotify)
	XUI_END_BIND_MAP

	XUI_BEGIN_EVENT_MAP
		XUI_HANDLE_CLICKED	(L"play",				OnPlay)
		XUI_HANDLE_FREEDRAW	(L"drawMenuSkin",		OnDrawMenuSkin)
		XUI_HANDLE_FREEDRAW	(L"drawMainBtnSkin",	OnDrawBtnSkin)
		XUI_HANDLE_FREEDRAW	(L"drawNotifySkin",		OnDrawNotifySkin)

		XUI_HANDLE_CHANGED	(L"menuSkin",			OnChangedMenuSkin)
		XUI_HANDLE_CHANGED	(L"mainBtnSkin",		OnChangedBtnSkin)
		XUI_HANDLE_CHANGED	(L"rowColsMode",		OnChangedBtnSkin)
		XUI_HANDLE_CHANGED	(L"RowsCols",			OnChangedBtnSkin)
		XUI_HANDLE_CHANGED	(L"notifySkin",			OnChangedNotifySkin)
		XUI_HANDLE_CHANGED	(L"ctrlsSkin",			OnChangedCtrlSkin)
		XUI_HANDLE_CHANGED	(L"ctlSide",			OnRedrawBtnSkin)
		XUI_HANDLE_CHANGED	(L"ctlPos",				OnRedrawBtnSkin)
	XUI_END_EVENT_MAP

private:
	void	OnSoundStop();
	BOOL	OnPlay();
	void	loadSkins();
	void	initSkins();
	BOOL	OnDrawMenuSkin(HDC hdc, LPRECT rcDraw);
	BOOL	OnDrawBtnSkin(HDC hdc, LPRECT rcDraw);
	BOOL	OnDrawNotifySkin(HDC hdc, LPRECT rcDraw);
	BOOL	OnChangedMenuSkin();
	BOOL	OnChangedBtnSkin();
	BOOL	OnChangedCtrlSkin();
	BOOL	OnChangedNotifySkin();
	BOOL	OnRedrawBtnSkin();

	void	getTimersRect(LPRECT rcSkin);
	BOOL	getCtlRect(LPRECT rcSkin);
};
