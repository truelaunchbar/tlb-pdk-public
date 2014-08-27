#pragma once

class CTimerMainBtn;

class CAddTimerBtn : public CTlbButton
{
	CTimerMainBtn*	m_parent;
public:
	CAddTimerBtn(CTimerMainBtn* parent);
	virtual ~CAddTimerBtn(void);

	virtual UINT	GetSupportedActions();
	virtual UINT	GetModeFlags();
	virtual void	GetChildUID(LPWSTR uid);
	virtual void	GetButtonText(LPWSTR text);
	virtual BOOL	SetIconLocation(LPCWSTR szLocation);
	virtual void	GetButtonName(LPWSTR name);
	virtual BOOL	QueryClass(CLSID classID, LPVOID* lpClass);
	virtual void	OnHotkey(UINT hkID);

	virtual LPWSTR	GetTipText(int tipID);
	virtual LPWSTR	GetTipCaption(int tipID);
	virtual HICON	GetTipIcon(int tipID);
	virtual int		GetTipsIconSize(int tipID);
	virtual void	DrawTipsIcon(HDC hDC, POINT pt, int tipID);

	virtual BOOL	LoadIcon(INT iSize);
	virtual BOOL	OnRun();
};
