#include "globals.h"
#include "MainBtnSkin.h"

using namespace TxSkin;
using namespace Gdiplus;

CMainBtnSkin::CMainBtnSkin()
{
	deleteSlice(0);
}

CMainBtnSkin::~CMainBtnSkin(void)
{
}

void CMainBtnSkin::loadDefaultSkin()
{
	getBase()->width(60);
	getBase()->height(18);

	txDrawLib::BrushWrap*	brush1 = NULL;
	txDrawLib::BrushWrap*	brush2 = new txDrawLib::solidBrush( Gdiplus::Color( 128, 255, 255, 255 ) );
	txDrawLib::BrushWrap*	brush3 = new txDrawLib::solidBrush( Gdiplus::Color( 180, 255, 255, 255 ) );

	TxSkin::elColorRect* bg = new TxSkin::elColorRect(this, NULL, FALSE, L"stop");
	bg->addColor(TxSkin::modParamValue((int) 0), brush1, NULL, TxSkin::elStateNormal);
	bg->addColor(TxSkin::modParamValue((int) 0), brush2, NULL, TxSkin::elStateHover);
	bg->addColor(TxSkin::modParamValue((int) 0), brush3, NULL, TxSkin::elStatePushed);
	getBase()->addElement(bg);

	TxSkin::elText* time = new TxSkin::elText(this, L"time-left", L"Arial Rounded MT Bold", FALSE, FALSE, DT_SINGLELINE | DT_CENTER | DT_VCENTER, Gdiplus::Color(255, 255, 255, 255), TRUE, 2, Gdiplus::Color(255, 0, 0, 0));
	time->X(0);
	time->Y(0);
	time->width(60);
	time->height(18);
	getBase()->addElement(time);

	init();
}

void CMainBtnSkin::loadSkin( std::wstring& skinFile )
{
	CSkinBase::loadSkin(skinFile, INI_MENUBTN_SKIN);
}

