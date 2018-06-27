#pragma once

#include <windows.h>
#include <comcat.h>
#include <ole2.h>
#include "tlbif.h"
#include <olectl.h>
#include <stdint.h>

#define HOTKEY_MOD_SHIFT	0x0100
#define HOTKEY_MOD_CONTROL	0x0200
#define HOTKEY_MOD_ALT		0x0400
#define HOTKEY_MOD_WIN		0x2000

//Object types for CONTAINER_CMD_GETSAVEDPATH
#define OT_ICONS			0x00
#define OT_PLUGINFILES		0x01

// Toolbar options (CONTAINER_CMD_GETTOOLBAROPTION)
#define TBO_SHOWCTXICONS	1		// lParam is (LPBOOL)
#define TBO_TOOLBARFOLDER	2		// lParam is (TCHAR[MAX_PATH])

// flags for CONTAINER_CMD_DRAWTEXT
#define	DTF_DRAWGLOW			0x01
#define	DTF_NOGLOW				0x02
#define	DTF_GLOWCOLOR			0x04
#define	DTF_COLOR				0x08
#define	DTF_GLOWSIZE			0x10
#define	DTF_FORCETRANSPARENTDC	0x20
#define	DTF_SHADOW				0x40

// folder type for CONTAINER_CMD_GETFOLDER
#define FOLDER_TYPE_TLBDLL			0
#define FOLDER_TYPE_USERPROFILE		1
#define FOLDER_TYPE_COMMONPROFILE	2
#define FOLDER_TYPE_TOOLBARS		4
#define FOLDER_TYPE_STD_TOOLBAR		5


// constants for ITlbContainer::Invoke
#define CONTAINER_CMD_SETHOTKEY				2 //wParam - hotkey code, lParam - hotkey ID
#define CONTAINER_CMD_ISVISIBLE				3 //(BOOL*) lParam
#define CONTAINER_CMD_HIDE					4 //hide button
#define CONTAINER_CMD_SHOW					5 //show button
#define CONTAINER_CMD_CREATEWNDREQUEST		6 //request for creating window
#define CONTAINER_CMD_OPENWINDOW			7 //
#define CONTAINER_CMD_KILLFOCUS				8 //notify parent about WM_KILFOCUS
#define CONTAINER_CMD_RBUTTONUP				9 //notify parent about WM_RBUTTONUP
#define CONTAINER_CMD_GETSAVEDPATH			10 //wParam - object type; lParam - LPWSTR pointer to string
#define CONTAINER_CMD_SETSTATE				11 //wParam - state of button
#define CONTAINER_CMD_SHOWMENU				12 //
#define CONTAINER_CMD_ENTERCTXMENU			13 //
#define CONTAINER_CMD_LEAVECTXMENU			14 //
#define CONTAINER_CMD_UPDATETIP				15 //wParam - zero based tip index
#define CONTAINER_CMD_REARRANGECHILDS		16 
#define CONTAINER_CMD_GETTOOLBAROPTION		17 //wParam - option ID, lParam - return value
#define CONTAINER_CMD_SWITCHTOOLBAR			18 //lParam - path to the new toolbar folder
#define CONTAINER_CMD_GETDAFULTSKIN			19 //lParam - LPWSTR path to the skin wParam sizeof LPWSTR in chars
#define CONTAINER_CMD_GETBUTTONTYPE			20 //lParam - LPDWORD 
#define CONTAINER_CMD_GETGLOWCOLOR			21 //lParam - LPCOLORREF
#define CONTAINER_CMD_GETCONTENTMARGINS		22 //lParam - LPRECT
#define CONTAINER_CMD_INMENU				23 //lParam - LPBOOL
#define CONTAINER_CMD_GETLASTWINDOW			24 //lParam - LPHWND
#define CONTAINER_CMD_SETLASTWINDOW			25 //lParam - HWND
#define CONTAINER_CMD_DRAWTEXT				26 //lParam - DRAW_TEXT
#define CONTAINER_CMD_REQUESTKBDFOCUS		27 //Request for keyboard focus
#define CONTAINER_CMD_RELEASEKBDFOCUS		28 //Release the keyboard focus
#define CONTAINER_CMD_GETMAXMENUWIDTH		29 //(LPDWORD) lParam - max menu width
#define CONTAINER_CMD_SELECTICON			30 //(LPWSTR) lParam - in: current icon location, out: selected icon location
#define CONTAINER_CMD_ISWORDWRAP			31 //lParam - LPBOOL
#define CONTAINER_CMD_GETFOLDER				32 //lParam - LPWSTR (MAX_PATH), wParam - folder type
#define CONTAINER_CMD_ISPORTABLE			33 //
#define CONTAINER_CMD_TIP_IMAGE_READY		34 //(LPWSTR) lParams - image url. (DIB_IMAGE*) wParam
#define CONTAINER_CMD_SHOWHOTKEY			35 //lParam - LPBOOL

// constants for ITlbContainer::UpdateButtonInfo
#define UPDATE_SIZE					0x01
#define UPDATE_REDRAW				0x02
#define UPDATE_TOOLTIP				0x04

// constants for BUTTONINFO.dwMask
#define BTNINFO_MASK_MODEFLAG		0x01
#define BTNINFO_MASK_ACTIONS		0x02
#define BTNINFO_MASK_SIZE			0x04
#define BTNINFO_MASK_ICON			0x08
#define BTNINFO_MASK_SELECTEDICON	0x10
#define BTNINFO_MASK_DESCRIPTION	0x20
#define BTNINFO_MASK_TEXT			0x40
#define BTNINFO_MASK_ACTUALSIZE		0x80

// constants for BUTTONINFO.dwModeFlags
#define BTN_FLAG_PLANE				0x00000001
#define BTN_FLAG_STOPDRAG			0x00000002
#define BTN_FLAG_COLAPSEONRUN		0x00000004
#define BTN_FLAG_NOTIPS				0x00000008
#define BTN_FLAG_SUPPORTSAVEDICON	0x00000010
#define BTN_FLAG_SUPPORTPNGICONS	0x00000020
#define BTN_FLAG_INPLACERENAME		0x00000040
#define BTN_FLAG_UNICODE			0x00000080
#define BTN_FLAG_NOTEXT				0x00000100
#define BTN_FLAG_NEED_R_CLICKS		0x00100000
#define BTN_FLAG_NEED_L_DBLCLICK	0x00200000
#define BTN_FLAG_DONTACTIVATE		0x00400000
#define BTN_FLAG_SEPARATOR			0x00800000
#define BTN_FLAG_NORUNSTRACK		0x01000000

// constants for BUTTONINFO.dwActions
#define CTMS_PROPERTIES	0x00001
#define CTMS_RENAME		0x00002
#define CTMS_CHANGEICON	0x00004
#define CTMS_HOVERRUN	0x00020
#define CTMS_HOTKEY		0x40		// change the hotkey
#define CTMS_FINDTARGET	0x00080
#define CTMS_OPENFOLDER	0x00100
#define CTMS_NODELETE	0x00200
//#define CTMS_NOSELFDELETE	0x400
#define CTMS_RESETICON	0x00800
#define CTMS_SETIMAGE	0x01000
#define CTMS_ABOUT		0x02000
#define CTMS_DELETE		0x04000
#define CTMS_RUNMENU	0x08000
#define CTMS_RUNALL		0x10000
#define CTMS_MCLICK		0x20000
#define CTMS_EXPAND		0x40000
#define CTMS_COLLAPSE	0x80000
#define CTMS_ALTCLICK	0x100000
#define CTMS_TRACKRUNS	0x200000

//Button states
#define BTN_S_PUSHED	0x01
#define BTN_S_SELECTED	0x02
#define BTN_S_INMENU	0x04
#define BTN_S_INACTION	0x08

#define TLB_ARRAYSIZE(a)    (sizeof(a)/sizeof(a[0]))

//Constans for button state
#define TS_STATE_NORMAL			0x01
#define TS_STATE_OVER			0x02
#define TS_STATE_DOWN			0x03
#define TS_STATE_OPENACTIVE		0x04
#define TS_STATE_OPENINACTIVE	0x05
#define TS_STATE_COUNT		TS_STATE_OPENINACTIVE

//Constants for button type
#define TS_ITEM_BTN					0x0001
#define TS_ITEM_MENUITEM			0x0002
#define TS_ITEM_MENUBTN				0x0004
#define TS_ITEM_MENUSEPARATOR		0x0005
#define TS_ITEM_SEPARATOR			0x0006
#define TS_ITEM_TEXTSEPARATOR		0x0007
#define TS_ITEM_MENUITEMSUBMENU		0x0008
#define TS_ITEM_BTNVERT				0x0009
#define TS_ITEM_MENUBTNVERT			0x000A
#define TS_ITEM_SEPARATORVERT		0x000B
#define TS_ITEM_CAPTION				0x000C
#define TS_ITEM_CHEVRON				0x000D
#define TS_ITEM_CHEVRONVERT			0x000E
#define TS_ITEM_TOPMENUSCROLLER		0x000F
#define TS_ITEM_BOTTOMMENUSCROLLER	0x0010
#define TS_ITEM_MENUBACKGROUND		0x0020

#define TS_ITEM_SBTOP				0x0011
#define TS_ITEM_SBBOTTOM			0x0012
#define TS_ITEM_SB					0x0013
#define TS_ITEM_SBROLLER			0x0014

#define TS_ITEM_SEARCHBOX			0x0015

#define TS_ITEM_MENU				0x0016
#define TS_ITEM_MENU_WITH_CAPTION	0x0017

#define TS_ITEM_IV_MENUITEM			0x0019
#define TS_ITEM_IV_SUBMENU			0x001A

#define TS_ITEM_MAX					TS_ITEM_IV_SUBMENU

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
#define TS_ITEM_RIGHTTEXT			0x100000
#define TS_ITEM_TRANSPARENT			0x200000
#define TS_ITEM_ICONVIEW			0x400000
#define TS_ITEM_WRAPTEXT			0x800000

//for TBTN_INVOKE_GETMENUFLAGS
#define MENU_F_NORENAME				0x00001
#define MENU_F_NOAUTOSHOW			0x00002
#define MENU_F_NOTSHOWONCLICK		0x00004
#define MENU_F_RUNONCLICK			0x00008
#define MENU_F_SUPPORT_DELAYED		0x00010
#define MENU_F_CHILDRENSORT			0x00020
#define MENU_F_VARHEIGHT			0x00040
#define MENU_F_DEFAULTWRAP			0x00080

// for TBTN_INVOKE_GET_TIP_FLAGS
#define TIP_F_INTERACTIVE			0x01
#define TIP_F_FULL_HTML				0x02


//Constants for ITlbButton::Invoke
#define TBTN_INVOKE_GETTOOLTIPSCOUNT	1  // wParam ignored, (int*) lParam - tooltips count
#define TBTN_INVOKE_GETTOOLTIPRECT		2  // wParam - zero based tooltip index, (RECT*) lParam - rectangle of tooltip
#define TBTN_INVOKE_GETTOOLTIPTEXT		3  // wParam - zero based tooltip index, (char*) lParam - Text of tooltip (1024 max)
#define TBTN_INVOKE_ONHOTKEY			25  // lParam - ID of the hotkey
#define TBTN_INVOKE_QUERYHOTKEY			4  // wParam - index of the hotkey (HKM_DATA*) lParam
#define TBTN_INVOKE_HOTKEYSCOUNT		5  // (int*) lParam - hotkeys count
#define TBTN_INVOKE_CREATEWND			6  // Create child windows here
#define TBTN_INVOKE_ONHIDE				7  // The button become hidden
#define TBTN_INVOKE_ONSHOW				8  // The button become visible

#define TBTN_INVOKE_ISMENU				9   // (BOOL*) lParam - TRUE if menu
#define TBTN_INVOKE_GETCHANGESHANDLE	10  // (HANDLE*) lParam - changes handle
#define TBTN_INVOKE_PREPARECHANGES		11  // (HANDLE) lParam - changes handle
#define TBTN_INVOKE_CLOSECHANGESHANDLE	12  // (HANDLE) lParam - changes handle
#define TBTN_INVOKE_FIRSTCHILDDATA		13  // (ENUM_CHILDS*) lParam
#define TBTN_INVOKE_NEXTCHILDDATA		14  // (ENUM_CHILDS*) lParam
#define TBTN_INVOKE_FREECHILDDATA		15  // (ENUM_CHILDS*) lParam
#define TBTN_INVOKE_CREATECHILD			16  // (CREATE_CHILDS*) lParam
#define TBTN_INVOKE_READCHANGES			17  // (UINT*) lParam: 0 - no changes, 1 - child changed, 2 - child deleted
#define TBTN_INVOKE_GETCHILDUID			18  // (LPTSTR) lParam - unique child string
#define TBTN_INVOKE_SUPPORTSORT			19  // (BOOL*) lParam - TRUE if plugin supports custom arrange of items
#define TBTN_INVOKE_COMPAREBUTTONS		20  // (COMPARE_BUTTONS*) lParam
#define TBTN_INVOKE_QUERYARRANGEMENU	21  // (QUERY_ARRANGE_MENU*) lParam

#define TBTN_INVOKE_GETTIPSTEXT			22  // wParam - zero based tooltip index, (LPWSTR*) lParam - Text of tooltip use CoTaskMemAlloc to allocate memory
#define TBTN_INVOKE_GETTIPSICON			23  // wParam - zero based tooltip index, (HICON*) lParam - Text of tooltip (1024 max)
#define TBTN_INVOKE_GETTIPSCAPTION		24  // wParam - zero based tooltip index, (LPWSTR*) lParam - Text of tooltip use CoTaskMemAlloc to allocate memory
#define TBTN_INVOKE_GETMENUFLAGS		26  // (UINT*) lParam - flags
#define TBTN_INVOKE_GETNAME				27  // (LPWSTR) lParam - Name of button (max name size is MAX_PATH)
#define TBTN_INVOKE_GETTIPSICONSIZE		28  // wParam - zero based tooltip index, (int*) lParam - size of icon
#define TBTN_INVOKE_DRAWTIPSICON		29  // wParam - zero based tooltip index, (DRAW_TIPS_ICON*) lParam - size of icon

#define TBTN_INVOKE_MENU_MEASUREITEM	30  // REDIRECT WM_MEASUREITEM
#define TBTN_INVOKE_MENU_DRAWITEM		31  // REDIRECT WM_DRAWITEM

#define TBTN_INVOKE_SKINCHANGED			32  // Process this message if your plugin uses default skin

#define TBTN_INVOKE_ONMOUSEWHEEL		33  // WM_MOUSEWHEEL

// Drg&Drop
#define TBTN_INVOKE_ACCEPTOBJECT		34  // lParam - DROP_OBJECT*
#define TBTN_INVOKE_DROP				35  // lParam - DROP_OBJECT*
#define TBTN_INVOKE_BEGINDRAG			36  // fill IDataObject from lParam. Use IDataObject::SetData

#define TBTN_INVOKE_ONDESTROY			40  // Object will be destroyed. You can save unsaved data here.

#define TBTN_INVOKE_ONRBUTTONDOWN		41  // WM_RBUTTONDOWN
#define TBTN_INVOKE_ONRBUTTONUP		    42  // WM_RBUTTONUP
#define TBTN_INVOKE_ONLBUTTONDBLCLICK   43  // WM_RBUTTONDBLCLICK

#define TBTN_INVOKE_SUPPORTGLASSMENU	44  // (BOOL*) lParam - TRUE if plugin supports glass menu

#define TBTN_INVOKE_OKDRAGEFFECTS		45  // (DWORD*) lParam - combination of DROPEFFECT_* constant valid during dragging the object
#define TBTN_INVOKE_DELAFTERDROP		46  // (BOOL*) lParam - retun TRUE to delete button after drop or use this as successful drag&drop operation notification

#define TBTN_INVOKE_F_FIRSTCHILDDATA	47  // (ENUM_CHILDS_F*) lParam
#define TBTN_INVOKE_F_NEXTCHILDDATA		48  // (ENUM_CHILDS_F*) lParam
#define TBTN_INVOKE_SUPPORTSEARCH		49  // (BOOL*) lParam - TRUE if plugin supports searching in the menus

#define TBTN_INVOKE_VALIDVIEWSTYLE		50  // WPARAM - view style (0 - default, 1 - icons, 2- icons & text) (BOOL*) lParam - TRUE if plugin supports the view style

#define TBTN_INVOKE_APPLYMARGINS		51  // (BOOL*) lParam - TRUE if apply the button margins to the plugin

#define TBTN_INVOKE_ONMBUTTONDOWN		52  // WM_MBUTTONDOWN
#define TBTN_INVOKE_ONMBUTTONUP		    53  // WM_MBUTTONUP

#define TBTN_INVOKE_ONKEYDOWN		    54  // WM_KEYDOWN
#define TBTN_INVOKE_ONKEYUP				55  // WM_KEYUP
#define TBTN_INVOKE_ONCHAR				56  // WM_CHAR
#define TBTN_INVOKE_ONSETFOCUS			57 
#define TBTN_INVOKE_ONKILLFOCUS			58 
#define TBTN_INVOKE_ONSYSKEYDOWN	    59  // WM_SYSKEYDOWN
#define TBTN_INVOKE_ONSYSKEYUP			60  // WM_SYSKEYUP
#define TBTN_INVOKE_ONSYSCHAR			61  // WM_SYSCHAR
#define TBTN_INVOKE_ONSETCURSOR				62  // WM_SETCURSOR
#define TBTN_INVOKE_QUERYARRANGECHILDREN	63  // (QUERY_ARRANGE_MENU*) lParam
#define TBTN_INVOKE_GETHEIGHT				64  // (GET_HEIGHT*) lParam

#define TBTN_INVOKE_GETTIPIMAGE				65  // (LPCWSTR) wParam - image url, (DIB_IMAGE*) lParam
#define TBTN_INVOKE_GETTIPSHTML				66  // wParam - zero based tooltip index, (LPWSTR*) lParam - Text of tooltip use CoTaskMemAlloc to allocate memory

#define TBTN_INVOKE_GET_TIP_FLAGS			67  // wParam - zero based tooltip index, (UINT*) lParam - return flags TIP_F_*
#define TBTN_INVOKE_ON_CLICK_TIP			68  // wParam - zero based tooltip index, (LPCWSTR) lParam - anchor url
#define TBTN_INVOKE_GET_TIP_FULL_HTML		69  // wParam - zero based tooltip index, (LPWSTR*) lParam - Text of tooltip use CoTaskMemAlloc to allocate memory
#define TBTN_INVOKE_QUERY_VERBS				70  // (int*) wParam - return verbs count here, (LPWSTR**) lParam - array of the verbs (first string: Name, second string: verb)
#define TBTN_INVOKE_RUN_VERB				71  // (LPCWSTR) lParam - verb string
#define TBTN_INVOKE_GET_JUMPLIST			72  // (JUMPLIST_ARRAY*) lParam Jumplists array. (int) wParam - zero based tooltip index. Use CoTaskMemFree to delete pointers.
#define TBTN_INVOKE_RUN_JUMPLIST			73  // (JUMPLIST_IDX*) lParam - jumplist id, (int) lParam - zero based tooltip index.
#define TBTN_INVOKE_GET_JUMPLIST_ICON		74  // (DIB_IMAGE*) lParam - image (DIB_IMAGE::img_id is the tipid). (JUMPLIST_IDX*) wParam - jumplist item ID.

struct JUMPLIST
{
    LPWSTR      name;
    uint32_t    type;
    LPWSTR*     items;
    int         count;
};

struct JUMPLIST_ARRAY
{
    int         count;
    JUMPLIST*   jumplists;
};

struct JUMPLIST_IDX
{
    int group_idx;
    int item_idx;
};

//Register
extern BOOL RegisterPlugin(HINSTANCE g_hInst, CLSID clsid, LPSTR lpszTitle);
extern BOOL UnregisterPlugin(CLSID clsid);
extern CATID CATID_TLBBUTTONS;

// QueryStatus for ITrueLaunchBar interface
#define TLBID_DPIAUTOSCALE		1