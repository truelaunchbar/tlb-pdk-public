#pragma once
#include "tlbif.h"
#include "tlbpdklib.h"

class CPlugin : public ITlbButton,
				public CTlbContainer
{
	UINT			m_ObjRefCount;		// Object reference count

	CTlbButton*		m_btn;
	ITlbContainer*	m_container;		// Button container
public:
	CPlugin(CTlbButton* btn = NULL);
	virtual ~CPlugin(void);

	//IUnknown methods
	STDMETHODIMP QueryInterface(REFIID, LPVOID*);
	STDMETHODIMP_(DWORD) AddRef();
	STDMETHODIMP_(DWORD) Release();

	//ITlbButton methods
	STDMETHODIMP Load(IStream *data);
	STDMETHODIMP Save(IStream *data);
	STDMETHODIMP SetRect(RECT *rcItem);
	STDMETHODIMP SetContainer(ITlbContainer *cont);
	STDMETHODIMP SetID(int btnID);
	STDMETHODIMP Draw(HANDLE hDC, RECT *rcItem);
	STDMETHODIMP OnMouseOver(long keys, int x, int y);
	STDMETHODIMP OnMouseLeave(long keys);
	STDMETHODIMP OnMouseEnter(long keys, int x, int y);
	STDMETHODIMP Create(IStream* data);
	STDMETHODIMP StateChanged(int state, int* needRedraw);
	STDMETHODIMP GetDescription(char* szDescription);
	STDMETHODIMP OnLButtonDown(long key, int x, int y);
	STDMETHODIMP OnLButtonUp(long key, int x, int y);
	STDMETHODIMP Run();
	STDMETHODIMP DoAction(int action);
	STDMETHODIMP SetName(LPCSTR szName);
	STDMETHODIMP_(int) QueryContextMenu(HMENU hMenu, int index, int cmdFirst, int cmdLast);
	STDMETHODIMP InvokeCommand(int ID);
	STDMETHODIMP SetLCID(DWORD dwLCID, HMODULE hResModule);
	STDMETHODIMP GetIconLocation(LPSTR szLocation);
	STDMETHODIMP SetIconLocation(LPCSTR szLocation);
	STDMETHODIMP GetButtonInfo(BUTTONINFO* btninfo);
	STDMETHODIMP Invoke(DWORD id, WPARAM wParam, LPARAM lParam);

	//CTlbContainer members
	virtual void		DrawItemText(HDC hDC, LPRECT rcText, LPCWSTR text);
	virtual HWND		GetToolbarWindow();
	virtual void		GetIconPoint(LPPOINT ptIcon);
	virtual int			GetIconSize();
	virtual DWORD		GetViewMode();
	virtual HFONT		GetItemFont();
	virtual COLORREF	GetTextColor();
	virtual COLORREF	GetTextGlowColor();
	virtual void		GetTextMargins(LPRECT rcMargins);
	virtual void		GetContentMargins(LPRECT rcMargins);
	virtual void		SetHotKey(WORD hk, UINT hkID);
	virtual BOOL		isWordWrap();
	virtual BOOL		showHotkey();
	virtual void		GetFolder(LPWSTR path, UINT folder_type);
	virtual BOOL		IsPortable();
	virtual BOOL		IsVisible();
	virtual BOOL		InMenu();
	virtual void		Hide();
	virtual void		Show();
	virtual void		QueryCreateWindow();
	virtual void		OpenParentMenu(HWND focusWnd = NULL);
	virtual void		OnKillFocus();
	virtual void		OnRButtonUp();
	virtual void		GetSavedPath(UINT objID, LPWSTR path);
	virtual void		SetState(UINT state);
	virtual void		SaveButton();
	virtual void		UpdateSize();
	virtual void		Redraw();
	virtual void		UpdateTips();
	virtual void		UpdateTip(UINT tipID, BOOL re_render = FALSE);
	virtual void		ShowMenu();
	virtual void		switchToolbar(LPWSTR path);
	virtual DWORD		GetButtonType();
	virtual void		EnterContextMenu();
	virtual void		LeaveContextMenu();
	virtual DWORD		GetStyle();
	virtual HWND		GetLastWindow();
	virtual void		SetLastWindow(HWND hWnd);
	virtual void		drawText(HDC hdc, LPCWSTR text, int c, LPRECT rcText, UINT dtParams, UINT flags, HFONT hFont, COLORREF color, INT vertical, COLORREF glowColor, INT glowSize);
	virtual BOOL		requestKbdFocus();
	virtual void		releaseKbdFocus();
	virtual DWORD		GetMaxMenuWidth();
	virtual void		AddTipImage(int tipID, LPCWSTR url, void* bits, int width, int height, BOOL redraw_only);

	virtual BOOL		tboShowCtxIcons();
	virtual BOOL		selectIcon(HWND parent, LPWSTR iconLocation);
};
