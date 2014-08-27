#pragma once

class CBREngine : public CXUIEngine
{
public:
	CBREngine(HINSTANCE hInst);
	virtual ~CBREngine(void);

	virtual void	DrawImage(HDC hdc, int x, int y, int width, int height, LPWSTR imgUrl);
	virtual void	DrawFrame(HDC hdc, int x, int y, int width, int height, LPWSTR imgUrl, int frames, int frame, int framesOrient);
};
