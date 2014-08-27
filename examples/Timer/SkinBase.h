#pragma once

class CSkinBase : public TxSkin::CSkin
{
public:
	CSkinBase();
	virtual ~CSkinBase(void);

	void loadSkin(std::wstring& skinFile, LPCWSTR skinType);

	virtual void drawText(HDC hdc, LPCWSTR text, int c, LPRECT rcText, UINT dtParams, UINT flags = 0, HFONT hFont = NULL, COLORREF color = 0, INT vertical = FALSE, COLORREF glowColor = 0, INT glowSize = 0);

protected:
	virtual void	loadDefaultSkin();
	virtual int		getParamValue(int idx, LPCWSTR param);
	virtual int		getParamValuesCount(LPCWSTR param);
};
