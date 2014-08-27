#include "globals.h"

void Version2Str(DWORD versionMS, DWORD versionLS, LPTSTR str)
{
	DWORD ver[4];
	ver[0] = HIWORD(versionMS);
	ver[1] = LOWORD(versionMS);
	ver[2] = HIWORD(versionLS);
	ver[3] = LOWORD(versionLS);
	int lastNum = 3;
	for(int i=3; i >=0; i--)
	{
		if(ver[i])
		{
			lastNum = i;
			break;
		}
	}
	if(lastNum < 1) lastNum = 1;
	str[0] = 0;
	for(int i=0; i <= lastNum; i++)
	{
		TCHAR num[100];
		_itot(ver[i], num, 10);
		lstrcat(str, num);
		if(i != lastNum)
		{
			lstrcat(str, TEXT("."));
		}
	}
}

void GetFileVersion(LPCTSTR fileName, DWORD& versionMS, DWORD& versionLS)
{
	versionMS = 0;
	versionLS = 0;
	DWORD dw;
	DWORD verSize = GetFileVersionInfoSize((LPTSTR) fileName, &dw);
	if(verSize)
	{
		void* verData = malloc(verSize);
		if(GetFileVersionInfo((LPTSTR) fileName, NULL, verSize, verData))
		{
			VS_FIXEDFILEINFO* verInfo;
			UINT len = 0;
			if(VerQueryValue(verData, TEXT("\\"), (void**) &verInfo, &len))
			{
				versionMS = verInfo->dwFileVersionMS;
				versionLS = verInfo->dwFileVersionLS;
			}
		}
		free(verData);
	}
}
