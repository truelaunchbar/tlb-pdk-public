#pragma once

#include "..\TLBPDK\tlbpdk.h"

// Interface to query container of plugin
interface CTlbContainer
{
	virtual void		DrawItemText(HDC hDC, LPRECT rcText, LPCWSTR text) = 0;
	virtual HWND		GetToolbarWindow() = 0;
	virtual void		GetIconPoint(LPPOINT ptIcon) = 0;
	virtual int			GetIconSize() = 0;
	virtual DWORD		GetButtonType() = 0;
	virtual DWORD		GetViewMode() = 0;
	virtual DWORD		GetStyle() = 0;
	virtual HFONT		GetItemFont() = 0;
	virtual COLORREF	GetTextColor() = 0;
	virtual COLORREF	GetTextGlowColor() = 0;
	virtual void		GetTextMargins(LPRECT rcMargins) = 0;
	virtual void		GetContentMargins(LPRECT rcMargins) = 0;
	virtual void		SetHotKey(WORD hk, UINT hkID) = 0;
	virtual BOOL		IsVisible() = 0;
	virtual BOOL		isWordWrap() = 0;
	virtual BOOL		InMenu() = 0;
	virtual void		Hide() = 0;
	virtual void		Show() = 0;
	virtual void		QueryCreateWindow() = 0;
	virtual void		OpenParentMenu(HWND focusWnd = NULL) = 0;
	virtual void		OnKillFocus() = 0;
	virtual void		OnRButtonUp() = 0;
	virtual void		GetSavedPath(UINT objID, LPWSTR path) = 0;
	virtual void		SetState(UINT state) = 0;
	virtual void		SaveButton() = 0;
	virtual void		UpdateSize() = 0;
	virtual void		Redraw() = 0;
	virtual void		UpdateTips() = 0;
	virtual void		UpdateTip(UINT tipID, BOOL re_render = FALSE) = 0;
	virtual void		ShowMenu() = 0;
	virtual BOOL		tboShowCtxIcons() = 0;
	virtual void		switchToolbar(LPWSTR path) = 0;
	virtual void		EnterContextMenu() = 0;
	virtual void		LeaveContextMenu() = 0;
	virtual HWND		GetLastWindow() = 0;
	virtual void		SetLastWindow(HWND hWnd) = 0;
	virtual void		drawText(HDC hdc, LPCWSTR text, int c, LPRECT rcText, UINT dtParams, UINT flags = 0, HFONT hFont = NULL, COLORREF color = 0, INT vertical = FALSE, COLORREF glowColor = 0, INT glowSize = 0) = 0;
	virtual BOOL		requestKbdFocus() = 0;
	virtual void		releaseKbdFocus() = 0;
	virtual DWORD		GetMaxMenuWidth() = 0;
	virtual BOOL		selectIcon(HWND parent, LPWSTR iconLocation) = 0;
	virtual void		GetFolder(LPWSTR path, UINT folder_type) = 0;
	virtual BOOL		IsPortable() = 0;
	virtual void		AddTipImage(int tipID, LPCWSTR url, void* bits, int width, int height, BOOL redraw_only) = 0;
};

class CTlbButton;

struct HOTKEY_DATA
{
    DWORD	id;
    WCHAR	name[260];
    WORD	hotKey;
    DWORD	hotKeyID;
};

struct CHILDS_DATA
{
    LPWSTR	Name;
    LPVOID	childData;
    LPVOID	enumData;
};

struct CREATE_CHILD_DATA
{
    CTlbButton*	parent;
    LPVOID		childData;
    CTlbButton*	child;
};

struct COMPARE_BUTTONS_DATA
{
    CTlbButton*	btn1;
    CTlbButton*	btn2;
    UINT		fnc;
    int			result;
};

struct ARRANGE_MENU_DATA
{
    int			sortID;
    LPWSTR		name;
};

struct VERB
{
	typedef std::vector<VERB>	vector;

	std::wstring text;
	std::wstring verb;

	VERB(const VERB& v)
	{
		text = v.text;
		verb = v.verb;
	}
	VERB& operator=(const VERB& v)
	{
		text = v.text;
		verb = v.verb;
	}
};

class CTxDIB;

// Plugin button class
class CTlbButton
{
private:
	HBITMAP*		m_menuBitmaps;
	int				m_bmpCount;

	void AddBitmap(HBITMAP bmp);
	void ClearBitmaps();
protected:
	RECT			m_rc;				// Rectangle of button
	DWORD			m_ID;				// ID of button
	HINSTANCE		m_hResModule;		// Resource module
	WCHAR			m_Name[MAX_PATH];	// The name of button
	LPWSTR			m_iconLocation;		// icon location if supported
	CTlbContainer*	m_container;
	HICON			m_hIcon;
	int				m_loadedIconSize;
	CTxDIB*			m_pngIcon;
public:
	CTlbButton(void);
	virtual ~CTlbButton(void);

	virtual BOOL OnSetLCID(DWORD dwLCID, HMODULE hInstance); //Called to set language ID. Use hInstance to load strings, dialogs etc. If hInstance == NULL the use g_hInst
	virtual BOOL isMenu(); //Return TRUE if this plugin is menu
	virtual BOOL FirstChildData(CHILDS_DATA* data);
	virtual BOOL NextChildData(CHILDS_DATA* data);
	virtual BOOL FirstChildDataF(CHILDS_DATA* data, LPWSTR filter, HANDLE hStop);
	virtual BOOL NextChildDataF(CHILDS_DATA* data, LPWSTR filter, HANDLE hStop);
	virtual BOOL FreeChildData(CHILDS_DATA* data);
	virtual BOOL CreateChild(CREATE_CHILD_DATA* childData);
	virtual HANDLE GetChangesHandle();
	virtual void PrepareChanges(HANDLE hChanges);
	virtual void CloseChangesHandle(HANDLE hChanges);
	virtual UINT ReadChanges();
	virtual void GetChildUID(LPWSTR uid);
	virtual void CompareButtons(COMPARE_BUTTONS_DATA* cpBtns);
	virtual BOOL QueryArrangeMenu(UINT idx, ARRANGE_MENU_DATA* qamData);
	virtual BOOL QueryArrangeChildren(UINT idx, ARRANGE_MENU_DATA* qamData);
	virtual void GetSize(SIZE* sz, BOOL actual);
	virtual int  GetHeight(int width, int defHeight, HDC hDC);
	virtual UINT GetModeFlags();
	virtual BOOL OnProperties();
	virtual BOOL OnRename();
	virtual BOOL OnAbout();
	virtual BOOL OnDelete();
	virtual BOOL OnChangeHotkey();
	virtual void GetButtonName(LPWSTR name);
	virtual void OnDestroy();
	virtual BOOL SupportGlassMenu();
	virtual BOOL SupportSearchMenu();
	virtual BOOL isValidViewStyle(UINT viewStyle);
	virtual BOOL applyMargins();
	virtual void QueryVerbs(VERB::vector& verbs);
	virtual void RunVerb(LPCWSTR verb, int tip_idx);

	//Child window support
	virtual void OnShow();
	virtual void OnHide();
	virtual void CreateWnd(HWND hWndParent);

	virtual int GetTipsCount(void);
	virtual void GetTipRect(int tipID, LPRECT rcTip);
	virtual LPWSTR GetTipText(int tipID);
	virtual LPWSTR GetTipHTML(int tipID);
	virtual LPWSTR GetTipFullHTML(int tipID);
	virtual LPWSTR GetTipCaption(int tipID);
	virtual UINT GetSupportedActions();
	virtual void GetButtonText(LPWSTR text);
	virtual BOOL SupportSortMenu();
	virtual HICON GetTipIcon(int tipID);
	virtual void* GetTipImage(int tipID, LPCWSTR url, int& width, int& height, BOOL redraw_on_ready);
	virtual UINT GetTipFlags(int tipID);
	virtual BOOL OnTipClick(int tipID, LPCWSTR url);

	virtual BOOL Save(IStream *data);
	virtual BOOL Load(IStream *data);
	virtual void SetRect(LPRECT rcItem);
	virtual void SetButtonID(UINT btnID);
	virtual void OnDraw(HDC hDC, LPRECT rcItem);
	virtual BOOL OnMouseOver(long keys, int x, int y);
	virtual BOOL OnMouseLeave(long keys);
	virtual BOOL OnMouseEnter(long keys, int x, int y);
	virtual BOOL OnCreate();
	virtual BOOL OnStateChanged(int state, int* needRedraw);
	virtual BOOL OnLButtonDown(long key, int x, int y);
	virtual BOOL OnLButtonUp(long key, int x, int y);
	virtual BOOL OnLButtonDblClick(long key, int x, int y);
	virtual BOOL OnRButtonDown(long key, int x, int y);
	virtual BOOL OnRButtonUp(long key, int x, int y);
	virtual BOOL OnMButtonDown(long key, int x, int y);
	virtual BOOL OnMButtonUp(long key, int x, int y);
	virtual BOOL OnMouseWheel(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnRun();
	virtual int  QueryContextMenu(HMENU hMenu, int index, int cmdFirst, int cmdLast);
	virtual BOOL OnContextMenuCommand(int ID);
	virtual void GetIconLocation(LPWSTR szLocation);
	virtual BOOL SetIconLocation(LPCWSTR szLocation);
	virtual void SetName(LPCWSTR szName);
	virtual void SetContainer(CTlbContainer* container);
	
	virtual void OnHotkey(UINT hkID);
	virtual BOOL GetHotKeyData(int idx, HOTKEY_DATA* hkData);
	virtual int	 GetHotkeysCount();
	virtual UINT GetMenuFlags();

	virtual int GetTipsIconSize(int tipID);
	virtual void DrawTipsIcon(HDC hDC, POINT pt, int tipID);

	virtual void OnMeasureItem(WPARAM wParam, LPARAM lParam);
	virtual void OnDrawItem(WPARAM wParam, LPARAM lParam);

	virtual BOOL LoadIcon(int size);
	virtual BOOL LoadPngIcon();

	virtual DWORD AcceptDropObject(IDataObject* lpObj, DWORD dwKeys);
	virtual DWORD DropObject(IDataObject* lpObj, DWORD dwEffect);
	virtual void  OnBeginDrag(IDataObject* lpObj);
	virtual BOOL  okDragEffects(DWORD& dwEffects);
	virtual BOOL  delAfterDrop(BOOL& bDel, DWORD dwEffect);

	virtual BOOL OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnKeyUp(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnChar(WPARAM wParam, LPARAM lParam);
	virtual void OnSetFocus();
	virtual void OnKillFocus();
	virtual BOOL OnSysKeyDown(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnSysKeyUp(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnSysChar(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnSetCursor(WPARAM wParam, LPARAM lParam);


	void tlbInsertMenuStringItem(HMENU		hMenu, 
									UINT		ID, 
									UINT		index, 
									LPTSTR		str, 
									BOOL		isDisabled = FALSE,
									UINT		checkStyle = 0, 
									BOOL		checkState = 0, 
									HMENU		hSubMenu = NULL,
									HBITMAP		img = NULL);

	virtual BOOL QueryClass(CLSID classID, LPVOID* lpClass);
	virtual BOOL QueryBtnInterface(REFIID riid, LPVOID* lpInterface);
};

// Macros

// tlbInsertMenuStringItem:checkStyle
#define CHK_STYLE_NONE	0
#define CHK_STYLE_CHECK	1
#define CHK_STYLE_RADIO	2

//Return flags for ReadChanges
#define READ_CHANGES_RESORT		0x0100	//Ask for reorder items
#define READ_CHANGES_LOADICONS	0x0200	//Ask for loading icons
#define READ_CHANGES_RECALC		0x0300	//Ask for recalc menu/toolbar layout in case of size change
#define READ_CHANGES_REDRAW		0x0400	//Ask for redraw button

// GetStyle constants
#define CTS_LARGEICONS				0x1
#define CTS_ISMENU					0x2
#define CTS_SHOWARROWS				0x4
#define CTS_SHOWTEXT				0x8

//You must implement these functions
extern LPCLSID		GetCLSID();
extern LPSTR		GetPluginName();
extern void			InitDLL();
extern void			UninitDLL();
extern CTlbButton*	CreatePlugin();

// some other functions
extern BOOL			isVista();

extern HINSTANCE	g_hInst;
