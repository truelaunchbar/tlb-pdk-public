#pragma once
#include "SkinBase.h"

class CCtlSkin : public CSkinBase
{
public:
	CCtlSkin();
	virtual ~CCtlSkin(void);

	void loadSkin(std::wstring& skinFile);

private:
	void loadDefaultSkin();
};
