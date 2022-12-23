#include <windows.h>
#include <stdio.h>
#include "main_frame_paint.h"
#include "main_controls.h"

extern byte DEBUG_SESSION;
extern RECT window_rect;
extern HWND hMainFrame;
extern PAINTSTRUCT ps;
int TranslateKeyMessage(MSG Msg);

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);