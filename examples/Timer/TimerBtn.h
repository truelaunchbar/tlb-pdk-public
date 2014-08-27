#pragma once

#include "TimerSkin.h"

class CTimerMainBtn;

class CTimerBtn :	public CTlbButton,
					public ISkinCallBack
{
	CTimerMainBtn*	m_parent;
	DWORD			m_timerID;
	int				m_state;
	CTimerSkin		m_skin;
	std::wstring	m_oldSkin;

public:
	CTimerBtn(CTimerMainBtn* parent, DWORD timerID);

	virtual ~CTimerBtn(void);

	virtual void	GetChildUID(LPWSTR uid);
	virtual void	GetSize(SIZE* sz, BOOL actual);
	virtual void	OnDraw(HDC hDC, LPRECT rcItem);
	virtual UINT	GetModeFlags(void);
	virtual UINT	ReadChanges();
	virtual UINT	GetSupportedActions();
	virtual BOOL	OnProperties();
	virtual void	OnHotkey(UINT hkID);
	virtual void	GetButtonText(LPWSTR text);
	virtual BOOL	OnDelete();
	virtual BOOL	QueryClass(CLSID classID, LPVOID* lpClass);

	virtual BOOL	OnMouseOver(long keys, int x, int y);
	virtual BOOL	OnMouseLeave(long keys);
	virtual BOOL	OnMouseEnter(long keys, int x, int y);
	virtual BOOL	OnLButtonUp(long key, int x, int y);
	virtual BOOL	OnLButtonDown(long key, int x, int y);
	virtual BOOL	OnStateChanged(int state, int* needRedraw);

	virtual LPWSTR	GetTipText(int tipID);
	virtual LPWSTR	GetTipCaption(int tipID);
	virtual HICON	GetTipIcon(int tipID);
	virtual int		GetTipsIconSize(int tipID);
	virtual void	DrawTipsIcon(HDC hDC, POINT pt, int tipID);

	// public ISkinCallBack
	virtual void	OnElementClick(LPCWSTR id, int sliceID);
	virtual void	OnSliderValue(LPCWSTR id, int val, int sliceID);

	DWORD getID()	{ return m_timerID; }

private:
	void loadSkin();
	void updateName();
};
