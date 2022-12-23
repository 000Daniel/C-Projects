#include "main_controls.h"
#include "main_frame.h"
#include "Richedit.h"
#include "resource.h"

HWND static_label1, static_label2, hRich_edit1, hButton1, hButton2, hButton3, hPanel1_vscroll;
RECT rich_edit1_margins = {3, 3, 0, 0}; //Left, Top, Right, Bottom
RECT vScrollbar_RECT = { 0,0,0,0 };
int vScrollHeight = 0, vScrollHeight_diff = 0;
float vScrollRatio = 0, vScrollPosRatio = 0;
HBITMAP hButton1_bitmap, hButton2_bitmap, hButton3_bitmap;

		//	This function adds all the controls for the 'Main' window.
void AddControls(HWND hWnd)
{
	if (DEBUG_SESSION == 1)
	{
		static_label1 = CreateWindowW(L"Static", L"This is a debug label.", WS_VISIBLE | WS_CHILD | DT_NOCLIP, 580, 20, 1000, 20, hWnd, NULL, NULL, NULL);
		static_label2 = CreateWindowW(L"Static", L"This is a debug label.", WS_VISIBLE | WS_CHILD | DT_NOCLIP, 580, 40, 1000, 20, hWnd, NULL, NULL, NULL);
	}
	hButton1 = CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT, 165, 24, 100, 20, hWnd, (HMENU)BUTTON_LOAD_FILE,	  NULL, NULL);
	hButton2 = CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT, 280, 24, 120, 20, hWnd, (HMENU)BUTTON_SAVE_FILE_AS, NULL, NULL);
	hButton3 = CreateWindowW(L"Button", L"", WS_VISIBLE | WS_CHILD | BS_BITMAP | BS_FLAT, 415, 24, 160, 20, hWnd, (HMENU)BUTTON_UPDATE_SB,	  NULL, NULL);
	SendMessageW(hButton1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hButton1_bitmap);
	SendMessageW(hButton2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hButton2_bitmap);
	SendMessageW(hButton3, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hButton3_bitmap);

			//	We need to load "riched20.dll" to create a Rich Edit.
	LoadLibrary("riched20.dll");
	hRich_edit1 = CreateWindowEx(0,RICHEDIT_CLASS,"",ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_HSCROLL | WS_VSCROLL,
		0, 60, window_rect.right - 33, window_rect.bottom - 116, hWnd, NULL, NULL, NULL);

	ControlsSettings();
}

		//	This function set's the settings for the existing controls.
void ControlsSettings()
{
			//#######################################	RICH	EDIT	SETTINGS	#######################################
	SendMessage(hRich_edit1, WM_SETFONT, (WPARAM)font_arial, 0);	// Rich edit's font.
	SendMessage(hRich_edit1, EM_SETBKGNDCOLOR, 0, GetBrushColor(c_background_darken));	// background color of the Rich edit.
			//	Sets the text color of the Rich edit.
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = GetBrushColor(c_foreground);
	SendMessage(hRich_edit1, EM_SETCHARFORMAT, 0, (LPARAM)&cf);

			//	Sets the rich edit's margins/text padding.
	RECT re1m_temp;
	SendMessage(hRich_edit1, EM_GETRECT, 0, (LPARAM)&re1m_temp);
	re1m_temp.left += rich_edit1_margins.left;
	re1m_temp.right += rich_edit1_margins.right;
	re1m_temp.top += rich_edit1_margins.top;
	re1m_temp.bottom += rich_edit1_margins.bottom;
	SendMessage(hRich_edit1, EM_SETRECT, 0, (LPARAM)&re1m_temp);
			//	'SetWindowText' resets the undo/redo stack.
	SetWindowText(hRich_edit1, "");
			//#################################################    END   ##################################################

			//	Here we create a new thread for the custom scrollbar.
	int Data_of_Thread = 1;
	HANDLE Handle_of_Thread = 0;

	Handle_of_Thread = CreateThread(NULL, 0, update_scrollbar_thread, &Data_of_Thread, 0, NULL);
	if (Handle_of_Thread == NULL)
		ExitProcess(Data_of_Thread);
	CloseHandle(Handle_of_Thread);
}

		//	This function loads the button Bitmaps.
void LoadImages()
{
	hButton1_bitmap = (HBITMAP)LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCEW(IDI_BITMAP1));
	hButton2_bitmap = (HBITMAP)LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCEW(IDI_BITMAP2));
	hButton3_bitmap = (HBITMAP)LoadBitmapW(GetModuleHandle(NULL), MAKEINTRESOURCEW(IDI_BITMAP3));
}

		//	This function uses Open File Dialog(Name) to load files into the Rich Edit.
void OpenFileName_func(HWND hWnd)
{
	OPENFILENAME ofn;

	char file_name[100];
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = "All files\0*.*\0Text Files\0*.TXT\0";
	ofn.nFilterIndex = 1;

	if (!GetOpenFileName(&ofn))
		return;

	FILE *file;
	file = fopen(ofn.lpstrFile, "r");
	fseek(file, 0, SEEK_END);
	int _size = ftell(file);
	rewind(file);
	char data[_size + 1];
	fread(data, _size, 1, file);
	data[_size] = '\0';

	SetRichEdit_Text(hRich_edit1, data);

	fclose(file);
}
		//	This function uses Save(Open) File Dialog(Name) to save the Rich Edit's contents into a file.
void SaveFileAs_func(HWND hWnd)
{
	OPENFILENAME ofn;

	char file_name[100] = "*.txt\0";
	ZeroMemory(&ofn, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = "All files (*)\0*.*\0Text Files (*.txt)\0*.txt\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrDefExt = "txt";

	GetSaveFileName(&ofn);

	FILE *file;
	file = fopen(ofn.lpstrFile, "wb");
	
	int _size = GetWindowTextLength(hRich_edit1);
	char data[_size + 1];
	GetWindowText(hRich_edit1, data, _size + 1);

	fwrite(data, _size + 1, 1, file);
	fclose(file);
}

		//	This function replaces all the text inside the Rich edit without
		//	Getting rid of the undo/redo stack.
void SetRichEdit_Text(HWND hwnd, char data[])
{	
	int index = GetWindowTextLength(hwnd);
	SetFocus(hwnd); // set focus
	SendMessageA(hwnd, EM_SETSEL, 0, (LPARAM)index);	// set selection to the end of text
	SendMessageA(hwnd, EM_REPLACESEL, 1, (LPARAM)data); // append new text.

	SendMessageA(hwnd, EM_SETSEL, 0, 0); //	Set the selection to the beginning of the page.
}
		//	This function appends text to the Rich edit without clearing the
		//	undo/redo stack.
		//	<<This is an unused function!>>
void AppendText(HWND hwnd, char data[])
{
	DWORD StartPos, EndPos;	//	Save the user's selection.
	SendMessageA(hwnd, EM_GETSEL, (WPARAM)StartPos, (LPARAM)EndPos);

	int index = GetWindowTextLength(hwnd);
	SetFocus(hwnd);
	SendMessageA(hwnd, EM_SETSEL, (WPARAM)index, (LPARAM)index);
	SendMessageA(hwnd, EM_REPLACESEL, 1, (LPARAM)data);
			//	Set the selection to the user's old selection.
	SendMessageA(hwnd, EM_SETSEL, StartPos, EndPos);
}
		//	This function replaces the text in the static debug texts.
void write_debug(int staticlabel, const size_t buffer_size , const wchar_t* const Format, ...)
{
	if (DEBUG_SESSION == 0)
		return;
	wchar_t regular_text[buffer_size];
	swprintf(regular_text, buffer_size, Format);
	if (staticlabel == 1)
		SetWindowTextW(static_label1, regular_text);
	if (staticlabel == 2)
		SetWindowTextW(static_label2, regular_text);
}

		//	This function calculates all the data necessary for the custom scrollbar.
SCROLLINFO vScrollbar_calc()
{	
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_RANGE | SIF_TRACKPOS;
	GetScrollInfo(hRich_edit1, SB_VERT, &si);

	if (si.nMax < 0 || si.nPage < 0) return si;

	vScrollRatio = (float)si.nPage / (float)si.nMax;
	vScrollPosRatio = (float)(si.nTrackPos + si.nPage) / (float)si.nMax;
	vScrollHeight = (int)((vScrollbar_RECT.bottom - vScrollbar_RECT.top) * vScrollRatio);

	vScrollHeight_diff = 0;
	if (vScrollHeight < 17)
	{
		vScrollHeight_diff = 17 - vScrollHeight;
		vScrollHeight = 17;
	}

	return si;
}

		//	This function runs on a different thread then the 'main' one.
		//	It repeats itself to update overy few milliseconds.
DWORD WINAPI update_scrollbar_thread(LPVOID lpParam)
{
	Sleep(50);		//	Too low of a number would cause the entire window to not render properly.
	update_vscroll_only = 1;
	RedrawWindow(hMainFrame, &vScrollbar_RECT, NULL, RDW_UPDATENOW | RDW_INVALIDATE);

	update_scrollbar_thread(lpParam);

	return 0;
}