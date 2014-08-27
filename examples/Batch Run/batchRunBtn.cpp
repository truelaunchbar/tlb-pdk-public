#include "globals.h"
#include ".\batchrunbtn.h"
#include "PropsDlg.h"
#include "PluginStream.h"
#include "xuistrings.h"

CBatchRunBtn::CBatchRunBtn(void) : m_xui(g_hInst)
{
	m_tipIcon	= NULL;
	m_hotKey	= 0;
}

CBatchRunBtn::~CBatchRunBtn(void)
{
	if(m_tipIcon)
	{
		DestroyIcon(m_tipIcon);
		m_tipIcon = NULL;
	}
	m_progs.Clear();
}

BOOL CBatchRunBtn::OnCreate()
{
	return openProperties();
}

BOOL CBatchRunBtn::OnSetLCID(DWORD dwLCID, HMODULE hInstance)
{
	TCHAR szLCID[20];
	wsprintf(szLCID, TEXT("%d"), dwLCID);
	BOOL ret = CTlbButton::OnSetLCID(dwLCID, hInstance);

	TCHAR langPath[MAX_PATH];
	LONG cbData = sizeof(langPath);
	RegQueryValue(HKEY_CLASSES_ROOT, TEXT("CLSID\\{FC712CA0-A945-11D4-A594-956F6349FC18}\\InprocServer32"), langPath, &cbData);
	PathRemoveFileSpec(langPath);
	PathAddBackslash(langPath);
	lstrcat(langPath, TEXT("langs\\"));
	lstrcat(langPath, szLCID);
	lstrcat(langPath, TEXT("\\batchrun.xml"));
	m_xui.clearStrings();
	m_xui.loadStrings(langPath);

	return ret;
}

BOOL CBatchRunBtn::LoadIcon(int size)
{
	if(!CTlbButton::LoadIcon(size))
	{
		m_hIcon = (HICON) LoadImage(g_hInst, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, size, size, LR_DEFAULTCOLOR);
		m_loadedIconSize = size;
	}
	return m_hIcon ? TRUE : FALSE;
}

BOOL CBatchRunBtn::SetIconLocation(LPCWSTR szLocation)
{
	if(m_tipIcon) DestroyIcon(m_tipIcon);
	m_tipIcon = NULL;
	if(m_hIcon) DestroyIcon(m_hIcon);
	m_hIcon = NULL;
	if(m_pngIcon)
	{
		delete m_pngIcon;
		m_pngIcon = NULL;
	}
	return CTlbButton::SetIconLocation(szLocation);
}

HICON CBatchRunBtn::GetTipIcon(int tipID)
{
	if(!m_tipIcon)
	{
		m_tipIcon = (HICON) LoadImage(g_hInst, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_LOADTRANSPARENT);
	}
	return m_tipIcon;
}

LPWSTR CBatchRunBtn::GetTipCaption(int tipID)
{
	LPWSTR name = (LPWSTR) CoTaskMemAlloc((lstrlen(m_Name) + 1) * sizeof(TCHAR));
	lstrcpy(name, m_Name);
	return name;
}

LPWSTR CBatchRunBtn::GetTipText(int tipID)
{
	std::wstring strTip;

	for(int i = 0; i < m_progs.GetCount(); i++)
	{
		WCHAR out[MAX_PATH];
		PathCompactPathEx(out, m_progs[i].fileName, 50, 0);
		if(i)
		{
			strTip += L"<br>";
		}
		strTip += out;
	}

	LPWSTR name = (LPWSTR) CoTaskMemAlloc((strTip.length() + 1) * sizeof(WCHAR));
	lstrcpy(name, strTip.c_str());
	return name;
}

BOOL CBatchRunBtn::Load(IStream *data)
{ 
	CPluginStream stream;
	if(stream.Load(data))
	{
		if(stream.OpenNode(TEXT("settings")))
		{
			MAKE_STR(m_iconLocation, stream.GetString(TEXT("iconLocation")));
			m_hotKey = stream.GetWORD(TEXT("runhotkey"), 0);
			stream.CloseNode();

			if(m_hotKey)
			{
				m_container->SetHotKey(m_hotKey, HOTKEY_RUN);
			}
		}
		if(stream.OpenNode(TEXT("progs")))
		{
			int cnt = stream.GetDWORD(TEXT("count"), 0);
			for(int i=0; i < cnt; i++)
			{
				TCHAR node[255];
				wsprintf(node, TEXT("app#%d"), i);
				if(stream.OpenNode(node))
				{
					FILE_TO_RUN fl;
					ZeroMemory(&fl, sizeof(FILE_TO_RUN));
					MAKE_STR(fl.fileName,	stream.GetString(TEXT("path")));
					MAKE_STR(fl.params,		stream.GetString(TEXT("params")));
					MAKE_STR(fl.workingDir, stream.GetString(TEXT("workDir")));
					m_progs.Add(&fl);

					stream.CloseNode();
				}
			}

			stream.CloseNode();
		}
	}

	return TRUE; 
}

BOOL CBatchRunBtn::Save(IStream *data)
{ 
	CPluginStream stream;
	stream.BeginNode(TEXT("settings"));
		stream.SaveString(TEXT("iconLocation"),	m_iconLocation);
		stream.SaveWORD(TEXT("runhotkey"), m_hotKey);
	stream.EndNode();

	stream.BeginNode(TEXT("progs"));
		stream.SaveDWORD(TEXT("count"), m_progs.GetCount());
		for(int i=0; i < m_progs.GetCount(); i++)
		{
			TCHAR node[255];
			wsprintf(node, TEXT("app#%d"), i);
			stream.BeginNode(node);
				if(m_progs[i].fileName)		stream.SaveString(TEXT("path"),		m_progs[i].fileName);
				if(m_progs[i].params)		stream.SaveString(TEXT("params"),	m_progs[i].params);
				if(m_progs[i].workingDir)	stream.SaveString(TEXT("workDir"),	m_progs[i].workingDir);
			stream.EndNode();
		}
	stream.EndNode();

	stream.Save(data);
	return TRUE; 
}

UINT CBatchRunBtn::GetSupportedActions(void)
{
	return CTMS_PROPERTIES | CTMS_CHANGEICON | CTMS_RESETICON;
}

UINT CBatchRunBtn::GetModeFlags(void)
{
	return BTN_FLAG_SUPPORTSAVEDICON | BTN_FLAG_SUPPORTPNGICONS;
}

BOOL CBatchRunBtn::OnProperties()
{
	m_container->SetHotKey(0, HOTKEY_RUN);
	if(openProperties())
	{
		m_container->SetHotKey(m_hotKey, HOTKEY_RUN);
		m_container->UpdateTips();
		m_container->SaveButton();
		return TRUE;
	}
	m_container->SetHotKey(m_hotKey, HOTKEY_RUN);
	return FALSE;
}

BOOL CBatchRunBtn::OnRun()
{
	for(int i=0; i < m_progs.GetCount(); i++)
	{
		ShellExecute(NULL, NULL, m_progs[i].fileName, m_progs[i].params, m_progs[i].workingDir, SW_SHOWDEFAULT);
	}
	return TRUE;
}

void CBatchRunBtn::OnHotkey(UINT hkID)
{
	switch(hkID)
	{
	case HOTKEY_RUN:
		OnRun();
		break;
	}
}

int CBatchRunBtn::GetHotkeysCount(void)
{
	return 1;
}

BOOL CBatchRunBtn::GetHotKeyData(int idx, HOTKEY_DATA* hkData)
{
	switch(idx)
	{
	case 0:
		hkData->hotKey = m_hotKey;
		hkData->hotKeyID = HOTKEY_RUN;
		hkData->id = HOTKEY_RUN;
		StringCchCopy(hkData->name, 260, m_xui.getStringDef(XUI_STR_HOTKEYNAME));
		StringCchCat(hkData->name, 260, L": ");
		StringCchCat(hkData->name, 260, m_Name);
		break;
	}
	return TRUE;
}

BOOL CBatchRunBtn::openProperties()
{
	CPropsDlg dlg(&m_xui);
	dlg.m_hotKey	= m_hotKey;
	dlg.m_progs		= m_progs;
	if(dlg.DoModal(m_container->GetToolbarWindow()) == IDOK)
	{
		m_progs		= dlg.m_progs;
		m_hotKey	= dlg.m_hotKey;
		return TRUE;
	}
	return FALSE;
}
