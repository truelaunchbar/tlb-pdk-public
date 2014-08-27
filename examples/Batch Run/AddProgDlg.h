#pragma once

class CAddProgDlg : public CXUIDialog
{
public:
	FILE_TO_RUN	m_fl;

	CAddProgDlg(CXUIEngine* engine);
	virtual ~CAddProgDlg(void);

	virtual void OnInitDialog();
	virtual BOOL OnEndDialog(UINT code);

	XUI_BEGIN_DATA_MAP
		XUI_DATA_MAP_PSTR(L"fileName",		m_fl.fileName)
		XUI_DATA_MAP_PSTR(L"params",		m_fl.params)
		XUI_DATA_MAP_PSTR(L"workingDir",	m_fl.workingDir)
	XUI_END_DATA_MAP
};
