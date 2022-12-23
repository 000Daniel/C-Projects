#include "main_frame_paint.h"
#include "main_frame.h"

HBRUSH c_background, c_background_hightlight, c_background_darken, c_foreground;
HBRUSH c_scrollbar_back, c_scrollbar_handle, c_scrollbar_handle_press;
HFONT font_arial;
int update_vscroll_only = 0;
BITMAP button1_bm;

		//	Here we set the color settings.
		//	These colors are used by other methods aswell.
void set_colors()
{
	c_background = CreateSolidBrush(RGB(55, 45, 50));
	c_background_hightlight = CreateSolidBrush(RGB(75, 65, 70));
	c_background_darken = CreateSolidBrush(RGB(40, 30, 35));
	c_foreground = CreateSolidBrush(RGB(240, 235, 240));
	c_scrollbar_back = CreateSolidBrush(RGB(45, 45, 45));
	c_scrollbar_handle = CreateSolidBrush(RGB(65, 65, 65));
	c_scrollbar_handle_press = CreateSolidBrush(RGB(75, 75, 75));
	font_arial = CreateFontA(20, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, 0, TEXT("Arial"));
}

void main_paint()
{
	if (DEBUG_SESSION == 1)
		write_debug(2, 50, L"Painting...");

	BeginPaint(hMainFrame, &ps);
	
	RECT Screen_rect;
	GetClientRect(hMainFrame, &Screen_rect);

	if (update_vscroll_only == 0)
	{
		int border_thickness_x = GetSystemMetrics(SM_CXSIZEFRAME);
		RECT rect = { 10,15,Screen_rect.right - border_thickness_x - 2,18 };

		FillRect(ps.hdc, &Screen_rect, c_background);
		FillRect(ps.hdc, &rect, c_background_hightlight);

		Draw_Text(10, 24, "This is a drawn text!", c_foreground, c_background, font_arial);
	}

				//	DRAW SCROLL BARS FOR RICH EDIT!
				//	VERTICAL SCROLL:
	if (Scrollbar_Available(OBJID_VSCROLL) == 0)
	{
		vScrollbar_RECT = (RECT){ Screen_rect.right - 17, 60, Screen_rect.right, Screen_rect.bottom - 17 };
		FillRect(ps.hdc, &vScrollbar_RECT, c_scrollbar_back);

		SCROLLINFO si = vScrollbar_calc();
		int top_point = (int)((vScrollbar_RECT.bottom - vScrollbar_RECT.top - vScrollHeight_diff) * vScrollPosRatio) - vScrollHeight + vScrollHeight_diff;
		RECT vScrollbar_thumb_RECT = {
			vScrollbar_RECT.left + 2,
			vScrollbar_RECT.top + top_point,
			vScrollbar_RECT.right - 2,
			vScrollbar_RECT.top + top_point + vScrollHeight
		};

		FillRect(ps.hdc, &vScrollbar_thumb_RECT, c_scrollbar_handle);
	}
	else
	{
		vScrollbar_RECT = (RECT){ Screen_rect.right - 17, 60, Screen_rect.right, Screen_rect.bottom - 17 };
		FillRect(ps.hdc, &vScrollbar_RECT, c_scrollbar_back);
	}

	EndPaint(hMainFrame, &ps);
	update_vscroll_only = 0;
}

void Draw_Text(int x, int y, char text[], HBRUSH foreground_color, HBRUSH background_color, HFONT hFont)
{
	if (DEBUG_SESSION == 1)
		write_debug(1, 50, L"Drawing text...");
	SetBkColor(ps.hdc, GetBrushColor(background_color));
	SetTextColor(ps.hdc, GetBrushColor(foreground_color));
	SelectObject(ps.hdc, hFont);
	RECT new_rect = { x,y,400,400 };
	DrawTextA(ps.hdc, text, -1, &new_rect, DT_SINGLELINE | DT_NOCLIP);
}

int Scrollbar_Available(LONG s_bar)
{
	SCROLLBARINFO psbi;
	psbi.cbSize = sizeof(SCROLLBARINFO);
	BOOL getScroll = GetScrollBarInfo(hRich_edit1, s_bar, &psbi);

	if (getScroll == FALSE)
		return 0;

	return (DWORD)psbi.rgstate[0];
}

COLORREF GetBrushColor(HBRUSH brush)	//	Method copied from: www.devblogs.microsoft.com/oldnewthing/20190802-00/?p=102747
{
	LOGBRUSH lbr;
	if (GetObject(brush, sizeof(lbr), &lbr) != sizeof(lbr)) {
		return CC_NONE;	// Not even a brush!
	}
	if (lbr.lbStyle != BS_SOLID) {
		return CC_NONE;	// Not a solid color brush.
	}
	return lbr.lbColor;
}