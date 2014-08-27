#include "globals.h"
#include "AddTimerBtn.h"
#include "TimerMainBtn.h"
#include "xuistrings.h"
#include "resource.h"

// {A7D0478E-4F70-4eb2-806C-975FC83C20C1}
GUID CLSID_ADDBTN = 
{ 0xa7d0478e, 0x4f70, 0x4eb2, { 0x80, 0x6c, 0x97, 0x5f, 0xc8, 0x3c, 0x20, 0xc1 } };


CAddTimerBtn::CAddTimerBtn(CTimerMainBtn* parent)
{
	m_parent = parent;
	MAKE_STR(m_iconLocation, parent->addTimerIconLocation());
}

CAddTimerBtn::~CAddTimerBtn(void)
{
}

UINT CAddTimerBtn::GetSupportedActions()
{
	return CTlbButton::GetSupportedActions() | CTMS_CHANGEICON | CTMS_RESETICON | CTMS_HOTKEY;
}

UINT CAddTimerBtn::GetModeFlags()
{
	return CTlbButton::GetModeFlags() | BTN_FLAG_SUPPORTPNGICONS;
}

void CAddTimerBtn::GetChildUID( LPWSTR uid )
{
	lstrcpy(uid, L"addtimer");
}

void CAddTimerBtn::GetButtonText( LPWSTR text )
{
	StringCchCopy(text, 255, m_parent->getXUI()->getStringDef(XUI_STR_ADDTIMERBTN));
}

BOOL CAddTimerBtn::LoadIcon( INT iSize )
{
	if(!CTlbButton::LoadIcon( iSize ) )
	{
		m_hIcon = 
			(HICON) LoadImage(
			g_hInst, 
			MAKEINTRESOURCE( IDI_ADD ), 
			IMAGE_ICON, 
			iSize, 
			iSize, 
			LR_DEFAULTCOLOR );

		m_loadedIconSize = iSize;
	}

	return m_hIcon ? TRUE : FALSE;
}

BOOL CAddTimerBtn::OnRun()
{
	m_parent->addNewTimer();
	return TRUE;
}

BOOL CAddTimerBtn::SetIconLocation( LPCWSTR szLocation )
{
	BOOL ret = CTlbButton::SetIconLocation(szLocation);
	m_parent->addTimerIconLocation(m_iconLocation);
	return ret;
}

void CAddTimerBtn::GetButtonName( LPWSTR name )
{
	StringCchCopy(name, 255, m_parent->getXUI()->getStringDef(XUI_STR_ADDTIMERBTN));
}

BOOL CAddTimerBtn::QueryClass( CLSID classID, LPVOID* lpClass )
{
	if(classID == CLSID_ADDBTN)
	{
		*lpClass = this;
		return TRUE;
	}
	return FALSE;
}

LPWSTR CAddTimerBtn::GetTipCaption( int tipID )
{
	std::wstring tip = m_parent->getXUI()->getStringDef(XUI_STR_ADDTIMERBTN);

	LPWSTR text = (LPWSTR) CoTaskMemAlloc((tip.length() + 1) * sizeof(WCHAR));
	lstrcpy(text, tip.c_str());

	return text;
}

LPWSTR CAddTimerBtn::GetTipText( int tipID )
{
	std::wstring tip = m_parent->getXUI()->getStringDef(XUI_STR_ADDTIMERTIP);

	LPWSTR text = (LPWSTR) CoTaskMemAlloc((tip.length() + 1) * sizeof(WCHAR));
	lstrcpy(text, tip.c_str());

	return text;
}

void CAddTimerBtn::OnHotkey( UINT hkID )
{
	OnRun();
}

HICON CAddTimerBtn::GetTipIcon( int tipID )
{
	return (HICON) 0xFFFFFFFF;
}

int CAddTimerBtn::GetTipsIconSize( int tipID )
{
	return 48;
}

void CAddTimerBtn::DrawTipsIcon( HDC hDC, POINT pt, int tipID )
{
	m_parent->getXUI()->DrawImage(hDC, pt.x, pt.y, 48, 48, L"logo.png");
}