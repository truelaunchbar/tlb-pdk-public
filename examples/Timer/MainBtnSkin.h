#pragma once

#include "SkinBase.h"

class CMainBtnSkin : public CSkinBase
{
public:
	CMainBtnSkin();
	virtual ~CMainBtnSkin(void);

	void loadSkin(std::wstring& skinFile);
	
protected:
	void loadDefaultSkin();
};
