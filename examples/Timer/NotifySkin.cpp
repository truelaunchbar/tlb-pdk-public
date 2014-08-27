#include "globals.h"
#include "NotifySkin.h"

CNotifySkin::CNotifySkin()
{
}

CNotifySkin::~CNotifySkin(void)
{
}

void CNotifySkin::loadSkin( std::wstring& skinFile )
{
	CSkinBase::loadSkin(skinFile, INI_NOTIFY_SKIN);
}

void CNotifySkin::loadDefaultSkin()
{
	getBase()->width(200);
	getBase()->height(34);

	TxSkin::elImage* bg = new TxSkin::elImage(this, NULL);
	bg->addImage(L"notify.png",	L"SKIN", g_hInst, TxSkin::modParamValue((int) 0), TxSkin::elStateNormal);
	bg->X(0);
	bg->Y(0);
	getBase()->addElement(bg);

	getBase()->width(bg->width());
	getBase()->height(bg->height());

	TxSkin::elText* name = new TxSkin::elText(this, L"name", L"Tahoma", TRUE, FALSE, DT_LEFT | DT_NOPREFIX, Gdiplus::Color(255, 255, 255, 255));
	name->X(14);
	name->Y(14);
	name->width(372);
	name->height(40);
	getBase()->addElement(name);

	name = new TxSkin::elText(this, L"message", L"Tahoma", FALSE, FALSE, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK, Gdiplus::Color(255, 255, 255, 255));
	name->X(14);
	name->Y(54);
	name->width(372);
	name->height(129);
	name->fontSize(20);
	getBase()->addElement(name);

	TxSkin::elImage* btn = new TxSkin::elImage(this, NULL, L"close");
	btn->addImageStates(L"notify-close.png",	L"SKIN", g_hInst, TRUE, 3, TxSkin::modParamValue((int) 0));
	btn->X(333);
	btn->Y(183);
	getBase()->addElement(btn);

	btn = new TxSkin::elImage(this, NULL, L"restart");
	btn->addImageStates(L"notify-restart.png",	L"SKIN", g_hInst, TRUE, 3, TxSkin::modParamValue((int) 0));
	btn->X(285);
	btn->Y(183);
	getBase()->addElement(btn);

	init();
}
