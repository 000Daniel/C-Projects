#include "main_frame.h"
#include "resource.h"

RECT window_rect = {200, 300, 800, 600};
wchar_t mainframe_name[] = L"Main Window";
byte DEBUG_SESSION = 0;
HWND hMainFrame;
PAINTSTRUCT ps, ps_scrollbar;

byte First_Resize = 1;

		//	WinMain is the main() of WinAPI, here WinAPI creates a window(hMainFrame).
int WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
	set_colors();

	WNDCLASSW wc = { 0 };
	wc.hbrBackground = c_background;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"MainFrame";
	wc.lpfnWndProc = WindowProcedure;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON));

	if (!RegisterClassW(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hMainFrame = CreateWindowW(L"MainFrame", mainframe_name, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		window_rect.left, window_rect.top, window_rect.right, window_rect.bottom, NULL, NULL, NULL, NULL);

	if (hMainFrame == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		//	TranslateKeyMessage(msg);	//	This function isn't in use anymore!
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

		//	this 'WindowProcedure' is the 'SendMessage' reciever of the main window.
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
				//	'WM_COMMAND' is useful to detect if we pressed on a button.
		case WM_COMMAND:
			switch (wp)
			{
				case BUTTON_LOAD_FILE:
					OpenFileName_func(hWnd);
					break;
				case BUTTON_SAVE_FILE_AS:
					SaveFileAs_func(hWnd);
					break;
				case BUTTON_UPDATE_SB:
					RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
					break;
			}
			break;
				//	'WM_CTLCOLORSTATIC' sets the color for the static labels,
				//	the 'return (INT_PTR)color;' used so WinAPI won't override the new colors.
		case WM_CTLCOLORSTATIC:
			if (DEBUG_SESSION == 0) break;
			if (static_label1 == (HWND)lp)
			{
				HDC hdcStatic_label1 = (HDC)wp;
				SetTextColor(hdcStatic_label1, GetBrushColor(c_foreground));
				SetBkColor(hdcStatic_label1, GetBrushColor(c_background));
				return (INT_PTR)c_background;
			}
			if (static_label2 == (HWND)lp)
			{
				HDC hdcStatic_label2 = (HDC)wp;
				SetTextColor(hdcStatic_label2, GetBrushColor(c_foreground));
				SetBkColor(hdcStatic_label2, GetBrushColor(c_background));
				return (INT_PTR)c_background;
			}
			break;
				//	'WM_CREATE' is sent once when the window is created.
				//	using this message we load the images(.bmp files),
				//	and then adding the other controls.
		case WM_CREATE:
			LoadImages();
			AddControls(hWnd);
			break;
		case WM_SIZE:
				//	'WM_SIZE' is called everytime the user resizes the window.
				//	When the program starts it sends a WM_SIZE message, we use First_Resize to detect that message
				//	and 'suppress' it so that the WM_PAINT message won't be called twice on startup.
			if (First_Resize)
			{
				First_Resize = 0;
				break;
			}

			RECT Screen_rect;
			GetClientRect(hMainFrame, &Screen_rect);
			SetWindowPos(hRich_edit1, 0, 0, 60, Screen_rect.right - 17, Screen_rect.bottom - 77, WVR_REDRAW);

			update_vscroll_only = 0;
			RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
			break;
				//	'WM_PAINT' is called to redraw the entire window.
		case WM_PAINT:
			main_paint();
			break;
				//	this 'WM_DESTROY' message is responsible for closing the window.
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
				//	Everything else goes into 'DefWindowProcW' to be automatically processed.
		default:
			return DefWindowProcW(hWnd, msg, wp, lp);
			break;
	}
}

		//	This function can be used to intercept key presses and more.
		//	In this program this function is unused.
int TranslateKeyMessage(MSG Msg)
{
			// Determine the virtual key code.
	int VirtualKeyCode = Msg.wParam;
	if (Msg.message == WM_KEYUP || Msg.message == WM_CHAR || Msg.message == WM_MOUSEWHEEL)
	{	}
	return VirtualKeyCode;
}