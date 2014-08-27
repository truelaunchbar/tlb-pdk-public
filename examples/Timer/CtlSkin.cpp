#include "globals.h"
#include "CtlSkin.h"

CCtlSkin::CCtlSkin()
{
}

CCtlSkin::~CCtlSkin(void)
{
}

void CCtlSkin::loadSkin( std::wstring& skinFile )
{
	CSkinBase::loadSkin(skinFile, INI_CONTROLS_SKIN);
}

void CCtlSkin::loadDefaultSkin()
{
	getBase()->width(16);
	getBase()->height(16);

	TxSkin::elImage* btn = new TxSkin::elImage(this, NULL, L"menu");
	btn->addImageStates(L"menu-btn.png", L"SKIN", g_hInst, TRUE, 3, TxSkin::modParamValue((int) 0));
	btn->X(0);
	btn->Y(0);
	getBase()->addElement(btn);

	init();
}
