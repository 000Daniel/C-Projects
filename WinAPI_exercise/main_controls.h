#include <windows.h>

extern HWND
		static_label1,
		static_label2,
		hRich_edit1,
		hButton1,
		hButton2,
		hButton3,
		hPanel1_vscroll;
extern RECT vScrollbar_RECT;
extern int
		vScrollHeight,
		vScrollHeight_diff;
extern float
		vScrollRatio,
		vScrollPosRatio;

#define BUTTON_LOAD_FILE 1
#define BUTTON_SAVE_FILE_AS 2
#define BUTTON_UPDATE_SB 3

void AddControls(HWND);
void ControlsSettings();
void LoadImages();
void OpenFileName_func(HWND);
void SaveFileAs_func(HWND);
void SetRichEdit_Text(HWND, char[]);
void write_debug(int, const size_t, const wchar_t* const, ...);
SCROLLINFO vScrollbar_calc();
DWORD WINAPI update_scrollbar_thread(LPVOID);