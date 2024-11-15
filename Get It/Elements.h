#include "stdio.h"
#include <windows.h>
#include "resource.h"
#include "Functions.h"
#include "gpedit.h"
#include <wchar.h>

LRESULT CALLBACK FlatButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ContainerProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MoveButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CreateButton(HWND hwnd, HINSTANCE hInstance, int Window_Width, int Window_Height, int ID);
HWND CreateContainer(HWND hwnd, LPARAM lParam);

void CreateCheckBox(HWND hwnd, LPCSTR lpcstr, BOOL CHECKED);

void UpdateControlsPositions();

void clear_line_count();

void LoadImagesFromRes();

		//	Each 'COL_BUTTON' here represents RGB settings for each of the buttons.
		//	For now they use the same color values, but it can be changed.
#define COL_BUTTON_EXIT RGB(140,0,0)
#define COL_BUTTON_EXIT_HOVER RGB(175,0,0)
#define COL_BUTTON_EXIT_PRESS RGB(125,0,0)

#define COL_BUTTON_MINIMIZE RGB(54,171,171)
#define COL_BUTTON_MINIMIZE_HOVER RGB(50,187,187)
#define COL_BUTTON_MINIMIZE_PRESS RGB(58,161,161)

#define COL_BUTTON_OPEN RGB(229,187,133)			//RGB(142,208,255)
#define COL_BUTTON_OPEN_HOVER RGB(226,196,158)		//RGB(160,215,255)
#define COL_BUTTON_OPEN_PRESS RGB(226,165,90)		//RGB(115,168,206)

#define COL_BUTTON_DELETE RGB(229,187,133)			//RGB(225,142,142)
#define COL_BUTTON_DELETE_HOVER RGB(226,196,158)	//RGB(225,160,160)
#define COL_BUTTON_DELETE_PRESS RGB(226,165,90)		//RGB(204,114,144)

#define COL_BUTTON_ADD RGB(229,187,133)
#define COL_BUTTON_ADD_HOVER RGB(226,196,158)
#define COL_BUTTON_ADD_PRESS RGB(226,165,90)
#define COL_BUTTON_ADD_TEXT RGB(255,255,255)		//RGB(58,58,58)

#define COL_BUTTON_SAVE RGB(229,187,133)			//RGB(161,196,165)
#define COL_BUTTON_SAVE_HOVER RGB(226,196,158)		//RGB(172,196,175)
#define COL_BUTTON_SAVE_PRESS RGB(226,165,90)		//RGB(138,168,142)

#define COL_BACKGROUND RGB(255,255,255)
#define COL_TITLEBAR RGB(229,187,133)