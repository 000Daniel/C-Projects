#include <stdio.h>
#include "Functions.h"
#include "Elements.h"

extern HWND hContainer;
HWND last_interacted_with;
int last_interacted_with_id;
extern int LineCount;
char FileLocation[MAX_PATH] = "";

void RecenterWindow(HWND hwnd)
{
            //  This function calculates where to recenter the program's position.
    RECT rect;
    GetWindowRect(hwnd, &rect);
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

            // Get the screen dimensions.
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

            // Calculate the new position.
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;

            // Set the window's position.
    SetWindowPos(hwnd, HWND_TOPMOST, x, y, windowWidth, windowHeight, SWP_NOSIZE | SWP_NOACTIVATE);
}

void UpdateScrollBar(HWND hwnd, int LineCount)
{
            //  This function updates the V-Scrollbar's(of the contrainer) data.
    int minScroll = 0;
    int maxScroll = LineCount * 30;

    RECT clientRect;        // Get the client area height.
    GetClientRect(hwnd, &clientRect);
    int clientHeight = clientRect.bottom - clientRect.top;

    SCROLLINFO si = {0};    // Scrollbar settings.
    si.cbSize = sizeof(SCROLLINFO);

    if (maxScroll >= clientHeight)
    {
                //  These settings display and enable the scrollbar.
        si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
        si.nMin = minScroll;
        si.nMax = maxScroll;
        si.nPage = clientHeight;
        si.nPos = minScroll;
        si.nTrackPos = si.nPos;

        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        EnableScrollBar(hwnd, SB_VERT, ESB_ENABLE_BOTH);
    }
    else
    {
                //  These settings hide or disable the scrollbar.
        if (LineCount == 0)
        {
            si.nMax = 0;        //  This hides the scrollbar entirely.
        }
        else
        { 
            si.nMax = 100;      //  This shows the scrollbar.
        }
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nPage = 1;
        si.nPos = 0;

        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);     
        EnableScrollBar(hwnd, SB_VERT, ESB_DISABLE_BOTH);   //  The 'ESB_DISABLE_BOTH' flag disables the scrollbar but still shows it.
    }
}

void Read_File_Function(HWND hwnd)
{
            //  A function that lets the user open a file and load its contents into the program.
    OPENFILENAME ofn;
    char szFile[260];

            // Initialize OPENFILENAME.
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrTitle = "Open Text File";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    if (GetOpenFileName(&ofn)) {
        FILE* file;
        fopen_s(&file, ofn.lpstrFile, "r");

        strncpy(FileLocation, ofn.lpstrFile, MAX_PATH);     //  After a file is saved or opened, save its directory to 'FileLocation'.

        if (!file) return;      //  If the file couldn't open, cancel the function.

        clear_all_lines();      //  This resets some information used for the Lines.

        char buffer[256];       //  This buffer will contain the Line's string.
        while (fgets(buffer, sizeof(buffer), file))
        {
            size_t len = strlen(buffer);

            if (len > 1)
            {
                        //  If the last character in the string is a 'new line', replace it with an 'end line'.
                        //  This prevents the creation of unnecessary lines.
                if (buffer[len - 1] == '\n')
                {
                    buffer[len - 1] = '\0';
                }

                        //  If the line starts with '[v]', check the checkbox TRUE and shorten the string by 3.
                if (strncmp(buffer, "[v]", 3) == 0)
                {
                    char* shortenedBuffer = buffer + 3;
                    CreateCheckBox(hContainer, shortenedBuffer, TRUE);
                }       //  Same, but with [x] make sure the checkbox is unchecked.
                else if (strncmp(buffer, "[x]", 3) == 0)
                {
                    char* shortenedBuffer = buffer + 3;
                    CreateCheckBox(hContainer, shortenedBuffer, FALSE);
                }
                else
                {
                    CreateCheckBox(hContainer, buffer, FALSE);
                }
            }
        }
        fclose(file);
    }
}

void Save_File_Function(HWND hwnd)
{
            //  Function that saves the current data available in the program to a .txt file.
            //  'dwAttrib' makes sure that 'FileLocation' actually exists.
            //  If it exists already skip this entire function, else show a save dialog box.
    DWORD dwAttrib = GetFileAttributes(FileLocation);
    if (((int)strlen(FileLocation)) <= 1 || dwAttrib == INVALID_FILE_ATTRIBUTES)
    {
        OPENFILENAME ofn;
        char szFile[260];

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.lpstrDefExt = "txt";
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "Text files\0*.TXT\0All files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.lpstrTitle = "Save File As";
        ofn.Flags = OFN_OVERWRITEPROMPT;
        
        if (GetSaveFileName(&ofn))
        {
            strncpy(FileLocation, ofn.lpstrFile, MAX_PATH);     //  Save the saved file's location to 'FileLocation'.

            char *dotPos = strrchr(FileLocation, '.');          // Find the last dot in the file's name.
            if (dotPos != NULL)
            {
                for (char *p = dotPos; *p != '\0'; p++)
                {
                    *p = (char)tolower((unsigned char)*p);      //  Replace the text after the dot to lower case. (.TXT -> .txt)
                }
            }
        }
    }

                //  Open the file at its location.
    FILE *file = fopen(FileLocation, "w");
    if (file == NULL) return;

                //  Go through every 'Line' under hContainer and save it into the .txt file.
    HWND hChild = GetWindow(hContainer, GW_CHILD);
    while (hChild)
    {
        char buffer_start[259];
                //  Check if the 'Line' is checked, if so add '[v]' or '[x]' to 'buffer_start'.
        int state = SendMessage(hChild, BM_GETCHECK, 0, 0);
        if (state == BST_CHECKED)
        {
            strncpy(buffer_start, "[v]", sizeof(buffer_start) - 1); // -1 to leave space for the null terminator
        }
        else
        {
            strncpy(buffer_start, "[x]", sizeof(buffer_start) - 1); // -1 to leave space for the null terminator
        }

                //  Save the 'Line's text to 'buffer'.
        char buffer[256];
        HWND hEdit = GetDlgItem(hChild, 1);
        GetWindowText(hEdit, buffer, sizeof(buffer));
                
        strcat(buffer_start, buffer);           //  Combine the buffer_start and buffer together.   
        fprintf(file, "%s\n", buffer_start);    //  Input this string into the .txt file.
        
        hChild = GetWindow(hChild, GW_HWNDNEXT);//  Get the next 'Line'.
    }

    fclose(file);
}

void last_interacted_update(HWND hwnd, int id)
{
            //  A small function to update which 'Line' the user last interacted with.
    last_interacted_with = hwnd;
    last_interacted_with_id = id;
}
HWND last_interacted_get()
{
    return GetDlgItem(last_interacted_with, (int)last_interacted_with_id);
}
void last_interacted_delete()
{
            //  A function used by the delete button to delete the last interacted 'Line'.
    HWND hLastInteracted = last_interacted_get();
    HWND hParent = GetParent(hLastInteracted);

    if (hLastInteracted == NULL) return;

    HWND hFocusOn = GetWindow(GetWindow(hLastInteracted, GW_HWNDNEXT), GW_CHILD);
    if (hFocusOn == NULL)           //  Incase the user deletes the last control, focus on the previous one.
    {
        hFocusOn = GetWindow(GetWindow(hLastInteracted, GW_HWNDPREV), GW_CHILD);
    }

    DestroyWindow(hLastInteracted); // Destroy the control.
    last_interacted_with = NULL;
    last_interacted_with_id = 0;
    LineCount--;

    if (hFocusOn != NULL)           //  Set focus on 'hFocusOn'.
    {
        SetFocus(hFocusOn);
        int length = GetWindowTextLength(hFocusOn);
        SendMessage(hFocusOn, EM_SETSEL, length, length);

        UpdateControlsPositions();
    }

    UpdateScrollBar(hParent, LineCount);
}

void clear_all_lines()
{
            //  This function deletes all hContainer's children (deletes the 'Lines').
    HWND hChild = GetWindow(hContainer, GW_CHILD);

    while (hChild)
    {
        DestroyWindow(hChild);
        hChild = GetWindow(hContainer, GW_CHILD);
    }
    clear_line_count();
    last_interacted_with = NULL;
    last_interacted_with_id = 0;
}

void PrintInfo(const char* message, ...)
{
            //  A quick function to print messages into a message box. This is only used for debugging!
    char buffer[256];
    va_list args;

    va_start(args, message);        // Initialize args to retrieve the additional arguments.
    vsnprintf(buffer, sizeof(buffer), message, args); // Format the string.
    va_end(args);                   // Clean up the variable argument list.

    LPCSTR lpcstrMessage = (LPCSTR)buffer;              // Convert char* to LPCSTR.
    MessageBoxA(NULL, lpcstrMessage, "Message", MB_OK); // Display the message box.
}