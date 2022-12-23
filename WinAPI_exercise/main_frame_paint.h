#include <windows.h>

extern HBRUSH
		c_background,
		c_background_hightlight,
		c_background_darken,
		c_foreground,
		c_scrollbar_back,
		c_scrollbar_handle,
		c_scrollbar_handle_press;
extern HFONT font_arial;
extern BITMAP button1_bm;
extern int update_vscroll_only;

void set_colors();
void main_paint();
void Draw_Text(int, int, char[], HBRUSH, HBRUSH, HFONT);
int Scrollbar_Available(LONG);
COLORREF GetBrushColor(HBRUSH);