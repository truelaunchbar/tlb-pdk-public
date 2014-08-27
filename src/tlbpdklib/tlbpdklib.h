#pragma once

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

	//Child window support
	virtual void OnShow();
	virtual void OnHide();
	virtual void CreateWnd(HWND hWndParent);

	virtual int GetTipsCount(void);
	virtual void GetTipRect(int tipID, LPRECT rcTip);
	virtual LPWSTR GetTipText(int tipID);
	virtual LPWSTR GetTipHTML(int tipID);
	virtual LPWSTR GetTipCaption(int tipID);
	virtual UINT GetSupportedActions();
	virtual void GetButtonText(LPWSTR text);
	virtual BOOL SupportSortMenu();
	virtual HICON GetTipIcon(int tipID);
	virtual void* GetTipImage(int tipID, LPCWSTR url, int& width, int& height, BOOL redraw_on_ready);

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

//Hotkeys modifiers
#define HOTKEY_MOD_SHIFT	0x0100
#define HOTKEY_MOD_CONTROL	0x0200
#define HOTKEY_MOD_ALT		0x0400
#define HOTKEY_MOD_WIN		0x2000

// Constants for parameter objID GetSavedPath 
#define OT_ICONS			0x00	// saved icons
#define OT_PLUGINFILES		0x01	// saved plugin files

// Constants for GetModeFlags
#define BTN_FLAG_PLANE				0x00000001	//button is flat (plane) useful for informational plugins
#define BTN_FLAG_STOPDRAG			0x00000002	//disable drag&drop
#define BTN_FLAG_NOTIPS				0x00000008	//disable tips on plugin button
#define BTN_FLAG_SUPPORTSAVEDICON	0x00000010	//support saved icons
#define BTN_FLAG_SUPPORTPNGICONS	0x00000020	//support PNG format for icons
#define BTN_FLAG_INPLACERENAME		0x00000040
#define BTN_FLAG_NOTEXT				0x00000100
#define BTN_FLAG_NEED_R_CLICKS		0x00100000
#define BTN_FLAG_NEED_L_DBLCLICK	0x00200000
#define BTN_FLAG_DONTACTIVATE		0x00400000
#define BTN_FLAG_SEPARATOR			0x00800000
#define BTN_FLAG_NORUNSTRACK		0x01000000

// Cosnants for GetSupportedActions
#ifndef CTMS_PROPERTIES
#define CTMS_PROPERTIES	0x0001		// Plugin support Properties
#define CTMS_RENAME		0x0002		// Plugin support Rename
#define CTMS_CHANGEICON	0x0004		// Plugin support "Change Icon"
#define CTMS_HOVERRUN	0x0020		// Plugin support "Run on mouse hover" feature
#define CTMS_HOTKEY		0x40		// change the hotkey
#define CTMS_FINDTARGET	0x0080		// Plugin support "Find Target" feature
#define CTMS_OPENFOLDER	0x0100		// Plugin support "Open Folder" feature
#define CTMS_NODELETE	0x0200		
#define CTMS_RESETICON	0x0800		// Plugin support "Reset Icon" feature
#define CTMS_SETIMAGE	0x1000		// Plugin support "Set Button Image"
#define CTMS_ABOUT		0x2000		// Plugin support "About"
#define CTMS_DELETE		0x4000		// Plugin support "Delete"*/
#endif

// tlbInsertMenuStringItem:checkStyle
#define CHK_STYLE_NONE	0
#define CHK_STYLE_CHECK	1
#define CHK_STYLE_RADIO	2

//Button states
#define BTN_S_PUSHED	0x01
#define BTN_S_SELECTED	0x02
#define BTN_S_INMENU	0x04
#define BTN_S_INACTION	0x08

//Return flags for ReadChanges
#define READ_CHANGES_RESORT		0x0100	//Ask for reorder items
#define READ_CHANGES_LOADICONS	0x0200	//Ask for loading icons
#define READ_CHANGES_RECALC		0x0300	//Ask for recalc menu/toolbar layout in case of size change
#define READ_CHANGES_REDRAW		0x0400	//Ask for redraw button

//for TBTN_INVOKE_GETMENUFLAGS
#define MENU_F_NORENAME				0x00001
#define MENU_F_NOAUTOSHOW			0x00002
#define MENU_F_NOTSHOWONCLICK		0x00004
#define MENU_F_RUNONCLICK			0x00008
#define MENU_F_SUPPORT_DELAYED		0x00010
#define MENU_F_CHILDRENSORT			0x00020
#define MENU_F_VARHEIGHT			0x00040
#define MENU_F_DEFAULTWRAP			0x00080

//for TBTN_INVOKE_GETBUTTONTYPE
#define TS_ITEM_SHOWTEXT			0x000800
#define TS_ITEM_ALWAYSBTN			0x001000
#define TS_ITEM_EXPANDMARK			0x002000
#define TS_ITEM_RUNMARK				0x004000
#define TS_ITEM_ICONONLY			0x008000
#define TS_ITEM_ONTOOLBAR			0x000100
#define TS_ITEM_INMENU				0x010000
#define TS_ITEM_NOICON				0x020000
#define TS_ITEM_NOTEXT				0x040000
#define TS_ITEM_TEAROFF				0x080000

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

// flags for CTlbContainer::drawText
#define	DTF_DRAWGLOW			0x01	// always draw glow
#define	DTF_NOGLOW				0x02	// don't draw glow
#define	DTF_GLOWCOLOR			0x04	// glowColor is valid
#define	DTF_COLOR				0x08	// color is valid
#define	DTF_GLOWSIZE			0x10	// glow size is valid
#define	DTF_FORCETRANSPARENTDC	0x20
#define	DTF_SHADOW				0x40

// folder type for CONTAINER_CMD_GETFOLDER
#define FOLDER_TYPE_TLBDLL			0
#define FOLDER_TYPE_USERPROFILE		1
#define FOLDER_TYPE_COMMONPROFILE	2

extern HINSTANCE	g_hInst;
