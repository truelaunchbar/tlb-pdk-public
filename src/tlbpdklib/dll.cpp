#include "tlbpdkcore.h"
#include "clsfact.h"

/**************************************************************************
global variables
**************************************************************************/

HINSTANCE   g_hResInst;
HINSTANCE   g_hInst;
UINT        g_DllRefCount;


/**************************************************************************

DllMain

**************************************************************************/

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	switch(ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hInst = (HINSTANCE) hModule;
		g_hResInst = (HINSTANCE) hModule;
		InitDLL();
		break;
	case DLL_PROCESS_DETACH:
		if(g_hResInst && g_hResInst != g_hInst)
		{
			FreeLibrary(g_hResInst);
			g_hResInst = g_hInst;
		}
		UninitDLL();
		break;
	}

	return TRUE;
}

/**************************************************************************

DllRegisterServer

**************************************************************************/


STDAPI DllRegisterServer(void)
{
	if(RegisterPlugin(g_hInst, *GetCLSID(), GetPluginName()) != S_OK)
	{
		return SELFREG_E_CLASS;
	}
	return S_OK;
}

/**************************************************************************

DllUnregisterServer

**************************************************************************/

STDAPI DllUnregisterServer(void)
{
	if(!UnregisterPlugin(*GetCLSID()))
		return SELFREG_E_CLASS;

	return S_OK;
}


/**************************************************************************

DllCanUnloadNow

**************************************************************************/

STDAPI DllCanUnloadNow(void)
{
	return (g_DllRefCount ? S_FALSE : S_OK);
}

/**************************************************************************

DllGetClassObject

**************************************************************************/

STDAPI DllGetClassObject(  REFCLSID rclsid, 
						 REFIID riid, 
						 LPVOID *ppReturn)
{
	*ppReturn = NULL;

	//if we don't support this classid, return the proper error code
	if(!IsEqualCLSID(rclsid, *GetCLSID()))
		return CLASS_E_CLASSNOTAVAILABLE;

	//create a CClassFactory object and check it for validity
	CClassFactory *pClassFactory = new CClassFactory(rclsid);
	if(NULL == pClassFactory)
		return E_OUTOFMEMORY;

	//get the QueryInterface return for our return value
	HRESULT hResult = pClassFactory->QueryInterface(riid, ppReturn);

	//call Release to decement the ref count - creating the object set it to one 
	//and QueryInterface incremented it - since its being used externally (not by 
	//us), we only want the ref count to be 1
	pClassFactory->Release();

	//return the result from QueryInterface
	return hResult;
}

BOOL isVista()
{
	DWORD addStyle = 0;
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if((bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi)))
	{
		if(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			if(osvi.dwMajorVersion >= 6)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}
