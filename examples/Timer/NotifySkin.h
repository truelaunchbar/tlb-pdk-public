#pragma once
#include "skinbase.h"

class CNotifySkin : public CSkinBase
{
public:
	CNotifySkin();
	virtual ~CNotifySkin(void);

	void loadSkin(std::wstring& skinFile);

protected:
	void loadDefaultSkin();
};
