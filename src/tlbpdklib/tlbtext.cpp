#include <Windows.h>
#include <uxtheme.h>
#include "tlbtext.h"
#include "C32DC.h"

namespace tlbpdklib
{

	typedef HRESULT (WINAPI *fncDrawThemeTextEx)(
		HTHEME hTheme,
		HDC hdc,
		int iPartId,
		int iStateId,
		__in_ecount(cchText) LPCWSTR pszText,
		int cchText,
		DWORD dwTextFlags,
		__inout LPRECT pRect,
		__in_opt const DTTOPTS *pOptions
		);

	void fastbluralpha(LPRGBQUAD pixels, int width, int height, int radius)
	{
		if (radius < 1) {
			return;
		}

		LPRGBQUAD pix = pixels;
		int w   = width;
		int h   = height;
		int wm  = w - 1;
		int hm  = h - 1;
		int wh  = w * h;
		int div = radius + radius + 1;

		int *a = new int[wh];
		int asum, x, y, i, yp, yi, yw;
		int *vmin = new int[max(w,h)];

		int divsum = (div+1) >> 1;
		divsum *= divsum;
		int *dv = new int[256 * divsum];
		for (i=0; i < 256 * divsum; ++i) 
		{
			dv[i] = (i/divsum);
		}

		yw = yi = 0;

		int* stack = new int[div];


		int stackpointer;
		int stackstart;
		int *sir;
		int rbs;
		int r1 = radius + 1;
		int aoutsum;
		int ainsum;

		for (y = 0; y < h; ++y)
		{
			ainsum	= 0;
			aoutsum = 0;
			asum	= 0;
			for(i =- radius; i <= radius; ++i) 
			{
				sir = stack + i + radius;
				*sir = pix[yi + min(wm, max(i, 0))].rgbReserved;

				rbs = r1 - abs(i);
				asum += (*sir) * rbs;

				if (i > 0)
				{
					ainsum += *sir;
				} else 
				{
					aoutsum += *sir;
				}
			}
			stackpointer = radius;

			for (x=0; x < w; ++x) {

				a[yi] = dv[asum];

				asum -= aoutsum;

				stackstart = stackpointer - radius + div;
				sir = stack + (stackstart % div);

				aoutsum -= *sir;

				if (y == 0) 
				{
					vmin[x] = min(x + radius + 1, wm);
				}

				*sir = pix[yw + vmin[x]].rgbReserved;

				ainsum	+= *sir;
				asum	+= ainsum;

				stackpointer = (stackpointer + 1) % div;
				sir = stack + (stackpointer % div);

				aoutsum += *sir;

				ainsum -= *sir;

				++yi;
			}
			yw += w;
		}
		for (x=0; x < w; ++x)
		{
			ainsum = aoutsum = asum = 0;
			yp =- radius * w;

			for(i=-radius; i <= radius; ++i) 
			{
				yi = max(0, yp) + x;

				sir = stack + i + radius;

				*sir = a[yi];

				rbs = r1 - abs(i);

				asum += a[yi] * rbs;

				if (i > 0) 
				{
					ainsum += *sir;
				} else 
				{
					aoutsum += *sir;
				}

				if (i < hm)
				{
					yp += w;
				}
			}

			yi = x;
			stackpointer = radius;

			for (y=0; y < h; ++y)
			{
				pix[yi].rgbReserved	= dv[asum];

				asum -= aoutsum;

				stackstart = stackpointer - radius + div;
				sir = stack + (stackstart % div);

				aoutsum -= *sir;

				if (x==0)
				{
					vmin[y] = min(y + r1, hm) * w;
				}
				int p = x + vmin[y];

				*sir = a[p];

				ainsum += *sir;

				asum += ainsum;

				stackpointer = (stackpointer + 1) % div;
				sir = stack + stackpointer;

				aoutsum += *sir;
				ainsum -= *sir;

				yi += w;
			}
		}
		delete [] a;
		delete [] vmin;
		delete [] dv;
		delete stack;
	}

	void tlbDrawText( HDC hdc, LPCTSTR text, int c, LPRECT rcText, UINT params, HFONT hFont /*= NULL*/, COLORREF color /*= 0xFFFFFFFF*/, INT vertical /*= 0*/, BOOL drawGlow, COLORREF glowColor, INT glowSize, BOOL isTransparentDC)
	{
		if(!hdc)	return;
		if(!rcText)	return;
		if(rcText->right <= rcText->left || rcText->bottom <= rcText->top)	return;
		BOOL process = TRUE;
		if(c < 0)
		{
			c = lstrlen(text);
		}

		BOOL isCalc = FALSE;
		if(params & DT_CALCRECT)
		{
			isCalc = TRUE;
		}

		if(vertical)
		{
			if(isCalc)
			{
				RECT rcTemp;
				rcTemp.left		= rcText->top;
				rcTemp.right	= rcText->bottom;
				rcTemp.top		= rcText->left;
				rcTemp.bottom	= rcText->right;
				tlbDrawText(hdc, text, c, &rcTemp, params, hFont, color, 0, drawGlow, glowColor, glowSize, isTransparentDC);
				rcText->left	= rcTemp.top;
				rcText->right	= rcTemp.bottom;
				rcText->top		= rcTemp.left;
				rcText->bottom	= rcTemp.right;
			} else
			{
				int height  = rcText->right  - rcText->left;
				int width   = rcText->bottom - rcText->top;

				BITMAPINFO bmp_info; 
				bmp_info.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER); 
				bmp_info.bmiHeader.biWidth			= width; 
				bmp_info.bmiHeader.biHeight			= -height; 
				bmp_info.bmiHeader.biPlanes			= 1; 
				bmp_info.bmiHeader.biBitCount		= 32; 
				bmp_info.bmiHeader.biCompression	= BI_RGB; 
				bmp_info.bmiHeader.biSizeImage		= 0; 
				bmp_info.bmiHeader.biXPelsPerMeter	= 0; 
				bmp_info.bmiHeader.biYPelsPerMeter	= 0; 
				bmp_info.bmiHeader.biClrUsed		= 0; 
				bmp_info.bmiHeader.biClrImportant	= 0; 

				HDC memDC = CreateCompatibleDC(NULL); 
				if(!memDC) return;

				void* buf = 0; 
				HBITMAP bmp = ::CreateDIBSection( 
					memDC, 
					&bmp_info, 
					DIB_RGB_COLORS, 
					&buf, 
					0, 
					0 
					); 

				if(buf)
				{ 
					ZeroMemory(buf, width * height * 4);

					HBITMAP tempBmp = (HBITMAP)::SelectObject(memDC, bmp);
					RECT tempRect;
					tempRect.left	= 0;
					tempRect.right	= width;
					tempRect.top	= 0;
					tempRect.bottom = height;

					HFONT		fnt = hFont;
					COLORREF	clr = color;
					if(clr == 0xFFFFFFFF)
					{
						clr = GetTextColor(hdc);
					}
					if(!fnt)
					{
						fnt = (HFONT) GetCurrentObject(hdc, OBJ_FONT);
					}
					tlbDrawText(memDC, text, c, &tempRect, params, fnt, clr, 0, drawGlow, glowColor, glowSize, TRUE);

					int width_r		= height;
					int height_r	= width;
					BITMAPINFO bmp_info_r; 
					bmp_info_r.bmiHeader.biSize				= sizeof(BITMAPINFOHEADER); 
					bmp_info_r.bmiHeader.biWidth			= width_r; 
					bmp_info_r.bmiHeader.biHeight			= -height_r; 
					bmp_info_r.bmiHeader.biPlanes			= 1; 
					bmp_info_r.bmiHeader.biBitCount			= 32; 
					bmp_info_r.bmiHeader.biCompression		= BI_RGB; 
					bmp_info_r.bmiHeader.biSizeImage		= 0; 
					bmp_info_r.bmiHeader.biXPelsPerMeter	= 0; 
					bmp_info_r.bmiHeader.biYPelsPerMeter	= 0; 
					bmp_info_r.bmiHeader.biClrUsed			= 0; 
					bmp_info_r.bmiHeader.biClrImportant		= 0; 

					void* buf_r = 0; 
					HBITMAP bmp_r = ::CreateDIBSection( 
						memDC, 
						&bmp_info_r, 
						DIB_RGB_COLORS, 
						&buf_r, 
						0, 
						0 
						); 
					SelectObject(memDC, bmp_r);
					LPRGBQUAD src_pixels = (LPRGBQUAD) buf;
					LPRGBQUAD dst_pixels = (LPRGBQUAD) buf_r;

					if(vertical > 0)
					{
						for (int x = 0; x < width; x++)
						{
							for(int y = 0; y < height; y++)
							{
								int y_r = x;
								int x_r = height - y - 1;
								int idx_src = y * width   + x;
								int idx_dst = y_r * width_r + x_r;
								dst_pixels[idx_dst] = src_pixels[idx_src];
							}
						}
					} else
					{
						for (int x = 0; x < width; x++)
						{
							for(int y = 0; y < height; y++)
							{
								int y_r = width - x - 1;
								int x_r = y;
								int idx_src = y * width   + x;
								int idx_dst = y_r * width_r + x_r;
								dst_pixels[idx_dst] = src_pixels[idx_src];
							}
						}
					}

					BLENDFUNCTION bf;
					bf.BlendOp = AC_SRC_OVER;
					bf.BlendFlags = 0;
					bf.AlphaFormat = AC_SRC_ALPHA;
					bf.SourceConstantAlpha = 255;

					AlphaBlend(hdc, rcText->left, rcText->top, width_r, height_r, memDC, 0, 0, width_r, height_r, bf);

					SelectObject(memDC, tempBmp);
					DeleteObject(bmp);
					DeleteObject(bmp_r);
				}
				DeleteDC(memDC);
			}

			return;
		}

		//if(isVista())
		{
			LPWSTR wText;
			wText = (LPWSTR) text;
			if(color == 0xFFFFFFFF)
			{
				color = GetTextColor(hdc);
			}

			if(drawGlow && !isCalc)
			{
				RECT rcGlowRect = *rcText;

				int width  = rcGlowRect.right  - rcGlowRect.left;
				int height = rcGlowRect.bottom - rcGlowRect.top;

				BITMAPINFO bmp_info; 
				bmp_info.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER); 
				bmp_info.bmiHeader.biWidth			= width; 
				bmp_info.bmiHeader.biHeight			= -height; 
				bmp_info.bmiHeader.biPlanes			= 1; 
				bmp_info.bmiHeader.biBitCount		= 32; 
				bmp_info.bmiHeader.biCompression	= BI_RGB; 
				bmp_info.bmiHeader.biSizeImage		= 0; 
				bmp_info.bmiHeader.biXPelsPerMeter	= 0; 
				bmp_info.bmiHeader.biYPelsPerMeter	= 0; 
				bmp_info.bmiHeader.biClrUsed		= 0; 
				bmp_info.bmiHeader.biClrImportant	= 0; 

				HDC memDC = CreateCompatibleDC(NULL); 

				void* buf = 0; 
				HBITMAP bmp = ::CreateDIBSection( 
					memDC, 
					&bmp_info, 
					DIB_RGB_COLORS, 
					&buf, 
					0, 
					0 
					); 

				if(buf && bmp)
				{
					ZeroMemory(buf, width * height * 4);

					HBITMAP tempBmp = (HBITMAP)::SelectObject(memDC, bmp);
					RECT tempRect;
					tempRect.left	= 0;
					tempRect.right	= width;
					tempRect.top	= 0;
					tempRect.bottom = height;
					SetTextColor(memDC, glowColor);
					{
						HFONT		fnt = hFont;
						COLORREF	clr = color;
						if(!fnt)
						{
							fnt = (HFONT) GetCurrentObject(hdc, OBJ_FONT);
						}
						if(clr == 0xFFFFFFFF)
						{
							clr = GetTextColor(hdc);
						}
						tlbDrawText(memDC, text, c, &tempRect, params, fnt, glowColor, 0, FALSE, 0, glowSize, TRUE);
					}

					fastbluralpha((LPRGBQUAD) buf, width, height, glowSize);

					RGBQUAD* pixels = (RGBQUAD*) buf;
					int sz = width * height;

					int r = GetRValue(glowColor);
					int g = GetGValue(glowColor);
					int b = GetBValue(glowColor);
					for(int i=0; i < sz; i++)
					{
						pixels[i].rgbReserved	= min(255, pixels[i].rgbReserved << 2);
						pixels[i].rgbRed		= r * pixels[i].rgbReserved >> 8;
						pixels[i].rgbGreen		= g * pixels[i].rgbReserved >> 8;
						pixels[i].rgbBlue		= b * pixels[i].rgbReserved >> 8;
					}


					BLENDFUNCTION bf;
					bf.BlendOp = AC_SRC_OVER;
					bf.BlendFlags = 0;
					bf.AlphaFormat = AC_SRC_ALPHA;
					bf.SourceConstantAlpha = 255;

					int x = rcText->left;
					int y = rcText->top;

					AlphaBlend(hdc, x, y, width, height, memDC, 0, 0, width, height, bf);

					SelectObject(memDC, tempBmp);
					DeleteDC(memDC);
					DeleteObject(bmp);
				}
			}
			// Draw text
			{
				C32DC dc;

				HDC		drawDC	= hdc;
				BOOL	isMemDC	= FALSE;
				if(!PtVisible(hdc, rcText->left, rcText->top) || !PtVisible(hdc, rcText->right, rcText->bottom))
				{
					drawDC	= dc.beginPaint(hdc, rcText);
					isMemDC	= TRUE;
				}

				COLORREF clr = color;
				if(clr == 0xFFFFFFFF)
				{
					clr = GetTextColor(hdc);
				}
				DTTOPTS opt = {0};
				opt.dwSize		= sizeof(DTTOPTS);
				opt.dwFlags		= DTT_TEXTCOLOR | DTT_COMPOSITED | (isCalc ? DTT_CALCRECT : 0);
				opt.crText		= clr;
				HFONT oldFont = NULL;
				if(hFont)
				{
					oldFont = (HFONT) SelectObject(drawDC, hFont);
				} else if(isMemDC)
				{
					oldFont = (HFONT) SelectObject(drawDC, GetCurrentObject(hdc, OBJ_FONT));
				}

				RECT rcDraw = *rcText;
				rcDraw.left    += glowSize / 2;
				rcDraw.top     += glowSize / 2;
				rcDraw.right   -= glowSize / 2;
				rcDraw.bottom  -= glowSize / 2;

				HMODULE hUxTheme = GetModuleHandle(TEXT("uxtheme.dll"));
				if(hUxTheme)
				{
					fncDrawThemeTextEx dtEX = (fncDrawThemeTextEx) GetProcAddress(hUxTheme, "DrawThemeTextEx");
					if(dtEX)
					{
						if(dtEX(NULL, drawDC, 0, 0, text, c, params, &rcDraw, &opt) == S_OK)
						{
							process = FALSE;
						} else
						{
							HTHEME hTheme = OpenThemeData(NULL, TEXT("TASKBAR"));
							if(dtEX(hTheme, drawDC, 0, 0, text, c, params, &rcDraw, &opt) == S_OK)
							{
								process = FALSE;
							}
							CloseThemeData(hTheme);
						}
					}
				}
				if(process)
				{
					if(isCalc || !isTransparentDC)
					{
						HFONT oldFont = NULL;
						if(hFont)
						{
							oldFont = (HFONT) SelectObject(hdc, hFont);
						}
						if(clr != 0xFFFFFFFF)
						{
							SetTextColor(hdc, clr);
						}
						SetBkMode(hdc, TRANSPARENT);
						DrawText(hdc, text, c, &rcDraw, params);
						if(oldFont)
						{
							SelectObject(hdc, oldFont);
						}
						process = FALSE;
						isMemDC = FALSE;
					} else
					{
						C32DC dc;
						HDC memDC = dc.beginPaint(hdc, &rcDraw);

						int width  = rcDraw.right  - rcDraw.left;
						int height = rcDraw.bottom - rcDraw.top;

						SetTextColor(memDC, RGB(255, 255, 255));

						SetBkMode(memDC, TRANSPARENT);
						HFONT oldFont = NULL;
						HFONT fnt = NULL;
						if(hFont)
						{
							fnt = hFont;
						} else
						{
							fnt = (HFONT) GetCurrentObject(hdc, OBJ_FONT);
						}
						LOGFONT lf;
						GetObject(fnt, sizeof(lf), &lf);
						lf.lfQuality = ANTIALIASED_QUALITY;
						fnt = CreateFontIndirect(&lf);
						oldFont = (HFONT) SelectObject(memDC, fnt);

						DrawText(memDC, text, c, &rcDraw, params);

						SelectObject(memDC, oldFont);
						DeleteObject(fnt);

						RGBQUAD* pixels = dc.getBits();
						int sz = width * height;

						int r = GetRValue(clr);
						int g = GetGValue(clr);
						int b = GetBValue(clr);
						for(int i=0; i < sz; i++)
						{
							if(pixels[i].rgbRed)
							{
								pixels[i].rgbReserved	= pixels[i].rgbRed;
								if(pixels[i].rgbReserved < 200)
								{
									pixels[i].rgbReserved = pixels[i].rgbReserved / 3 * 2;
								}
								pixels[i].rgbRed		= (int) (r	* pixels[i].rgbReserved) / 255;
								pixels[i].rgbGreen		= (int) (g	* pixels[i].rgbReserved) / 255;
								pixels[i].rgbBlue		= (int) (b	* pixels[i].rgbReserved) / 255;
							}
						}

						dc.endPaint();

						if(oldFont)
						{
							SelectObject(memDC, oldFont);
						}

						process = FALSE;
						isMemDC = FALSE;
					}
				}

				if(!process && isCalc)
				{
					rcText->right  = rcText->left + (rcDraw.right  - rcDraw.left) + glowSize;
					rcText->bottom = rcText->top  + (rcDraw.bottom - rcDraw.top)  + glowSize;
				}

				if(oldFont)
				{
					SelectObject(drawDC, oldFont);
				}
				if(isMemDC)
				{
					dc.endPaint();
				}
			}
		}
		if(process)
		{
			if(color != 0xFFFFFFFF)
			{
				SetTextColor(hdc, color);
			}
			SetBkMode(hdc, TRANSPARENT);
			HFONT oldFont = NULL;
			if(hFont)
			{
				oldFont = (HFONT) SelectObject(hdc, hFont);
			}
			switch(vertical)
			{
			case 1:
				TextOut(hdc, rcText->right, rcText->top + 3, text, c);
				break;
			case -1:
				TextOut(hdc, rcText->left, rcText->bottom - 3, text, c);
				break;
			default:
				DrawText(hdc, text, c, rcText, params);
				break;
			}
			if(oldFont)
			{
				SelectObject(hdc, oldFont);
			}
		}
	}

}
