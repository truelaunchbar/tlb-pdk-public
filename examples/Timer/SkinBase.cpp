#include "globals.h"
#include "SkinBase.h"

using namespace TxSkin;
using namespace Gdiplus;

CSkinBase::CSkinBase()
{
	addParam(L"started");
	addParam(L"time");
	addParam(L"time-left");
	addParam(L"time-pass");
	addParam(L"hours");
	addParam(L"minutes");
	addParam(L"seconds");
	addParam(L"hours-left");
	addParam(L"minutes-left");
	addParam(L"seconds-left");
	addParam(L"hours-pass");
	addParam(L"minutes-pass");
	addParam(L"seconds-pass");
	addParam(L"name");
	addParam(L"message");
}

CSkinBase::~CSkinBase(void)
{
}

void CSkinBase::loadSkin( std::wstring& skinFile, LPCWSTR skinType )
{
	BOOL bLoadDefault = TRUE;

	clear();

	WCHAR iniFile[MAX_PATH];
	if(!skinFile.empty())
	{
		GetModuleFileName(g_hInst, iniFile, MAX_PATH);
		PathRemoveFileSpec(iniFile);
		PathAddBackslash(iniFile);
		lstrcat(iniFile, L"skins\\");
		lstrcat(iniFile, skinFile.c_str());
		if(PathFileExists(iniFile))
		{
			bLoadDefault = FALSE;
		}
	}
	if(!bLoadDefault)
	{
		bLoadDefault = TRUE;
		WCHAR xmlFile[MAX_PATH];
		GetPrivateProfileString(L"skins", skinType, L"", xmlFile, MAX_PATH, iniFile);
		if(xmlFile[0])
		{
			WCHAR xmlPath[MAX_PATH];
			StringCchCopy(xmlPath, MAX_PATH, iniFile);
			PathRemoveFileSpec(xmlPath);
			PathAddBackslash(xmlPath);
			StringCchCat(xmlPath, MAX_PATH, xmlFile);
			if(PathFileExists(xmlPath))
			{
				if(load(xmlPath))
				{
					bLoadDefault = FALSE;
				}
			}
		}
	}
	if(bLoadDefault)
	{
		loadDefaultSkin();
		return;
	}

	init();
}

void CSkinBase::loadDefaultSkin()
{

}

int CSkinBase::getParamValue( int idx, LPCWSTR param )
{
	return idx;
}

int CSkinBase::getParamValuesCount( LPCWSTR param )
{
	return 1;
}

void CSkinBase::drawText( HDC hdc, LPCWSTR text, int c, LPRECT rcText, UINT dtParams, UINT flags /*= 0*/, HFONT hFont /*= NULL*/, COLORREF color /*= 0*/, INT vertical /*= FALSE*/, COLORREF glowColor /*= 0*/, INT glowSize /*= 0*/ )
{
	BOOL		drawGlow	= FALSE;
	COLORREF	lGlowColor	= glowColor;
	INT			lGlowSize	= glowSize;
	COLORREF	lTextColor	= color;
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
	} else
	{
		drawGlow = FALSE;
	}
	tlbpdklib::tlbDrawText(hdc, text, c, rcText, dtParams, hFont, lTextColor, vertical, drawGlow, lGlowColor, lGlowSize, TRUE);
}
