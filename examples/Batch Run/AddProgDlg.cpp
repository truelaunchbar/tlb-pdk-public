#include "globals.h"
#include ".\addprogdlg.h"
#include "resource.h"
#include <shlobj.h>

CAddProgDlg::CAddProgDlg(CXUIEngine* engine) : CXUIDialog(L"res:add.xml", engine)
{
	ZeroMemory(&m_fl, sizeof(FILE_TO_RUN));
}

CAddProgDlg::~CAddProgDlg(void)
{
	FREE_CLEAR_STR(m_fl.fileName);
	FREE_CLEAR_STR(m_fl.params);
	FREE_CLEAR_STR(m_fl.workingDir);
}

void CAddProgDlg::OnInitDialog()
{
	CXUIDialog::OnInitDialog();
	dataExchange(FALSE, NULL);
}

BOOL CAddProgDlg::OnEndDialog(UINT code)
{
	if(code == IDOK)
	{
		dataExchange(TRUE, NULL);
	}
	return TRUE;
}

