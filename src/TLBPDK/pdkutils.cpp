#include <tlbpdk.h>
#include <shlwapi.h>

CATID CATID_TLBBUTTONS = { /* 49e86696-eaa6-45a2-9480-c78c013b9713 */
    0x49e86696,
    0xeaa6,
    0x45a2,
    {0x94, 0x80, 0xc7, 0x8c, 0x01, 0x3b, 0x97, 0x13}
  };

/**************************************************************************

   RegisterComCat

**************************************************************************/

BOOL PDKRegisterComCat(CLSID clsid, CATID CatID)
{
	ICatRegister   *pcr;
	HRESULT        hr = S_OK ;
    
	CoInitialize(NULL);

	hr = CoCreateInstance(  CLSID_StdComponentCategoriesMgr, 
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_ICatRegister, 
							(LPVOID*)&pcr);

	if(SUCCEEDED(hr))
	{
		hr = pcr->RegisterClassImplCategories(clsid, 1, &CatID);
		pcr->Release();
	}
        
	CoUninitialize();
	return SUCCEEDED(hr);
}

/**************************************************************************

   RegisterServer

**************************************************************************/

typedef struct{
   HKEY  hRootKey;
   LPTSTR szSubKey;
   LPTSTR lpszValueName;
   LPTSTR szData;
}DOREGSTRUCT, *LPDOREGSTRUCT;

BOOL RegisterServer(HINSTANCE g_hInst, CLSID clsid, LPTSTR lpszTitle)
{
	int      i;
	HKEY     hKey;
	LRESULT  lResult;
	DWORD    dwDisp;
	TCHAR    szSubKey[MAX_PATH];
	TCHAR    szCLSID[MAX_PATH];
	TCHAR    szModule[MAX_PATH];
	LPWSTR   pwsz;

	//get the CLSID in string form
	StringFromIID(clsid, &pwsz);

	if(pwsz)
	{
	#ifdef UNICODE
		lstrcpy(szCLSID, pwsz);
	#else
		WideCharToMultiByte( CP_ACP,
							0,
							pwsz,
							-1,
							szCLSID,
							TLB_ARRAYSIZE(szCLSID),
							NULL,
							NULL);
	#endif
	   //free the string
		LPMALLOC pMalloc;
		CoGetMalloc(1, &pMalloc);
		pMalloc->Free(pwsz);
		pMalloc->Release();
	}

	//get this app's path and file name
	GetModuleFileName(g_hInst, szModule, TLB_ARRAYSIZE(szModule));

	DOREGSTRUCT ClsidEntries[] = {HKEY_CLASSES_ROOT,   TEXT("CLSID\\%s"),                  NULL,                   lpszTitle,
								  HKEY_CLASSES_ROOT,   TEXT("CLSID\\%s\\InprocServer32"),  NULL,                   szModule,
								  HKEY_CLASSES_ROOT,   TEXT("CLSID\\%s\\InprocServer32"),  TEXT("ThreadingModel"), TEXT("Apartment"),
								  HKEY_LOCAL_MACHINE,   TEXT("Software\\True Software\\TrueLaunchBar\\plugins\\%s"),NULL, lpszTitle,
								  HKEY_LOCAL_MACHINE,   TEXT("Software\\True Software\\TrueLaunchBar\\plugins\\%s\\Langs"),NULL, NULL,
								  NULL,                NULL,                               NULL,                   NULL};

	//register the CLSID entries
	for(i = 0; ClsidEntries[i].hRootKey; i++)
	{
		//create the sub key string - for this case, insert the file extension
		wsprintf(szSubKey, ClsidEntries[i].szSubKey, szCLSID);

		lResult = RegCreateKeyEx(  ClsidEntries[i].hRootKey,
								  szSubKey,
								  0,
								  NULL,
								  REG_OPTION_NON_VOLATILE,
								  KEY_WRITE,
								  NULL,
								  &hKey,
								  &dwDisp);
   
		if(NOERROR == lResult && ClsidEntries[i].szData)
		{
			TCHAR szData[MAX_PATH];
			//if necessary, create the value string
			wsprintf(szData, ClsidEntries[i].szData, szModule);
   
			lResult = RegSetValueEx(   hKey,
									 ClsidEntries[i].lpszValueName,
									 0,
									 REG_SZ,
									 (LPBYTE)szData,
									 lstrlen(szData) + 1);
      
			RegCloseKey(hKey);
		}
	}

	return TRUE;
}

BOOL RegisterPlugin(HINSTANCE g_hInst, CLSID clsid, LPSTR lpszTitle)
{
	if(!RegisterServer(g_hInst, clsid, lpszTitle))
	   return SELFREG_E_CLASS;
	if(!PDKRegisterComCat(clsid, CATID_TLBBUTTONS))
	   return SELFREG_E_CLASS;
	return S_OK;
}

BOOL UnregisterPlugin(CLSID clsid)
{
	TCHAR    szSubKey[MAX_PATH];
	TCHAR    szCLSID[MAX_PATH];
	LPWSTR   pwsz;

	//get the CLSID in string form
	StringFromIID(clsid, &pwsz);

	if(pwsz)
	{
	#ifdef UNICODE
		lstrcpy(szCLSID, pwsz);
	#else
		WideCharToMultiByte( CP_ACP,
							0,
							pwsz,
							-1,
							szCLSID,
							TLB_ARRAYSIZE(szCLSID),
							NULL,
							NULL);
	#endif
	   //free the string
		LPMALLOC pMalloc;
		CoGetMalloc(1, &pMalloc);
		pMalloc->Free(pwsz);
		pMalloc->Release();
	}

    wsprintf(szSubKey, "CLSID\\%s", szCLSID);
	SHDeleteKey(HKEY_CLASSES_ROOT, szSubKey);

	return TRUE;
}
