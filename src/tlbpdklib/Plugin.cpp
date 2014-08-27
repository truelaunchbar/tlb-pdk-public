#include "tlbpdkcore.h"
#include ".\plugin.h"
#include "tlbpdklib.h"
#include <WindowsX.h>
#include "tlbtext.h"
#include <ShlObj.h>
#include <Shlwapi.h>
#include <strsafe.h>

extern HINSTANCE   g_hResInst;
extern HINSTANCE   g_hInst;

CPlugin::CPlugin(CTlbButton* btn)
{
	m_ObjRefCount = 1;
	if(!btn)
	{
		m_btn = CreatePlugin();
	} else
	{
		m_btn = btn;
	}
	g_DllRefCount++;
}

CPlugin::~CPlugin(void)
{
	delete m_btn;
	g_DllRefCount--;
}

//////////////////////////////////////////////////////////////////////
// IUnknown methods
//////////////////////////////////////////////////////////////////////

STDMETHODIMP CPlugin::QueryInterface(REFIID riid, LPVOID *ppReturn)
{
	*ppReturn = NULL;

	//IUnknown
	if(IsEqualIID(riid, IID_ITlbButton))
	{
		*ppReturn = (ITlbButton*) this;
	}
	if(IsEqualIID(riid, IID_IUnknown))
	{
		*ppReturn = (IUnknown*) ((ITlbButton*) this);
	}
	if(!(*ppReturn) && m_btn)
	{
		m_btn->QueryBtnInterface(riid, ppReturn);
	}

	if(*ppReturn)
	{
		(*(LPUNKNOWN*)ppReturn)->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

STDMETHODIMP_(DWORD) CPlugin::AddRef()
{
	return ++m_ObjRefCount;
}

STDMETHODIMP_(DWORD) CPlugin::Release()
{
	if(--m_ObjRefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_ObjRefCount;
}

//////////////////////////////////////////////////////////////////////
// ITlbButton methods
//////////////////////////////////////////////////////////////////////

// Load parameters for button
STDMETHODIMP CPlugin::Load(IStream *data)
{
	if(m_btn->Load(data))
	{
		return S_OK;
	}
	return S_FALSE;
}

// Save button parameters
STDMETHODIMP CPlugin::Save(IStream *data)
{
	if(m_btn->Save(data))
	{
		return S_OK;
	}
	return S_FALSE;
}

// Sets button rectangle
STDMETHODIMP CPlugin::SetRect(RECT *rcItem)
{
	m_btn->SetRect(rcItem);
	return S_OK;
}

// Set button container
// NOTE: do not call AddRef to store container pointer
STDMETHODIMP CPlugin::SetContainer(ITlbContainer *cont)
{
	m_container = cont;
	m_btn->SetContainer(this);
	return S_OK;
}

STDMETHODIMP CPlugin::SetID(int btnID)
{
	m_btn->SetButtonID(btnID);
	return S_OK;
}

STDMETHODIMP CPlugin::Draw(HANDLE hDC, RECT *rcItem)
{
	m_btn->OnDraw((HDC) hDC, rcItem);
	return S_OK;
}

STDMETHODIMP CPlugin::OnMouseOver(long keys, int x, int y)
{
	if(m_btn->OnMouseOver(keys, x, y))
	{
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CPlugin::OnMouseLeave(long keys)
{
	if(m_btn->OnMouseLeave(keys))
	{
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CPlugin::OnMouseEnter(long keys, int x, int y)
{
	if(m_btn->OnMouseEnter(keys, x, y))
	{
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CPlugin::Create(IStream* data)
{
	if(m_btn->OnCreate())
	{
		return Save(data);
	}
	return S_FALSE;
}


STDMETHODIMP CPlugin::StateChanged(int state, int* needRedraw)
{
	if(m_btn->OnStateChanged(state, needRedraw))
	{
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CPlugin::GetDescription(char* szDescription)
{
	return S_FALSE;
}

STDMETHODIMP CPlugin::OnLButtonDown(long key, int x, int y)
{
	if(m_btn->OnLButtonDown(key, x, y))
	{
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CPlugin::OnLButtonUp(long key, int x, int y)
{
	if(m_btn->OnLButtonUp(key, x, y))
	{
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CPlugin::Run()
{
	if(m_btn->OnRun())
	{
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CPlugin::DoAction(int action)
{
	BOOL res = FALSE;
	switch(action)
	{
	case CTMS_PROPERTIES:
		res = m_btn->OnProperties();
		break;
	case CTMS_RENAME:
		res = m_btn->OnRename();
		break;
	case CTMS_ABOUT:
		res = m_btn->OnAbout();
		break;
	case CTMS_DELETE:
		res = m_btn->OnDelete();
		break;
	case CTMS_HOTKEY:
		res = m_btn->OnChangeHotkey();
		break;
	}
	if(!res)
	{
		return S_FALSE;
	}
	return S_OK;
}

STDMETHODIMP_(int) CPlugin::QueryContextMenu(HMENU hMenu, int index, int cmdFirst, int cmdLast)
{
	return m_btn->QueryContextMenu(hMenu, index, cmdFirst, cmdLast);
}

STDMETHODIMP CPlugin::InvokeCommand(int ID)
{
	if(m_btn->OnContextMenuCommand(ID))
	{
		return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CPlugin::SetLCID(DWORD dwLCID, HMODULE hResModule)
{
	m_btn->OnSetLCID(dwLCID, hResModule);
	return S_OK;
}

STDMETHODIMP CPlugin::GetIconLocation(LPSTR szLocation)
{
	m_btn->GetIconLocation((LPWSTR) szLocation);
	return S_OK;
}

STDMETHODIMP CPlugin::SetIconLocation(LPCSTR szLocation)
{
	if(m_btn->SetIconLocation((LPCWSTR) szLocation))
	{
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CPlugin::SetName(LPCSTR szName)
{
	m_btn->SetName((LPCWSTR) szName);
	return S_OK;
}

STDMETHODIMP CPlugin::GetButtonInfo(BUTTONINFO* btninfo)
{
	if(btninfo->dwMask & BTNINFO_MASK_ACTIONS)
	{
		btninfo->dwActions = m_btn->GetSupportedActions();
	}
	if(btninfo->dwMask & BTNINFO_MASK_MODEFLAG)
	{
		btninfo->dwModeFlags = m_btn->GetModeFlags() | BTN_FLAG_UNICODE;
	}
	if(btninfo->dwMask & BTNINFO_MASK_SIZE)
	{
		m_btn->GetSize(&btninfo->szBtnSize, FALSE);
	}
	if(btninfo->dwMask & BTNINFO_MASK_ACTUALSIZE)
	{
		m_btn->GetSize(&btninfo->szBtnSize, TRUE);
	}
	if(btninfo->dwMask & BTNINFO_MASK_TEXT)
	{
		m_btn->GetButtonText((LPWSTR) btninfo->strData);
	}
	return S_OK;
}

STDMETHODIMP CPlugin::Invoke(DWORD id, WPARAM wParam, LPARAM lParam)
{
	switch(id)
	{
	case TBTN_INVOKE_GETHEIGHT:
		{
			GET_HEIGHT* gh = (GET_HEIGHT*) lParam;
			gh->outHeight = m_btn->GetHeight(gh->width, gh->defHeight, gh->hdc);
		}
		return S_OK;
	case TBTN_INVOKE_ONKEYDOWN:
		if(m_btn->OnKeyDown(wParam, lParam))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_ONSETCURSOR:
		if(m_btn->OnSetCursor(wParam, lParam))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_ONKEYUP:
		if(m_btn->OnKeyUp(wParam, lParam))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_ONCHAR:
		if(m_btn->OnChar(wParam, lParam))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_ONSETFOCUS:
		m_btn->OnSetFocus();
		return S_OK;
	case TBTN_INVOKE_ONKILLFOCUS:
		m_btn->OnKillFocus();
		return S_OK;
	case TBTN_INVOKE_ONSYSKEYDOWN:
		if(m_btn->OnSysKeyDown(wParam, lParam))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_ONSYSKEYUP:
		if(m_btn->OnSysKeyUp(wParam, lParam))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_ONSYSCHAR:
		if(m_btn->OnSysChar(wParam, lParam))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_APPLYMARGINS:
		{
			LPBOOL ret = (LPBOOL) lParam;
			*ret = m_btn->applyMargins();
		}
		return S_OK;
	case TBTN_INVOKE_VALIDVIEWSTYLE:
		{
			LPBOOL ret = (LPBOOL) lParam;
			*ret = m_btn->isValidViewStyle((UINT) wParam);
		}
		return S_OK;
	case TBTN_INVOKE_SUPPORTSEARCH:
		if(m_btn->SupportSearchMenu())
		{
			LPBOOL bSearch = (LPBOOL) lParam;
			*bSearch = TRUE;
		} else
		{
			LPBOOL bSearch = (LPBOOL) lParam;
			*bSearch = FALSE;
		}
		return S_OK;
	case TBTN_INVOKE_F_FIRSTCHILDDATA:
		{
			ENUM_CHILDS_F* data = (ENUM_CHILDS_F*) lParam;
			CHILDS_DATA d;
			d.childData		= data->childData;
			d.enumData		= data->enumData;
			d.Name			= (LPWSTR) data->Name;
			if(m_btn->FirstChildDataF(&d, data->filter, data->hStopEvent))
			{
				data->childData		= d.childData;
				data->enumData		= d.enumData;
				data->Name			= (LPTSTR) d.Name;
				return S_OK;
			}
		}
		break;
	case TBTN_INVOKE_F_NEXTCHILDDATA:
		{
			ENUM_CHILDS_F* data = (ENUM_CHILDS_F*) lParam;
			CHILDS_DATA d;
			d.childData		= data->childData;
			d.enumData		= data->enumData;
			d.Name			= (LPWSTR) data->Name;
			if(m_btn->NextChildDataF(&d, data->filter, data->hStopEvent))
			{
				data->childData		= d.childData;
				data->enumData		= d.enumData;
				data->Name			= (LPTSTR) d.Name;
				return S_OK;
			} else
			{
				data->enumData		= NULL;
			}
		}
		break;
	case TBTN_INVOKE_OKDRAGEFFECTS:
		{
			DWORD dwEffects = *((LPDWORD) lParam);
			if(m_btn->okDragEffects(dwEffects))
			{
				*((LPDWORD) lParam) = dwEffects;
			}
		}
		return S_OK;
	case TBTN_INVOKE_DELAFTERDROP:
		{
			BOOL bDel = *((LPBOOL) lParam);
			if(m_btn->delAfterDrop(bDel, (DWORD) wParam))
			{
				*((LPDWORD) lParam) = bDel;
			}
		}
		return S_OK;
	case TBTN_INVOKE_SUPPORTGLASSMENU:
		if(m_btn->SupportGlassMenu())
		{
			LPBOOL bGlass = (LPBOOL) lParam;
			*bGlass = TRUE;
		} else
		{
			LPBOOL bGlass = (LPBOOL) lParam;
			*bGlass = FALSE;
		}
		return S_OK;
	case TBTN_INVOKE_ONRBUTTONDOWN:
		if(m_btn->OnRButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_ONRBUTTONUP:
		if(m_btn->OnRButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_ONMBUTTONDOWN:
		if(m_btn->OnMButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_ONMBUTTONUP:
		if(m_btn->OnMButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_ONLBUTTONDBLCLICK:
		if(m_btn->OnLButtonDblClick(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)))
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_ONDESTROY:
		m_btn->OnDestroy();
		return S_OK;
	case TBTN_INVOKE_BEGINDRAG:
		m_btn->OnBeginDrag((IDataObject*) lParam);
		return S_OK;
	case TBTN_INVOKE_ONMOUSEWHEEL:
		if(m_btn->OnMouseWheel(wParam, lParam)) 
		{
			return S_OK;
		}
		return S_FALSE;
	case TBTN_INVOKE_DROP:
		{
			DROP_OBJECT* drop = (DROP_OBJECT*) lParam;
			drop->dropEffect = m_btn->DropObject(drop->obj, drop->dropEffect);
		}
		return S_OK;
	case TBTN_INVOKE_ACCEPTOBJECT:
		{
			DROP_OBJECT* drop = (DROP_OBJECT*) lParam;
			drop->dropEffect = m_btn->AcceptDropObject(drop->obj, drop->dwKeys);
		}
		return S_OK;
	case TBTN_INVOKE_MENU_MEASUREITEM:
		m_btn->OnMeasureItem(wParam, lParam);
		return S_OK;
	case TBTN_INVOKE_MENU_DRAWITEM:
		m_btn->OnDrawItem(wParam, lParam);
		return S_OK;
	case TBTN_INVOKE_GETTIPSICONSIZE:
		{
			int* icSZ = (int*) lParam;
			*icSZ = m_btn->GetTipsIconSize(wParam);
		}
		return S_OK;
	case TBTN_INVOKE_DRAWTIPSICON:
		{
			DRAW_TIPS_ICON* dIc = (DRAW_TIPS_ICON*) lParam;
			m_btn->DrawTipsIcon(dIc->hDC, dIc->pt, wParam);
		}
		return S_OK;
	case TBTN_INVOKE_GETMENUFLAGS:
		{
			UINT* flags = (UINT*) lParam;
			*flags = m_btn->GetMenuFlags();
		}
		return S_OK;
	case TBTN_INVOKE_QUERYHOTKEY:
		{
			HKM_DATA* hkd = (HKM_DATA*) lParam;
			HOTKEY_DATA hkData;
			ZeroMemory(&hkData, sizeof(hkData));
			if(m_btn->GetHotKeyData((int) wParam, &hkData))
			{
				hkd->id = hkData.id;
				hkd->hotKey = hkData.hotKey;
				hkd->hotKeyID = hkData.hotKeyID;
				memcpy(hkd->name, hkData.name, sizeof(hkData.name));
				return S_OK;
			}
		}
		break;
	case TBTN_INVOKE_HOTKEYSCOUNT:
		{
			int* cnt = (int*) lParam;
			*cnt = m_btn->GetHotkeysCount();
		}
		return S_OK;
	case TBTN_INVOKE_ONHOTKEY:
		m_btn->OnHotkey((UINT) lParam);
		return S_OK;
	case TBTN_INVOKE_GETNAME:
		m_btn->GetButtonName((LPWSTR) lParam);
		return S_OK;
	case TBTN_INVOKE_GETTIPSICON:
		{
			HICON* lpIcon = (HICON*) lParam;
			*lpIcon = m_btn->GetTipIcon(wParam);
		}
		return S_OK;
	case TBTN_INVOKE_GETTIPIMAGE:
		{
			LPCWSTR url = (LPCWSTR) wParam;
			DIB_IMAGE* img = (DIB_IMAGE*) lParam;
			img->bits = m_btn->GetTipImage(img->img_id, url, img->width, img->height, img->redraw_on_ready);
		}
		return S_OK;
	case TBTN_INVOKE_GETTIPSCAPTION:
		{
			LPWSTR* text = (LPWSTR*) lParam;
			*text = m_btn->GetTipCaption(wParam);
			if(!*text) return S_FALSE;
		}
		return S_OK;
	case TBTN_INVOKE_GETTOOLTIPSCOUNT:
		{
			int* tipsCount = (int*) lParam;
			*tipsCount = m_btn->GetTipsCount();
		}
		return S_OK;
	case TBTN_INVOKE_GETTOOLTIPRECT:
		{
			LPRECT rcTip = (LPRECT) lParam;
			m_btn->GetTipRect(wParam, rcTip);
		}
		return S_OK;
	case TBTN_INVOKE_GETTIPSTEXT:
		{
			LPWSTR* text = (LPWSTR*) lParam;
			*text = m_btn->GetTipText(wParam);
			if(!*text) return S_FALSE;
		}
		return S_OK;
	case TBTN_INVOKE_GETTIPSHTML:
		{
			LPWSTR* text = (LPWSTR*) lParam;
			*text = m_btn->GetTipHTML(wParam);
			if(!*text) return S_FALSE;
		}
		return S_OK;
	case TBTN_INVOKE_ONSHOW:
		m_btn->OnShow();
		return S_OK;
	case TBTN_INVOKE_ONHIDE:
		m_btn->OnHide();
		return S_OK;
	case TBTN_INVOKE_CREATEWND:
		m_btn->CreateWnd((HWND) lParam);
		return S_OK;
	case TBTN_INVOKE_ISMENU:
		{
			BOOL* im = (BOOL*) lParam;
			*im = m_btn->isMenu();
		}
		return S_OK;
	case TBTN_INVOKE_GETCHANGESHANDLE:
		{
			HANDLE* hChanges = (HANDLE*) lParam;
			*hChanges = m_btn->GetChangesHandle();
		}
		return S_OK;
	case TBTN_INVOKE_PREPARECHANGES:
		m_btn->PrepareChanges((HANDLE) lParam);
		return S_OK;
	case TBTN_INVOKE_CLOSECHANGESHANDLE:
		m_btn->CloseChangesHandle((HANDLE) lParam);
		return S_OK;
	case TBTN_INVOKE_FIRSTCHILDDATA:
		{
			ENUM_CHILDS* data = (ENUM_CHILDS*) lParam;
			CHILDS_DATA d;
			d.childData		= data->childData;
			d.enumData		= data->enumData;
			d.Name			= (LPWSTR) data->Name;
			if(m_btn->FirstChildData(&d))
			{
				data->childData		= d.childData;
				data->enumData		= d.enumData;
				data->Name			= (LPTSTR) d.Name;
				return S_OK;
			}
		}
		break;
	case TBTN_INVOKE_NEXTCHILDDATA:
		{
			ENUM_CHILDS* data = (ENUM_CHILDS*) lParam;
			CHILDS_DATA d;
			d.childData		= data->childData;
			d.enumData		= data->enumData;
			d.Name			= (LPWSTR) data->Name;
			if(m_btn->NextChildData(&d))
			{
				data->childData		= d.childData;
				data->enumData		= d.enumData;
				data->Name			= (LPTSTR) d.Name;
				return S_OK;
			} else
			{
				data->enumData		= NULL;
			}
		}
		break;
	case TBTN_INVOKE_FREECHILDDATA:
		{
			CHILDS_DATA d;
			d.childData		= (LPVOID) lParam;
			d.enumData		= NULL;
			d.Name			= NULL;
			if(m_btn->FreeChildData(&d))
			{
				return S_OK;
			}
		}
		break;
	case TBTN_INVOKE_CREATECHILD:
		{
			CREATE_CHILDS* data = (CREATE_CHILDS*) lParam;
			CREATE_CHILD_DATA d;
			d.parent		= ((CPlugin*) data->parent)->m_btn;
			d.childData		= data->childData;
			d.child			= NULL;
			if(m_btn->CreateChild(&d))
			{
				data->child = new CPlugin(d.child);
				return S_OK;
			}
		}
		break;
	case TBTN_INVOKE_READCHANGES:
		{
			UINT* changes = (UINT*) lParam;
			*changes = m_btn->ReadChanges();
		}
		return S_OK;
	case TBTN_INVOKE_GETCHILDUID:
		m_btn->GetChildUID((LPWSTR) lParam);
		return S_OK;
	case TBTN_INVOKE_COMPAREBUTTONS:
		{
			COMPARE_BUTTONS* cmpData = (COMPARE_BUTTONS*) lParam;
			COMPARE_BUTTONS_DATA cmpD;
			cmpD.btn1 = ((CPlugin*) cmpData->btn1)->m_btn;
			cmpD.btn2 = ((CPlugin*) cmpData->btn2)->m_btn;
			cmpD.fnc = cmpData->fnc;
			cmpD.result = cmpData->result;
			m_btn->CompareButtons(&cmpD);
			cmpData->result = cmpD.result;
		}
		return S_OK;
	case TBTN_INVOKE_QUERYARRANGEMENU:
		{
			QUERY_ARRANGE_MENU* amData = (QUERY_ARRANGE_MENU*) lParam;
			ARRANGE_MENU_DATA amD;
			amD.name	= (LPWSTR) amData->name;
			amD.sortID	= amData->sortID;
			if(m_btn->QueryArrangeMenu(wParam, &amD))
			{
				amData->name	= (LPTSTR) amD.name;
				amData->sortID	= amD.sortID;
				return S_OK;
			}
		}
		break;
	case TBTN_INVOKE_QUERYARRANGECHILDREN:
		{
			QUERY_ARRANGE_MENU* amData = (QUERY_ARRANGE_MENU*) lParam;
			ARRANGE_MENU_DATA amD;
			amD.name	= (LPWSTR) amData->name;
			amD.sortID	= amData->sortID;
			if(m_btn->QueryArrangeChildren(wParam, &amD))
			{
				amData->name	= (LPTSTR) amD.name;
				amData->sortID	= amD.sortID;
				return S_OK;
			}
		}
		break;
	case TBTN_INVOKE_SUPPORTSORT:
		{
			BOOL* res = (BOOL*) lParam;
			*res = m_btn->SupportSortMenu();
		}
		return S_OK;
	}
	return S_FALSE;
}


void CPlugin::DrawItemText(HDC hDC, LPRECT rcText, LPCWSTR text)
{
	m_container->DrawItemText(hDC, rcText, (LPCSTR) text);
}

HWND CPlugin::GetToolbarWindow()
{
	return m_container->GetBandWND();
}

void CPlugin::GetIconPoint(LPPOINT ptIcon)
{
	m_container->GetIconPoint(ptIcon);
}

DWORD CPlugin::GetButtonType()
{
	DWORD btnType = 0;
	m_container->Invoke(CONTAINER_CMD_GETBUTTONTYPE, 0, (LPARAM) &btnType);
	return btnType;
}

int	CPlugin::GetIconSize()
{
	int szIcon = 16;
	CONTAINER_PARAMS cParams = {0, 0, 0};
	cParams.cbSize = sizeof(CONTAINER_PARAMS);
	if(m_container)
	{
		m_container->GetParams(&cParams);
		szIcon = cParams.dwIconSize;
	}
	return szIcon;
}

DWORD CPlugin::GetViewMode()
{
	int viewMode = 0;
	CONTAINER_PARAMS cParams = {0, 0, 0};
	cParams.cbSize = sizeof(CONTAINER_PARAMS);
	if(m_container)
	{
		m_container->GetParams(&cParams);
		viewMode = cParams.dwViewMode;
	}
	return viewMode;
}

HFONT CPlugin::GetItemFont()
{
	HFONT hFont = 0;
	CONTAINER_PARAMS cParams = {0, 0, 0};
	cParams.cbSize = sizeof(CONTAINER_PARAMS);
	if(m_container)
	{
		m_container->GetParams(&cParams);
		hFont = (HFONT) cParams.hItemFont;
	}
	return hFont;
}

COLORREF CPlugin::GetTextColor()
{
	RECT rcMargins;
	COLORREF txtColor = 0;
	m_container->GetTextProps(&rcMargins, &txtColor);
	return txtColor;
}

void CPlugin::GetTextMargins(LPRECT rcMargins)
{
	ZeroMemory(rcMargins, sizeof(RECT));
	COLORREF txtColor = 0;
	m_container->GetTextProps(rcMargins, &txtColor);
}

void CPlugin::SetHotKey(WORD hk, UINT hkID)
{
	m_container->Invoke(CONTAINER_CMD_SETHOTKEY, hk, hkID);
}

BOOL CPlugin::IsVisible()
{
	BOOL vis = TRUE;
	m_container->Invoke(CONTAINER_CMD_ISVISIBLE, 0, (LPARAM) &vis);
	return vis;
}

void CPlugin::Hide()
{
	m_container->Invoke(CONTAINER_CMD_HIDE, 0, 0);
}

void CPlugin::Show()
{
	m_container->Invoke(CONTAINER_CMD_SHOW, 0, 0);
}

void CPlugin::QueryCreateWindow()
{
	m_container->Invoke(CONTAINER_CMD_CREATEWNDREQUEST, 0, 0);
}

void CPlugin::OpenParentMenu(HWND focusWnd)
{
	m_container->Invoke(CONTAINER_CMD_OPENWINDOW, 0, (LPARAM) focusWnd);
}

void CPlugin::OnKillFocus()
{
	m_container->Invoke(CONTAINER_CMD_KILLFOCUS, 0, 0);
}

void CPlugin::OnRButtonUp()
{
	m_container->Invoke(CONTAINER_CMD_RBUTTONUP, 0, 0);
}

void CPlugin::GetSavedPath(UINT objID, LPWSTR path)
{
	m_container->Invoke(CONTAINER_CMD_GETSAVEDPATH, objID, (LPARAM) path);
}

void CPlugin::SetState(UINT state)
{
	m_container->Invoke(CONTAINER_CMD_SETSTATE, state, NULL);
}

void CPlugin::SaveButton()
{
	m_container->SaveButton();
}

void CPlugin::UpdateSize()
{
	m_container->UpdateButtonInfo(UPDATE_SIZE);
}

void CPlugin::Redraw()
{
	m_container->UpdateButtonInfo(UPDATE_REDRAW);
}

void CPlugin::UpdateTips()
{
	m_container->UpdateButtonInfo(UPDATE_TOOLTIP);
}

void CPlugin::ShowMenu()
{
	m_container->Invoke(CONTAINER_CMD_SHOWMENU, 0, 0);
}

BOOL CPlugin::tboShowCtxIcons()
{
	BOOL ret = TRUE;
	m_container->Invoke(CONTAINER_CMD_GETTOOLBAROPTION, TBO_SHOWCTXICONS, (LPARAM) &ret);
	return ret;
}

void CPlugin::switchToolbar(LPWSTR path)
{
	m_container->AddRef();
	m_container->Invoke(CONTAINER_CMD_SWITCHTOOLBAR, 0, (LPARAM) path);
	m_container->Release();
}

void CPlugin::UpdateTip(UINT tipID, BOOL re_render)
{
	if(m_container)
	{
		m_container->Invoke(CONTAINER_CMD_UPDATETIP, tipID, (LPARAM) re_render);
	}
}

COLORREF CPlugin::GetTextGlowColor()
{
	COLORREF clr = 0xFFFFFFFF;
	if(m_container)
	{
		m_container->Invoke(CONTAINER_CMD_GETGLOWCOLOR, NULL, (LPARAM) &clr);
	}
	return clr;
}

void CPlugin::EnterContextMenu()
{
	if(m_container)
	{
		m_container->Invoke(CONTAINER_CMD_ENTERCTXMENU, NULL, NULL);
	}
}

void CPlugin::LeaveContextMenu()
{
	if(m_container)
	{
		m_container->Invoke(CONTAINER_CMD_LEAVECTXMENU, NULL, NULL);
	}
}

DWORD CPlugin::GetStyle()
{
	DWORD style = 0;
	CONTAINER_PARAMS cParams = {0, 0, 0};
	cParams.cbSize = sizeof(CONTAINER_PARAMS);
	if(m_container)
	{
		m_container->GetParams(&cParams);
		style = cParams.style;
	}
	return style;
}

void CPlugin::GetContentMargins( LPRECT rcMargins )
{
	ZeroMemory(rcMargins, sizeof(RECT));
	m_container->Invoke(CONTAINER_CMD_GETCONTENTMARGINS, 0, (LPARAM) rcMargins);
}

BOOL CPlugin::InMenu()
{
	BOOL inMenu = FALSE;
	m_container->Invoke(CONTAINER_CMD_INMENU, 0, (LPARAM) &inMenu);
	return inMenu;
}

HWND CPlugin::GetLastWindow()
{
	HWND hWnd = NULL;
	m_container->Invoke(CONTAINER_CMD_GETLASTWINDOW, 0, (LPARAM) &hWnd);
	return hWnd;
}

void CPlugin::SetLastWindow( HWND hWnd )
{
	m_container->Invoke(CONTAINER_CMD_SETLASTWINDOW, 0, (LPARAM) hWnd);
}

void CPlugin::drawText( HDC hdc, LPCWSTR text, int c, LPRECT rcText, UINT dtParams, UINT flags, HFONT hFont, COLORREF color, INT vertical, COLORREF glowColor, INT glowSize )
{
	DRAW_TEXT dt;

	dt.hdc			= hdc;
	dt.text			= text;
	dt.count		= c;
	dt.rcDraw		= *rcText;
	dt.dtParams		= dtParams;
	dt.hFont		= hFont;
	dt.flags		= flags;
	dt.textColor	= color;
	dt.glowColor	= glowColor;
	dt.glowSize		= glowSize;
	HRESULT res = m_container->Invoke(CONTAINER_CMD_DRAWTEXT, 0, (LPARAM) &dt);
	*rcText = dt.rcDraw;
	if(res != S_OK)
	{
		BOOL		drawGlow	= FALSE;
		COLORREF	lGlowColor	= glowColor;
		INT			lGlowSize	= glowSize;
		COLORREF	lTextColor	= color;
		if(!(flags & DTF_COLOR))
		{
			lTextColor = GetTextColor();
		}
		if(!(flags & DTF_GLOWCOLOR))
		{
			lGlowColor = GetTextGlowColor();
		}
		if(!(flags & DTF_GLOWSIZE))
		{
			if(lGlowColor != 0xFFFFFFFF)
			{
				lGlowSize = 4;
			} else
			{
				lGlowSize = 0;
			}
		}
		if(flags & DTF_DRAWGLOW)
		{
			drawGlow = TRUE;
		} else if(flags & DTF_NOGLOW)
		{
			drawGlow = FALSE;
		} else
		{
			if(GetTextGlowColor() != 0xFFFFFFFF)
			{
				drawGlow = TRUE;
			} else
			{
				FALSE;
			}
		}
		tlbpdklib::tlbDrawText(hdc, text, c, rcText, dtParams, hFont, lTextColor, vertical, drawGlow, lGlowColor, lGlowSize, FALSE);
	}
}

BOOL CPlugin::requestKbdFocus()
{
	if(m_container->Invoke(CONTAINER_CMD_REQUESTKBDFOCUS, 0, 0) == S_OK)
	{
		return TRUE;
	}
	return FALSE;
}

void CPlugin::releaseKbdFocus()
{
	m_container->Invoke(CONTAINER_CMD_RELEASEKBDFOCUS, 0, 0);
}

DWORD CPlugin::GetMaxMenuWidth()
{
	DWORD width = 250;
	m_container->Invoke(CONTAINER_CMD_GETMAXMENUWIDTH, 0, (LPARAM) &width);
	return width;
}

BOOL CPlugin::selectIcon( HWND parent, LPWSTR iconLocation )
{
	if(m_container->Invoke(CONTAINER_CMD_SELECTICON, (WPARAM) parent, (LPARAM) iconLocation) == S_OK)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CPlugin::isWordWrap()
{
	BOOL ww = TRUE;
	m_container->Invoke(CONTAINER_CMD_ISWORDWRAP, 0, (LPARAM) &ww);
	return ww;
}

BOOL CPlugin::showHotkey()
{
	BOOL ww = FALSE;
	m_container->Invoke(CONTAINER_CMD_SHOWHOTKEY, 0, (LPARAM) &ww);
	return ww;
}

void CPlugin::GetFolder( LPWSTR path, UINT folder_type )
{
	if(m_container->Invoke(CONTAINER_CMD_GETFOLDER, (WPARAM) folder_type, (LPARAM) path) != S_OK)
	{
		switch(folder_type)
		{
		case FOLDER_TYPE_TLBDLL:
			{
				LONG cbData = MAX_PATH * sizeof(WCHAR);
				RegQueryValue(HKEY_CLASSES_ROOT, L"CLSID\\{FC712CA0-A945-11D4-A594-956F6349FC18}\\InprocServer32", path, &cbData);
				PathRemoveFileSpec(path);
				PathAddBackslash(path);
			}
			break;
		case FOLDER_TYPE_USERPROFILE:
			SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
			PathAddBackslash(path);
			StringCchCat(path, MAX_PATH, L"Tordex\\True Launch Bar\\");
			break;
		case FOLDER_TYPE_COMMONPROFILE:
			SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path);
			PathAddBackslash(path);
			StringCchCat(path, MAX_PATH, L"Tordex\\True Launch Bar\\");
			break;
		}
	}
}

BOOL CPlugin::IsPortable()
{
	BOOL ww = FALSE;
	m_container->Invoke(CONTAINER_CMD_ISPORTABLE, 0, (LPARAM) &ww);
	return ww;
}

void CPlugin::AddTipImage( int tipID, LPCWSTR url, void* bits, int width, int height, BOOL redraw_only )
{
	DIB_IMAGE img;
	img.img_id			= tipID;
	img.bits			= bits;
	img.width			= width;
	img.height			= height;
	img.redraw_on_ready	= redraw_only;
	m_container->Invoke(CONTAINER_CMD_TIP_IMAGE_READY, (WPARAM) &img, (LPARAM) url);
}
