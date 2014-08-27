#include "globals.h"
#include ".\propsdlg.h"
#include "resource.h"
#include <CommCtrl.h>
#include "AddProgDlg.h"
#include "xuistrings.h"

CPropsDlg::CPropsDlg(CXUIEngine* engine) : CXUIDialog(L"res:properties.xml", engine)
{
}

CPropsDlg::~CPropsDlg(void)
{
}

void CPropsDlg::FillList( int activeIDX )
{
	m_ctlFiles->clearItems();
	for(int i=0; i < m_progs.GetCount(); i++)
	{
		int idx = m_ctlFiles->insertItem(m_progs[i].fileName, 0);
		if(activeIDX == i)
		{
			m_ctlFiles->selectItem(idx);
		}
	}
}

void CPropsDlg::OnInitDialog()
{
	CXUIDialog::OnInitDialog();
	dataExchange(FALSE, NULL);
	FillList(0);

	DragAcceptFiles(m_ctlFiles->get_wnd(), TRUE);
	SetProp(m_ctlFiles->get_wnd(), TEXT("parent_dlg"), (HANDLE) this);
	m_oldListProc = (WNDPROC) SetWindowLongPtr(m_ctlFiles->get_wnd(), GWLP_WNDPROC, (LONG_PTR) CPropsDlg::ListWndProc);

	// fill vertion tab
	TCHAR str[MAX_PATH];
	GetModuleFileName(g_hInst, str, MAX_PATH);
	DWORD versionLS, versionMS;
	GetFileVersion(str, versionMS, versionLS);

	TCHAR strVer[255];
	Version2Str(versionMS, versionLS, strVer);
	TCHAR strText[255];
	wsprintf(strText, TEXT("v%s"), strVer);

	find(L"version")->value_STR(strText);
}

BOOL CPropsDlg::OnEndDialog( UINT code )
{
	if(code == IDOK)
	{
		dataExchange(TRUE, NULL);
	}
	return TRUE;
}

LRESULT CALLBACK CPropsDlg::ListWndProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	CPropsDlg* pThis = NULL;
	pThis = (CPropsDlg*)GetProp(hWnd, TEXT("parent_dlg"));
	if(uMessage == WM_DROPFILES)
	{
		pThis->OnDropFiles((HDROP) wParam);
	}
	return CallWindowProc(pThis->m_oldListProc, hWnd, uMessage, wParam, lParam);
}

void CPropsDlg::OnDropFiles( HDROP hDrop )
{
	int cnt = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	for(int i=0; i < cnt; i++)
	{
		FILE_TO_RUN fl;
		ZeroMemory(&fl, sizeof(FILE_TO_RUN));
		int sz = DragQueryFile(hDrop, i, NULL, 0);
		if(sz > 0)
		{
			fl.fileName = new TCHAR[sz + 1];
			fl.workingDir = new TCHAR[sz + 1];
			DragQueryFile(hDrop, i, fl.fileName, sz + 1);
			lstrcpy(fl.workingDir, fl.fileName);
			PathRemoveFileSpec(fl.workingDir);
			PathRemoveBackslash(fl.workingDir);
			m_progs.Add(&fl);
		}
	}
	FillList(m_ctlFiles->getSelectedIndex());
}

BOOL CPropsDlg::OnAdd()
{
	CAddProgDlg dlg(m_engine);
	if(dlg.DoModal(m_hWnd) == IDOK)
	{
		FILE_TO_RUN fl;
		ZeroMemory(&fl, sizeof(FILE_TO_RUN));
		MAKE_STR(fl.fileName,	dlg.m_fl.fileName);
		MAKE_STR(fl.params,		dlg.m_fl.params);
		MAKE_STR(fl.workingDir, dlg.m_fl.workingDir);
		m_progs.Add(&fl);
		FillList(m_progs.GetCount() - 1);
	}
	return TRUE;
}

BOOL CPropsDlg::OnEdit()
{
	int idx = m_ctlFiles->getSelectedIndex();
	if(idx >= 0)
	{
		CAddProgDlg dlg(m_engine);
		MAKE_STR(dlg.m_fl.fileName,		m_progs[idx].fileName);
		MAKE_STR(dlg.m_fl.params,		m_progs[idx].params);
		MAKE_STR(dlg.m_fl.workingDir,	m_progs[idx].workingDir);
		if(dlg.DoModal(m_hWnd) == IDOK)
		{
			MAKE_STR(m_progs[idx].fileName,		dlg.m_fl.fileName);
			MAKE_STR(m_progs[idx].params,		dlg.m_fl.params);
			MAKE_STR(m_progs[idx].workingDir,	dlg.m_fl.workingDir);
			FillList(idx);
		}
	}
	return TRUE;
}

BOOL CPropsDlg::OnDel()
{
	int idx = m_ctlFiles->getSelectedIndex();
	if(idx >= 0)
	{
		if(MessageBox(m_hWnd, m_engine->getStringDef(XUI_MSG_DELFILE), m_engine->getStringDef(XUI_STR_APPNAME), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			m_progs.Delete(idx);
			if(idx >= m_progs.GetCount())
			{
				idx = m_progs.GetCount() - 1;
			}
			FillList(idx);
		}
	}
	return TRUE;
}

BOOL CPropsDlg::OnMoveUp()
{
	int idx = m_ctlFiles->getSelectedIndex();
	if(idx > 0)
	{
		FILE_TO_RUN fl = m_progs[idx - 1];
		m_progs[idx - 1] = m_progs[idx];
		m_progs[idx] = fl;
		FillList(idx - 1);
	}
	return TRUE;
}

BOOL CPropsDlg::OnMoveDown()
{
	int idx = m_ctlFiles->getSelectedIndex();
	if(idx < m_progs.GetCount() - 1)
	{
		FILE_TO_RUN fl = m_progs[idx + 1];
		m_progs[idx + 1] = m_progs[idx];
		m_progs[idx] = fl;
		FillList(idx + 1);
	}
	return TRUE;
}

