#include "tlbpdkcore.h"
#include "tlbpdklib.h"
#include <txdib.h>
#include <shlwapi.h>
#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>
#include <CommCtrl.h>

extern HINSTANCE   g_hResInst;
extern HINSTANCE   g_hInst;

CTlbButton::CTlbButton(void)
{
	m_menuBitmaps	 = NULL;
	m_bmpCount		 = NULL;
	m_pngIcon		 = NULL;
	m_hIcon			 = NULL;
	m_loadedIconSize = 0;
	m_ID			 = 0;
	m_iconLocation	 = NULL;
	m_container		 = NULL;
	m_hResModule	 = NULL;
	m_Name[0]		 = 0;
	m_container		 = NULL;
	g_DllRefCount++;
}

CTlbButton::~CTlbButton(void)
{
	ClearBitmaps();
	if(m_hIcon) DestroyIcon(m_hIcon);
	if(m_pngIcon) delete m_pngIcon;
	FREE_CLEAR_STR(m_iconLocation);
	g_DllRefCount--;
}

void CTlbButton::AddBitmap(HBITMAP bmp)
{
	if(!bmp) return;
	if(!m_bmpCount)
	{
		m_menuBitmaps = (HBITMAP*) malloc(sizeof(HBITMAP));
	} else
	{
		m_menuBitmaps = (HBITMAP*) realloc(m_menuBitmaps, sizeof(HBITMAP) * (m_bmpCount + 1));
	}
	m_menuBitmaps[m_bmpCount] = bmp;
	m_bmpCount++;
}

void CTlbButton::ClearBitmaps()
{
	for(int i=0; i < m_bmpCount; i++)
	{
		DeleteObject(m_menuBitmaps[i]);
	}
	if(m_menuBitmaps)
	{
		free(m_menuBitmaps);
	}
	m_menuBitmaps	= NULL;
	m_bmpCount		= 0;
}


int CTlbButton::GetTipsCount(void)
{
	return 1;
}

void CTlbButton::GetTipRect(int tipID, LPRECT rcTip)
{
	*rcTip = m_rc;
}

LPWSTR CTlbButton::GetTipText(int tipID)
{
	LPWSTR name = (LPWSTR) CoTaskMemAlloc((lstrlen(m_Name) + 1) * sizeof(WCHAR));
	lstrcpy(name, m_Name);
	return name;
}

LPWSTR CTlbButton::GetTipHTML(int tipID)
{
	return NULL;
}

void CTlbButton::SetRect(LPRECT rcItem)
{
	m_rc = rcItem[0];
}

void CTlbButton::GetIconLocation(LPWSTR szLocation)
{
	if(m_iconLocation && szLocation)
	{
		StringCchCopy(szLocation, MAX_PATH, m_iconLocation);
	}
}

BOOL CTlbButton::SetIconLocation(LPCWSTR szLocation)
{
	if(m_hIcon)
	{
		DestroyIcon(m_hIcon);
		m_hIcon = NULL;
	}
	if(m_pngIcon)
	{
		delete m_pngIcon;
		m_pngIcon = NULL;
	}
	MAKE_STR(m_iconLocation, szLocation);
	if(m_container)
	{
		m_container->SaveButton();
		m_container->Redraw();
	}
	return TRUE;
}

void CTlbButton::SetName(LPCWSTR szName)
{
	StringCchCopy(m_Name, MAX_PATH,szName);
}

void CTlbButton::SetContainer(CTlbContainer* container)
{
	m_container = container;

	if(m_container)
	{
		WCHAR tlb_path[MAX_PATH];
		m_container->GetFolder(tlb_path, FOLDER_TYPE_TLBDLL);

		WCHAR dll_path[MAX_PATH];
		StringCchCopy(dll_path, MAX_PATH, tlb_path);
		StringCchCat(dll_path, MAX_PATH, L"cairo.dll");
		LoadLibrary(dll_path);

		StringCchCopy(dll_path, MAX_PATH, tlb_path);
		StringCchCat(dll_path, MAX_PATH, L"freeimage.dll");
		LoadLibrary(dll_path);
	}
}

BOOL CTlbButton::OnSetLCID(DWORD dwLCID, HMODULE hInstance)
{
	if(!hInstance)
	{
		m_hResModule = g_hInst;
	} else
	{
		m_hResModule = hInstance;
	}
	return TRUE;
}

 BOOL CTlbButton::isMenu() { return FALSE; }
 BOOL CTlbButton::FirstChildData(CHILDS_DATA* data) { return FALSE; }
 BOOL CTlbButton::NextChildData(CHILDS_DATA* data) { return FALSE; }
 BOOL CTlbButton::FirstChildDataF(CHILDS_DATA* data, LPWSTR filter, HANDLE hStop) { return FALSE; }
 BOOL CTlbButton::NextChildDataF(CHILDS_DATA* data, LPWSTR filter, HANDLE hStop) { return FALSE; }
 BOOL CTlbButton::FreeChildData(CHILDS_DATA* data) { return FALSE; }
 BOOL CTlbButton::CreateChild(CREATE_CHILD_DATA* childData) { return FALSE; }
 HANDLE CTlbButton::GetChangesHandle() { return NULL; }
 void CTlbButton::PrepareChanges(HANDLE hChanges) {}
 void CTlbButton::CloseChangesHandle(HANDLE hChanges) {}
 UINT CTlbButton::ReadChanges() { return 0; }
 void CTlbButton::GetChildUID(LPWSTR uid) { }
 void CTlbButton::CompareButtons(COMPARE_BUTTONS_DATA* cpBtns) {}
 BOOL CTlbButton::QueryArrangeMenu(UINT idx, ARRANGE_MENU_DATA* qamData) { return FALSE; }
 BOOL CTlbButton::QueryArrangeChildren(UINT idx, ARRANGE_MENU_DATA* qamData) { return FALSE; }
 void CTlbButton::GetSize(SIZE* sz, BOOL actual) {}
 int  CTlbButton::GetHeight( int width, int defHeight, HDC hDC ) { return defHeight; }
 UINT CTlbButton::GetModeFlags() { return 0; }
 BOOL CTlbButton::OnProperties() { return FALSE; }
 BOOL CTlbButton::OnRename() { return FALSE; }
 BOOL CTlbButton::OnAbout() { return FALSE; }
 BOOL CTlbButton::OnDelete() { return FALSE; }
 BOOL CTlbButton::OnChangeHotkey() { return FALSE; }
 void CTlbButton::GetButtonName(LPWSTR name) 
 { 
	 lstrcpy(name, m_Name);
 }

void CTlbButton::OnShow() {}
void CTlbButton::OnHide() {}
void CTlbButton::CreateWnd(HWND hWndParent) {}

LPWSTR CTlbButton::GetTipCaption(int tipID) { return 0; }
UINT CTlbButton::GetSupportedActions() { return 0; }
void CTlbButton::GetButtonText(LPWSTR text) 
{
	lstrcpy(text, m_Name);
}
BOOL CTlbButton::SupportSortMenu() { return FALSE; }
HICON CTlbButton::GetTipIcon(int tipID) { return NULL; }

void* CTlbButton::GetTipImage( int tipID, LPCWSTR url, int& width, int& height, BOOL redraw_on_ready )
{
	width	= 0;
	height	= 0;
	return NULL;
}


BOOL CTlbButton::Save(IStream *data) { return TRUE; }
BOOL CTlbButton::Load(IStream *data) {return TRUE; }
void CTlbButton::SetButtonID(UINT btnID) { m_ID = btnID; }

void CTlbButton::OnDraw(HDC hDC, LPRECT rcItem)
{
	int iconLeft = 0;
	int iconTop = 0;
	DWORD btnType = 0;
	if(m_container)
	{
		POINT pt;
		m_container->GetIconPoint(&pt);
		iconLeft = rcItem->left + pt.x;
		iconTop = rcItem->top + pt.y;
		btnType = m_container->GetButtonType();
	}

	if(!btnType || !(btnType & TS_ITEM_NOICON))
	{
		int szIcon = m_container->GetIconSize();
		if(LoadPngIcon())
		{
			m_pngIcon->draw(hDC, iconLeft, iconTop, szIcon, szIcon);
		} else
		{
			if(LoadIcon(szIcon))
			{
				DrawIconEx((HDC) hDC, iconLeft, iconTop, m_hIcon, szIcon, szIcon, 0, 0, DI_NORMAL);
			}
		}
	}
}

BOOL CTlbButton::OnMouseOver(long keys, int x, int y) 
{ 
	return FALSE; 
}

BOOL CTlbButton::OnMouseLeave(long keys) 
{ 
	return FALSE; 
}

BOOL CTlbButton::OnMouseEnter(long keys, int x, int y) 
{ 
	return FALSE; 
}

BOOL CTlbButton::OnCreate() 
{ 
	return TRUE; 
}

BOOL CTlbButton::OnStateChanged(int state, int* needRedraw) 
{ 
	return FALSE; 
}

BOOL CTlbButton::OnLButtonDown(long key, int x, int y) 
{ 
	return FALSE; 
}

BOOL CTlbButton::OnLButtonDblClick(long key, int x, int y)
{
	return FALSE;
}

BOOL CTlbButton::OnRButtonDown(long key, int x, int y)
{
	return FALSE;
}

BOOL CTlbButton::OnRButtonUp(long key, int x, int y)
{
	return FALSE;
}


BOOL CTlbButton::OnMButtonDown( long key, int x, int y )
{
	return FALSE;
}

BOOL CTlbButton::OnMButtonUp( long key, int x, int y )
{
	return FALSE;
}

BOOL CTlbButton::OnLButtonUp(long key, int x, int y) 
{ 
	return FALSE; 
}

BOOL CTlbButton::OnRun() 
{ 
	return FALSE; 
}

int  CTlbButton::QueryContextMenu(HMENU hMenu, int index, int cmdFirst, int cmdLast) 
{ 
	ClearBitmaps();
	return 0; 
}

BOOL CTlbButton::OnContextMenuCommand(int ID) 
{ 
	return FALSE; 
}

BOOL CTlbButton::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

DWORD CTlbButton::AcceptDropObject(IDataObject* lpObj, DWORD dwKeys)
{
	return DROPEFFECT_NONE;
}

DWORD CTlbButton::DropObject(IDataObject* lpObj, DWORD dwEffect)
{
	return DROPEFFECT_NONE;
}

void CTlbButton::OnBeginDrag(IDataObject* lpObj)
{
}

BOOL CTlbButton::okDragEffects(DWORD& dwEffects)
{
	return FALSE;
}

BOOL CTlbButton::delAfterDrop(BOOL& bDel, DWORD dwEffect)
{
	return FALSE;
}

BOOL CTlbButton::QueryClass(CLSID classID, LPVOID* lpClass)
{
	return FALSE;
}

BOOL CTlbButton::QueryBtnInterface(REFIID riid, LPVOID* lpInterface)
{
	return FALSE;
}


void CTlbButton::OnHotkey(UINT hkID)
{
}

BOOL CTlbButton::GetHotKeyData(int idx, HOTKEY_DATA* hkData)
{
	return FALSE;
}

int	 CTlbButton::GetHotkeysCount()
{
	return 1;
}

UINT CTlbButton::GetMenuFlags()
{
	return 0;
}

int CTlbButton::GetTipsIconSize(int tipID)
{
	return 32;
}

void CTlbButton::DrawTipsIcon(HDC hDC, POINT pt, int tipID)
{
}

void CTlbButton::OnMeasureItem(WPARAM wParam, LPARAM lParam)
{
	LPMEASUREITEMSTRUCT itemData = (LPMEASUREITEMSTRUCT) lParam;
	itemData->itemHeight = 20;
	itemData->itemWidth =  20;
}

void CTlbButton::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	LPDRAWITEMSTRUCT itemData = (LPDRAWITEMSTRUCT) lParam;
	CTxDIB* img = (CTxDIB*) itemData->itemData;
	if(img)
	{
		img->draw(itemData->hDC, itemData->rcItem.left, itemData->rcItem.top + 2);
	}
}

void CTlbButton::tlbInsertMenuStringItem(HMENU		hMenu, 
											UINT		ID, 
											UINT		index, 
											LPTSTR		str, 
											BOOL		isDisabled,
											UINT		checkStyle, 
											BOOL		checkState, 
											HMENU		hSubMenu,
											HBITMAP		img)
{
	MENUITEMINFO mi;
	ZeroMemory(&mi, sizeof(mi));
	mi.cbSize = sizeof(MENUITEMINFO);

	if(img && m_container->tboShowCtxIcons())
	{
		mi.fMask = MIIM_STRING | MIIM_ID;
		mi.dwTypeData = str;
		mi.cch = lstrlen(str);
		mi.wID = ID;

		if(isVista())
		{
			mi.fMask |= MIIM_BITMAP;
			mi.hbmpItem = img;
			AddBitmap(mi.hbmpItem);
		}
	} else
	{
		mi.fMask = MIIM_TYPE | MIIM_ID;
		mi.fType = MFT_STRING;
		mi.dwTypeData = str;
		mi.cch = lstrlen(str);
		mi.wID = ID;
	}
	switch(checkStyle)
	{
	case CHK_STYLE_CHECK:
		mi.fMask |= MIIM_STATE;
		if(checkState)
		{
			mi.fState = MFS_CHECKED;
		} else
		{
			mi.fState = MFS_UNCHECKED;
		}
		break;
	case CHK_STYLE_RADIO:
		mi.fMask |= MIIM_STATE;
		mi.fType |= MFT_RADIOCHECK;
		if(checkState)
		{
			mi.fState = MFS_CHECKED;
		} else
		{
			mi.fState = MFS_UNCHECKED;
		}
		break;
	}
	if(hSubMenu)
	{
		mi.fMask |= MIIM_SUBMENU;
		mi.hSubMenu = hSubMenu;
	}
	if(isDisabled)
	{
		mi.fMask |= MIIM_STATE;
		mi.fState = MFS_DISABLED;
	}
	InsertMenuItem(hMenu, index, TRUE, &mi);
}

BOOL CTlbButton::LoadIcon(int size)
{
	if(m_hIcon && size == m_loadedIconSize)
	{
		return TRUE;
	}
	if(m_hIcon) DestroyIcon(m_hIcon);
	m_hIcon = NULL;
	if(m_iconLocation && m_iconLocation[0])
	{
		TCHAR il[MAX_PATH];
		lstrcpy(il, m_iconLocation);
		int idx = PathParseIconLocation(il);
		if(PathIsRelative(il))
		{
			if(m_container)
			{
				m_container->GetSavedPath(OT_ICONS, il);
				PathAddBackslash(il);
				lstrcat(il, m_iconLocation);
			}
		}
		m_hIcon = ExtractIconFromFile(il, idx, size);
	}
	if(m_hIcon) 
	{
		m_loadedIconSize = size;
	}
	return m_hIcon ? TRUE : FALSE;
}


BOOL CTlbButton::LoadPngIcon(void)
{
	if(m_pngIcon) return TRUE;
	if(m_iconLocation && m_iconLocation[0])
	{
		LPWSTR ext = PathFindExtension(m_iconLocation);
		if(ext && !StrCmpI(ext, TEXT(".png")))
		{
			TCHAR il[MAX_PATH];
			lstrcpy(il, m_iconLocation);
			if(PathIsRelative(il))
			{
				if(m_container)
				{
					m_container->GetSavedPath(OT_ICONS, il);
					PathAddBackslash(il);
					lstrcat(il, m_iconLocation);
				}
			}
			m_pngIcon = new CTxDIB;
			m_pngIcon->load(il);
			if(!m_pngIcon->isValid())
			{
				delete m_pngIcon;
				m_pngIcon = NULL;
			}
		}
	}
	if(m_pngIcon) return TRUE;
	return FALSE;
}

void CTlbButton::OnDestroy()
{

}

BOOL CTlbButton::SupportGlassMenu()
{
	return FALSE;
}


BOOL CTlbButton::SupportSearchMenu()
{
	return FALSE;
}

BOOL CTlbButton::isValidViewStyle( UINT viewStyle )
{
	return TRUE;
}


BOOL CTlbButton::applyMargins()
{
	return TRUE;
}

BOOL CTlbButton::OnKeyDown( WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

BOOL CTlbButton::OnKeyUp( WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

BOOL CTlbButton::OnChar( WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

void CTlbButton::OnSetFocus()
{

}

void CTlbButton::OnKillFocus()
{

}

BOOL CTlbButton::OnSysKeyDown( WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

BOOL CTlbButton::OnSysKeyUp( WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}

BOOL CTlbButton::OnSysChar( WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}


BOOL CTlbButton::OnSetCursor( WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}
