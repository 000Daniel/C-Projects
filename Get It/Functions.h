#include <windows.h>
#include <commdlg.h>
#include "resource.h"
#include "GetIt.h"

void RecenterWindow(HWND hwnd);
void UpdateScrollBar(HWND hwnd, int LineCount);
void Read_File_Function(HWND hwnd);
void Save_File_Function(HWND hwnd);
void last_interacted_update(HWND hwnd, int id);
HWND last_interacted_get();
void last_interacted_delete();
void clear_all_lines();
void PrintInfo(const char* message, ...);