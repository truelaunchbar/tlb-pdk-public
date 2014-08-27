#include "globals.h"
#include ".\fileslist.h"

CFilesList::CFilesList(void)
{
}

CFilesList::~CFilesList(void)
{
	Clear();
}

void CFilesList::Clear()
{
	for(int i=0; i < GetCount(); i++)
	{
		FREE_CLEAR_STR((*this)[i].fileName);
		FREE_CLEAR_STR((*this)[i].params);
		FREE_CLEAR_STR((*this)[i].workingDir);
	}
	CTArray<FILE_TO_RUN>::Clear();
}

CFilesList& CFilesList::operator=(CFilesList& val)
{
	Clear();
	for(int i=0; i < val.GetCount(); i++)
	{
		FILE_TO_RUN fl;
		ZeroMemory(&fl, sizeof(FILE_TO_RUN));
		MAKE_STR(fl.fileName,	val[i].fileName);
		MAKE_STR(fl.params,		val[i].params);
		MAKE_STR(fl.workingDir,	val[i].workingDir);
		Add(&fl);
	}
	return *this;
}

void CFilesList::Delete(int idx)
{
	if(idx >= 0 && idx < GetCount())
	{
		FILE_TO_RUN fl = (*this)[idx];
		CTArray<FILE_TO_RUN>::Delete(idx);
		FREE_CLEAR_STR(fl.fileName);
		FREE_CLEAR_STR(fl.params);
		FREE_CLEAR_STR(fl.workingDir);
	}
}
