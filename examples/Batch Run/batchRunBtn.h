#pragma once

#include "arrays.h"
#include "FilesList.h"
#include "BREngine.h"

#define HOTKEY_RUN		1

class CBatchRunBtn :	public CTlbButton
{
	HICON			m_tipIcon;
	WORD			m_hotKey;
	CFilesList		m_progs;
	CBREngine		m_xui;
public:
	CBatchRunBtn(void);
	virtual ~CBatchRunBtn(void);

	virtual BOOL   OnCreate();

	BOOL openProperties();
	virtual BOOL   OnSetLCID(DWORD dwLCID, HMODULE hInstance);
	virtual BOOL   LoadIcon(int size);
	virtual BOOL   SetIconLocation(LPCWSTR szLocation);
	virtual HICON  GetTipIcon(int tipID);
	virtual LPWSTR GetTipCaption(int tipID);
	virtual LPWSTR GetTipText(int tipID);
	virtual BOOL   Load(IStream *data);
	virtual BOOL   Save(IStream *data);
	virtual UINT   GetSupportedActions(void);
	virtual UINT   GetModeFlags(void);
	virtual BOOL   OnProperties();
	virtual BOOL   OnRun();
	virtual void   OnHotkey(UINT hkID);
	virtual int    GetHotkeysCount(void);
	virtual BOOL   GetHotKeyData(int idx, HOTKEY_DATA* hkData);
};
