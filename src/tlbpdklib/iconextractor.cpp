#include "tlbpdkcore.h"
#include <stdio.h>
#include <Wownt16.h>
#include <shlwapi.h>

struct EMUN_ICONS
{
	int index;
	TCHAR* name;
};

BOOL CALLBACK EnumIcons(
  HMODULE hModule,   // module handle
  LPCTSTR lpszType,  // resource type
  LPTSTR lpszName,   // resource name
  LONG_PTR lParam    // application-defined parameter
)
{
	EMUN_ICONS* eic = (EMUN_ICONS*) lParam;
	if(!eic->index)
	{
		if(((LONG)lpszName) < 65536)
		{
			eic->name = lpszName;
		} else
		{
			eic->name = new TCHAR [lstrlen(lpszName) + 1];
			lstrcpy(eic->name, lpszName);
		}
		return FALSE;
	}
	eic->index --;
	return TRUE;
}

typedef UINT (WINAPI *fPrivateExtractIconsW)(
	IN LPCWSTR szFileName,
	IN int      nIconIndex,
	IN int      cxIcon,
	IN int      cyIcon,
	OUT HICON   *phicon,
	OUT UINT    *piconid,
	IN UINT     nIcons,
	IN UINT     flags);

HICON ExtractIconFromFile(LPCTSTR flName, DWORD index, DWORD IconSize)
{
	TCHAR fileName[MAX_PATH];
	fileName[0] = 0;
	if(!StrCmpI(PathFindFileName(flName), TEXT("shell32.dll")))
	{
		HKEY key = NULL;
		if(RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Icons"), &key) == ERROR_SUCCESS)
		{
			TCHAR valName[10];
			wsprintf(valName, TEXT("%d"), index);
			TCHAR substFile[MAX_PATH];
			substFile[0] = 0;
			DWORD type = REG_SZ;
			DWORD size = sizeof(substFile);
			if(RegQueryValueEx(key, valName, NULL, &type, (LPBYTE) substFile, &size)  == ERROR_SUCCESS)
			{
				TCHAR icFile[MAX_PATH];
				DWORD icIDX = PathParseIconLocation(substFile);
				ExpandEnvironmentStrings(substFile, icFile, MAX_PATH);
				if(PathFileExists(icFile))
				{
					lstrcpy(fileName, icFile);
					index = icIDX;
				}
			}
			RegCloseKey(key);
		}
	}
	if(!fileName[0])
	{
		ExpandEnvironmentStrings(flName, fileName, MAX_PATH);
	}
	HICON hIc = NULL;
	// try to use PrivateExtractIcons
	{
		HMODULE user32 = LoadLibrary(TEXT("user32.dll"));
		if(user32)
		{
			fPrivateExtractIconsW pei = (fPrivateExtractIconsW) GetProcAddress(user32, "PrivateExtractIconsW");
			if(pei)
			{
				UINT icID;
				pei(fileName, index, IconSize, IconSize, &hIc, &icID, 1, LR_DEFAULTCOLOR | LR_LOADFROMFILE/* | LR_LOADTRANSPARENT*/);
			}
			FreeLibrary(user32);
		}
	}
	if(!hIc)
	{
		HMODULE hMod = LoadLibraryEx(fileName, NULL, LOAD_LIBRARY_AS_DATAFILE | LOAD_WITH_ALTERED_SEARCH_PATH);
		if(hMod)
		{
			int idx = (int) index;
			__try
			{
				if(idx >= 0)
				{
					EMUN_ICONS eic;
					eic.index = index;
					eic.name = NULL;
					EnumResourceNames(hMod, RT_GROUP_ICON, EnumIcons, (LONG) &eic);
					if(eic.name)
					{
						HRSRC hResource = FindResource(hMod, eic.name, RT_GROUP_ICON);
						if(hResource)
						{
							HGLOBAL hMem = LoadResource(hMod, hResource);
							PBYTE lpResource = (PBYTE) LockResource(hMem);
							int iconID = LookupIconIdFromDirectoryEx(lpResource, TRUE, IconSize, IconSize, LR_DEFAULTCOLOR);
							
							hResource = FindResource(hMod, MAKEINTRESOURCE(iconID), MAKEINTRESOURCE(RT_ICON));
							hMem = LoadResource(hMod, hResource);
							lpResource = (PBYTE) LockResource(hMem);
							hIc = CreateIconFromResourceEx(lpResource, SizeofResource(hMod, hResource), TRUE, 0x00030000, IconSize, IconSize, LR_DEFAULTCOLOR);
						}
					}
					if(((LONG)eic.name) >= 65536)
					{
						delete eic.name;
					}
				} else
				{
					HRSRC hResource = FindResource(hMod, MAKEINTRESOURCE(-idx), RT_GROUP_ICON);
					if(hResource)
					{
						HGLOBAL hMem = LoadResource(hMod, hResource);
						PBYTE lpResource = (PBYTE) LockResource(hMem);
						int iconID = LookupIconIdFromDirectoryEx(lpResource, TRUE, IconSize, IconSize, LR_DEFAULTCOLOR);
						
						hResource = FindResource(hMod, MAKEINTRESOURCE(iconID), MAKEINTRESOURCE(RT_ICON));
						hMem = LoadResource(hMod, hResource);
						lpResource = (PBYTE) LockResource(hMem);
						hIc = CreateIconFromResourceEx(lpResource, SizeofResource(hMod, hResource), TRUE, 0x00030000, IconSize, IconSize, LR_DEFAULTCOLOR);
					}
				}
			} __except(1)
			{
				hIc = NULL;
			}
			FreeLibrary(hMod);
		}
	}
	if(!hIc)
	{
		hIc = (HICON) LoadImage(NULL, fileName, IMAGE_ICON, IconSize, IconSize, LR_DEFAULTCOLOR | LR_LOADFROMFILE);
		if(!hIc)
		{
			HICON lgIc = NULL;
			HICON smIc = NULL;
			ExtractIconEx(fileName, index, &lgIc, &smIc, 1);
			if(IconSize >= 32)
			{
				if(lgIc)
				{
					hIc = lgIc;
					if(smIc) DestroyIcon(smIc);
				} else
				{
					hIc = smIc;
				}
			} else
			{
				if(smIc)
				{
					hIc = smIc;
					if(lgIc) DestroyIcon(lgIc);
				} else
				{
					hIc = lgIc;
				}
			}
		}
	}
	return hIc;
}
