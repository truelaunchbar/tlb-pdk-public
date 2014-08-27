#pragma once

class CTimerXUI : public CXUIEngine
{
public:
	CTimerXUI(void);
	virtual ~CTimerXUI(void);

	virtual void DrawImage(HDC hdc, int x, int y, int width, int height, LPWSTR imgUrl);
};
