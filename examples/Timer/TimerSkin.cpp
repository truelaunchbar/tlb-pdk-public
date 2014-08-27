#include "globals.h"
#include "TimerSkin.h"

using namespace TxSkin;
using namespace Gdiplus;

CTimerSkin::CTimerSkin()
{
}

CTimerSkin::~CTimerSkin(void)
{
}

void CTimerSkin::loadSkin( std::wstring& skinFile )
{
	CSkinBase::loadSkin(skinFile, INI_MENU_SKIN);
}

void CTimerSkin::loadDefaultSkin()
{
	getBase()->width(170);
	getBase()->height(34);

	TxSkin::elImage* btn = new TxSkin::elImage(this, L"started", L"startstop");
	btn->addImageStates(L"start.png",	L"SKIN", g_hInst, TRUE, 3, TxSkin::modParamValue((int) 0));
	btn->addImageStates(L"stop.png",	L"SKIN", g_hInst, TRUE, 3, TxSkin::modParamValue((int) 1));
	btn->X(138);
	btn->Y(1);
	getBase()->addElement(btn);

	TxSkin::elText* time = new TxSkin::elText(this, L"time-left", L"Arial Rounded MT Bold", FALSE, FALSE, DT_SINGLELINE | DT_LEFT | DT_VCENTER, Gdiplus::Color(255, 255, 255, 255), TRUE, 2, Gdiplus::Color(255, 0, 0, 0), L"edit");
	time->X(1);
	time->Y(1);
	time->width(138);
	time->height(30);
	getBase()->addElement(time);

	init();
}

int CTimerSkin::getParamValuesCount( LPCWSTR param )
{
	if(!StrCmpI(param, L"started"))
	{
		return 2;
	}
	return 1;
}
