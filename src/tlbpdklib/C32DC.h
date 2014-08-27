#pragma once

namespace tlbpdklib
{
	class C32DC
	{
		int		m_width;
		int		m_height;
		HDC		m_memDC;
		HBITMAP	m_bmp;
		HBITMAP	m_oldBmp;
		LPVOID	m_bits;
		HDC		m_hTargetDC;
		POINT	m_oldViewPort;
		RECT	m_rcTarget;
	public:
		C32DC();
		virtual ~C32DC(void);

		HDC			beginPaint(HDC hdc, LPRECT rcDraw);
		void		endPaint();
		RGBQUAD*	getBits()	{ return (RGBQUAD*) m_bits; }
	};
};
