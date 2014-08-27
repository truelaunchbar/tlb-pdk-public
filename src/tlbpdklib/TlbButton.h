#pragma once

// Interface to query container of plugin
interface CTlbContainer
{
	virtual void		DrawItemText(HDC hDC, LPRECT rcText, LPCSTR text) = 0;
	virtual HWND		GetToolbarWindow() = 0;
	virtual void		GetIconPoint(LPPOINT ptIcon) = 0;
	virtual int			GetIconSize() = 0;
	virtual DWORD		GetViewMode() = 0;
	virtual HFONT		GetItemFont() = 0;
	virtual COLORREF	GetTextColor() = 0;
	virtual void		GetTextMargins(LPRECT rcMargins) = 0;
	virtual void		SetHotKey(DWORD hk, UINT hkID) = 0;
	virtual BOOL		IsVisible() = 0;
	virtual void		Hide() = 0;
	virtual void		Show() = 0;
	virtual void		QueryCreateWindow() = 0;
	virtual void		OpenParentMenu() = 0;
	virtual void		OnKillFocus() = 0;
	virtual void		OnRButtonUp() = 0;
	virtual void		GetSavedPath(UINT objID, LPWSTR path) = 0;
	virtual void		SetState(UINT state) = 0;
	virtual void		SaveButton() = 0;
	virtual void		UpdateSize() = 0;
	virtual void		Redraw() = 0;
	virtual void		UpdateTips() = 0;
};

// Plugin button class
class CTlbButton
{
protected:
	RECT			m_rc;				// Rectangle of button
	DWORD			m_ID;				// ID of button
	HINSTANCE		m_hResModule;		// Resource module
	char			m_Name[MAX_PATH];	// The name of button
	char*			m_iconLocation;		// icon location if supported
	CTlbContainer*	m_container;
public:
	CTlbButton(void);
	virtual ~CTlbButton(void);

	virtual BOOL OnSetLCID(DWORD dwLCID, HMODULE hInstance); //Called to set language ID. Use hInstance to load strings, dialogs etc. If hInstance == NULL the use g_hInst
	virtual BOOL isMenu(); //Return TRUE if this plugin is menu
	virtual BOOL FirstChildData(ENUM_CHILDS* data);
	virtual BOOL NextChildData(ENUM_CHILDS* data);
	virtual BOOL FreeChildData(ENUM_CHILDS* data);
	virtual BOOL CreateChild(CREATE_CHILDS* childData);
	virtual HANDLE GetChangesHandle();
	virtual void PrepareChanges(HANDLE hChanges);
	virtual void CloseChangesHandle(HANDLE hChanges);
	virtual UINT ReadChanges();
	virtual void GetChildUID(LPWSTR uid);
	virtual void CompareButtons(COMPARE_BUTTONS* cpBtns);
	virtual BOOL QueryArrangeMenu(UINT idx, QUERY_ARRANGE_MENU* qamData);
	virtual void GetSize(SIZE* sz, BOOL actual);
	virtual UINT GetModeFlags();
	virtual HRESULT OnProperties();
	virtual HRESULT OnRename();
	virtual HRESULT OnAbout();
	virtual HRESULT OnDelete();
	virtual void GetButtonName(LPWSTR name);

	//Child window support
	virtual void OnShow();
	virtual void OnHide();
	virtual void CreateWnd(HWND hWndParent);

	virtual int GetTipsCount(void);
	virtual void GetTipRect(int tipID, LPRECT rcTip);
	virtual LPWSTR GetTipText(int tipID);
	virtual LPWSTR GetTipCaption(int tipID);
	virtual UINT GetSupportedActions();
	virtual void GetButtonText(char* text);
	virtual BOOL SupportSortMenu();
	virtual HICON GetTipIcon(int tipID);

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
	virtual BOOL OnRun();
	virtual int  QueryContextMenu(HMENU hMenu, int index, int cmdFirst, int cmdLast);
	virtual BOOL OnContextMenuCommand(int ID);
	virtual void GetIconLocation(LPSTR szLocation);
	virtual BOOL SetIconLocation(LPCSTR szLocation);
	virtual void SetName(LPCSTR szName);
	virtual void SetContainer(CTlbContainer* container);
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

// Cosnants for GetSupportedActions
#define CTMS_PROPERTIES	0x01		// Plugin support Properties
#define CTMS_RENAME		0x02		// Plugin support Rename
#define CTMS_CHANGEICON	0x04		// Plugin support "Change Icon"
#define CTMS_HOVERRUN	0x20		// Plugin support "Run on mouse hover" feature
#define CTMS_FINDTARGET	0x80		// Plugin support "Find Target" feature
#define CTMS_OPENFOLDER	0x100		// Plugin support "Open Folder" feature
#define CTMS_NODELETE	0x200		
#define CTMS_RESETICON	0x800		// Plugin support "Reset Icon" feature
#define CTMS_SETIMAGE	0x1000		// Plugin support "Set Button Image"
#define CTMS_ABOUT		0x2000		// Plugin support "About"
#define CTMS_DELETE		0x4000		// Plugin support "Delete"

//Button states
#define BTN_S_PUSHED	0x01
#define BTN_S_SELECTED	0x02
#define BTN_S_INMENU	0x04
#define BTN_S_INACTION	0x08

//You must implement these functions
extern LPCLSID		GetCLSID();
extern LPSTR		GetPluginName();
extern void			InitDLL();
extern void			UninitDLL();
extern CTlbButton*	CreatePlugin();
