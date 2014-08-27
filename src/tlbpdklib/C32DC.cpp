#include <Windows.h>
#include "C32DC.h"

namespace tlbpdklib
{
	C32DC::C32DC()
	{
		m_width		= 0;
		m_height	= 0;
		m_memDC		= NULL;
		m_bmp		= NULL;
		m_oldBmp	= NULL;
		m_bits		= NULL;
		m_hTargetDC	= NULL;
	}

	C32DC::~C32DC(void)
	{
		if(m_memDC && m_oldBmp)
		{
			SelectObject(m_memDC, m_oldBmp);
		}
		if(m_memDC)
		{
			DeleteDC(m_memDC);
		}
		if(m_bmp)
		{
			DeleteObject(m_bmp);
		}
	}

	HDC C32DC::beginPaint( HDC hdc, LPRECT rcDraw )
	{
		m_width		= rcDraw->right - rcDraw->left;
		m_height	= rcDraw->bottom- rcDraw->top;

		BITMAPINFO bmp_info; 
		bmp_info.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER); 
		bmp_info.bmiHeader.biWidth			= m_width; 
		bmp_info.bmiHeader.biHeight			= -m_height; 
		bmp_info.bmiHeader.biPlanes			= 1; 
		bmp_info.bmiHeader.biBitCount		= 32; 
		bmp_info.bmiHeader.biCompression	= BI_RGB; 
		bmp_info.bmiHeader.biSizeImage		= 0; 
		bmp_info.bmiHeader.biXPelsPerMeter	= 0; 
		bmp_info.bmiHeader.biYPelsPerMeter	= 0; 
		bmp_info.bmiHeader.biClrUsed		= 0; 
		bmp_info.bmiHeader.biClrImportant	= 0; 

		m_memDC = CreateCompatibleDC(NULL); 

		m_bmp = ::CreateDIBSection( 
			m_memDC, 
			&bmp_info, 
			DIB_RGB_COLORS, 
			&m_bits, 
			0, 
			0 
			);
		if(m_bits)
		{
			ZeroMemory(m_bits, m_width * m_height * 4);

			m_oldBmp = (HBITMAP)::SelectObject(m_memDC, m_bmp);

			m_hTargetDC = hdc;
			m_rcTarget	= *rcDraw;
			SetWindowOrgEx(m_memDC, rcDraw->left, rcDraw->top, &m_oldViewPort);

			return m_memDC;
		} else
		{
			DeleteDC(m_memDC);
			m_memDC = NULL;
		}
		return NULL;
	}

	void C32DC::endPaint()
	{
		BOOL draw = TRUE;

		SetWindowOrgEx(m_memDC, m_oldViewPort.x, m_oldViewPort.y, NULL);

		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.AlphaFormat = AC_SRC_ALPHA;
		bf.SourceConstantAlpha = 255;

		AlphaBlend(m_hTargetDC, m_rcTarget.left, m_rcTarget.top,
			m_rcTarget.right - m_rcTarget.left,
			m_rcTarget.bottom - m_rcTarget.top, m_memDC,
			0, 0,
			m_rcTarget.right	- m_rcTarget.left,
			m_rcTarget.bottom	- m_rcTarget.top,
			bf);

		SelectObject(m_memDC, m_oldBmp);
		DeleteDC(m_memDC);
		DeleteObject(m_bmp);

		m_memDC		= NULL;
		m_bmp		= NULL;
		m_oldBmp	= NULL;
	}
}
