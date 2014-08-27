#pragma once

#include "SkinBase.h"

class CTimerSkin : public CSkinBase
{
public:
	CTimerSkin();
	virtual ~CTimerSkin(void);

	void loadSkin(std::wstring& skinFile);

protected:
	virtual void	loadDefaultSkin();
	virtual int		getParamValuesCount(LPCWSTR param);
};
