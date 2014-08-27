#pragma once
#include "resource.h"
#include "arrays.h"
#include "FilesList.h"

class CPropsDlg : public CXUIDialog
{
	CXUIList*	m_ctlFiles;
	WNDPROC		m_oldListProc;
public:
	WORD			m_hotKey;
	CFilesList		m_progs;

	CPropsDlg(CXUIEngine* engine);
	virtual ~CPropsDlg(void);

	virtual void OnInitDialog();
	virtual BOOL OnEndDialog(UINT code);

	XUI_BEGIN_DATA_MAP
		XUI_DATA_MAP_WORD	(L"hotkey",	m_hotKey)
	XUI_END_DATA_MAP

	XUI_BEGIN_BIND_MAP
		XUI_BIND_ITEM	(L"files",	L"list",	m_ctlFiles)
	XUI_END_BIND_MAP

	XUI_BEGIN_EVENT_MAP
		XUI_HANDLE_CLICKED	(L"btnAdd",			OnAdd)
		XUI_HANDLE_CLICKED	(L"btnEdit",		OnEdit)
		XUI_HANDLE_CLICKED	(L"btnDel",			OnDel)
		XUI_HANDLE_CLICKED	(L"btnMoveUp",		OnMoveUp)
		XUI_HANDLE_CLICKED	(L"btnMoveDown",	OnMoveDown)
	XUI_END_EVENT_MAP

private:
	static LRESULT CALLBACK ListWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

	void FillList(int activeIDX);
	void OnDropFiles(HDROP hDrop);
	
	BOOL OnAdd();
	BOOL OnEdit();
	BOOL OnDel();
	BOOL OnMoveUp();
	BOOL OnMoveDown();
};
