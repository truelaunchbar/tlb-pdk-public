#pragma once

#include "arrays.h"

class CFilesList : public CTArray<FILE_TO_RUN>
{
public:
	CFilesList(void);
	~CFilesList(void);

	CFilesList& operator=(CFilesList& val);
	void Delete(int idx);

	void Clear();
};
