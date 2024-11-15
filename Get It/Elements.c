#include "Elements.h"

extern HWND hContainer;
int LineCount = 0;
static int scrollPos = 0; // Scroll position
int window_height;

WNDPROC OriginalEditProc;

HWND* hwndList_;
size_t size_ = 0;
size_t capacity_ = 10;

typedef struct {
    HWND *list;
    size_t size;
    size_t capacity;
} HWNDList;

HBITMAP Img_ArrowUp = NULL;
HBITMAP Img_ArrowUp_h = NULL;
HBITMAP Img_ArrowUp_p = NULL;
HBITMAP Img_ArrowDown = NULL;
HBITMAP Img_ArrowDown_h = NULL;
HBITMAP Img_ArrowDown_p = NULL;
HBITMAP hImage = NULL;

void CreateButton(HWND hwnd, HINSTANCE hInstance,int Window_Width, int Window_Height, int ID)
{
            //  ID_BUTTON1 (400) - exit
            //  ID_BUTTON2 (401) - open
            //  ID_BUTTON3 (402) - delete
            //  ID_BUTTON4 (403) - add
            //  ID_BUTTON5 (405) - save
            //  ID_BUTTON6 (404) - minimize
    RECT r;

    switch (ID)
    {
    case ID_BUTTON1:
        r.left = Window_Width - 25;     //  X
        r.top = 0;                      //  Y
        r.right = 25;                   //  width
        r.bottom = 25;                  //  height
        break;
    case ID_BUTTON2:
        r.left = 50;
        r.top = 0;
        r.right = 50;
        r.bottom = 25;
        break;
    case ID_BUTTON3:
        r.left = 100;
        r.top = 0;
        r.right = 50;
        r.bottom = 25;
        break;
    case ID_BUTTON4:
        r.left = 150;
        r.top = 0;
        r.right = 50;
        r.bottom = 25;
        break;
    case ID_BUTTON5:
        r.left = 0;
        r.top = 0;
        r.right = 50;
        r.bottom = 25;
        break;
    case ID_BUTTON6:
        r.left = Window_Width - 50;
        r.top = 0;
        r.right = 25;
        r.bottom = 25;
        break;
    }

    HWND hButton = CreateWindowEx(
        0,
        "BUTTON",
        NULL,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        r.left,                 // X
        r.top,                  // Y
        r.right,                //width
        r.bottom,               //height
        hwnd,
        (HMENU)(uintptr_t)ID,   // Button ID
        hInstance,
        NULL
    );
    
        // Set the button procedure to handle custom drawing.
    SetWindowLongPtr(hButton, GWLP_WNDPROC, (LONG_PTR)FlatButtonProc);
}

void DrawFlatButton(HWND hButton, HDC hdc, int HoverState) {
    COLORREF clr_default = COL_BUTTON_EXIT;     //  Default button color
    COLORREF clr_hover = COL_BUTTON_EXIT_HOVER; //  Mouse hovering above color
    COLORREF clr_press = COL_BUTTON_EXIT_PRESS; //  Mouse pressing color
    COLORREF clr_text = RGB(255, 255, 255);     //  Text color
    LPCSTR string = "X";                        //  Button text
    BOOL LoadedImage = FALSE;                   //  'LoadedImage' determins whether to use an image or colors.    
    
    switch (GetDlgCtrlID(hButton))      //  Each case here is special for each button.
    {
    case ID_BUTTON2:
        clr_default = COL_BUTTON_OPEN;
        clr_hover = COL_BUTTON_OPEN_HOVER;
        clr_press = COL_BUTTON_OPEN_PRESS;
        string = "Open";
        break;
    case ID_BUTTON3:
        clr_default = COL_BUTTON_DELETE;
        clr_hover = COL_BUTTON_DELETE_HOVER;
        clr_press = COL_BUTTON_DELETE_PRESS;
        string = "Delete";
        break;
    case ID_BUTTON4:
        clr_default = COL_BUTTON_ADD;
        clr_hover = COL_BUTTON_ADD_HOVER;
        clr_press = COL_BUTTON_ADD_PRESS;
        clr_text = COL_BUTTON_ADD_TEXT;
        string = "Add";
        break;
    case ID_BUTTON5:
        clr_default = COL_BUTTON_SAVE;
        clr_hover = COL_BUTTON_SAVE_HOVER;
        clr_press = COL_BUTTON_SAVE_PRESS;
        string = "Save";
        break;
    case ID_BUTTON6:
        clr_default = COL_BUTTON_MINIMIZE;
        clr_hover = COL_BUTTON_MINIMIZE_HOVER;
        clr_press = COL_BUTTON_MINIMIZE_PRESS;
        string = "-";
        break;
    case ID_BUTTON_MOVEUP:
        LoadedImage = TRUE;
        switch (HoverState)
        {
        case 0:
            hImage = Img_ArrowUp;
            break;
        case 1:
            hImage = Img_ArrowUp_h;
            break;
        case 2:
            hImage = Img_ArrowUp_p;
            break;
        }
        break;
    case ID_BUTTON_MOVEDOWN:
        LoadedImage = TRUE;
        switch (HoverState)
        {
        case 0:
            hImage = Img_ArrowDown;
            break;
        case 1:
            hImage = Img_ArrowDown_h;
            break;
        case 2:
            hImage = Img_ArrowDown_p;
            break;
        }
    }
    
    RECT rect;
    GetClientRect(hButton, &rect);
    
    if (LoadedImage)        //  If loaded and image, display it as the button.
    {
        if (!hImage)
            return;

        BITMAP bm;
        bm.bmWidth = rect.right - rect.left;
        bm.bmHeight = rect.bottom - rect.top;
        GetObject(hImage, sizeof(bm), &bm);

        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hImage);
        BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);

        DeleteObject(hOldBitmap);
        hOldBitmap = NULL;
        DeleteDC(hMemDC);
        ReleaseDC(hButton, hdc);
    }
    else                    // If the image didn't load use default button colors.
    {
        HBRUSH brush;       // This brush will adapt the relevant color.

        switch (HoverState)
        {
        case 0:
            brush = CreateSolidBrush(clr_default);
            break;
        case 1:
            brush = CreateSolidBrush(clr_hover);
            break;
        case 2:
            brush = CreateSolidBrush(clr_press);
            break;
        }
        SetBkMode(hdc, TRANSPARENT);
        FillRect(hdc, &rect, brush);        //  This draws a rectancle the same size as the button.
        DeleteObject(brush);

        SetTextColor(hdc, clr_text);        // Draw button's text.
        DrawText(hdc, string, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

LRESULT CALLBACK FlatButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int HoverState = 0;
        //  HoverState represents the state of the button in terms of mouse interacion:
        //  0 - default.
        //  1 - mouse is hovering over.
        //  2 - mouse is pressing the button.
        //
        //  'InvalidateRect(hWnd, NULL, TRUE);' is a redraw request to refresh the button's graphic.

    switch (uMsg) {
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawFlatButton(hWnd, hdc, HoverState);
            EndPaint(hWnd, &ps);
            return 0;

        case WM_LBUTTONUP:    //  Forward every message from here, to the main function "GetIt.C"
            HoverState = 1;
            InvalidateRect(hWnd, NULL, TRUE);
            SendMessage(GetParent(hWnd), WM_COMMAND, GetDlgCtrlID(hWnd), 0);
            return 0;

        case WM_LBUTTONDOWN:
            HoverState = 2;
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;

        case WM_MOUSEMOVE:
            if (!HoverState == 1) {
                HoverState = 1;
                InvalidateRect(hWnd, NULL, TRUE);

                        //  Track the mouse's movement even outside of the control.
                TRACKMOUSEEVENT tme;
                tme.cbSize = sizeof(TRACKMOUSEEVENT);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = hWnd;
                TrackMouseEvent(&tme);
            }
            return 0;

        case WM_MOUSELEAVE:
            HoverState = 0;
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND CreateContainer(HWND hwnd, LPARAM lParam)
{
                    // Register the container window class and create it.
                    //  This is 'hContainer', and it contains all of the "Lines" controls.
            WNDCLASS wc = {0};
            wc.lpfnWndProc = ContainerProc;
            wc.hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
            wc.lpszClassName = "ContainerClass";
            RegisterClass(&wc);

            RECT windowRect;
            GetWindowRect(hwnd, &windowRect);
            int width = windowRect.right - windowRect.left;
            window_height = windowRect.bottom - windowRect.top;
            window_height -= 42;

            HWND hContainer = CreateWindow("ContainerClass", "Container",
                         WS_CHILD | WS_VISIBLE | WS_VSCROLL,
                         10, 30, width - 20, window_height,
                         hwnd, (HMENU)ID_CONTAINER, NULL, NULL);
    return hContainer;
}

LRESULT CALLBACK ContainerProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        UpdateScrollBar(hwnd, 0);
        LoadImagesFromRes();
        break;

    case WM_LBUTTONDOWN:        //  Allow draging of the entire program.
        SendMessage(GetParent(hwnd), WM_NCLBUTTONDOWN, HTCAPTION, 0);
        break;

    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)       //  If the button is pressed check or uncheck it.
            if (IsDlgButtonChecked(hwnd, LOWORD(wParam)) == TRUE)
            {
                CheckDlgButton(hwnd, LOWORD(wParam), BST_UNCHECKED);
            }
            else
            {
                CheckDlgButton(hwnd, LOWORD(wParam), BST_CHECKED);
            }

                        //  Set focus on the line's edit control.
            HWND hEdit = GetWindow((HWND)lParam, GW_CHILD);
            SetFocus(hEdit);

            int length = GetWindowTextLength(hEdit);
            SendMessage(hEdit, EM_SETSEL, length, length);
        break;

    case WM_VSCROLL:
                //  This code handles all of the (vertical)scroll logic of the hContainer.
        SCROLLINFO si;
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_ALL;

                //  Skip the rest of the function if the user cannot scroll at all.
        if (GetScrollInfo(hwnd, SB_VERT, &si) && si.nMax < si.nPage) return 0;

        int scrollCode = LOWORD(wParam);    //  scrollCode contains information about what the user is trying to do with the scrollbar.
        switch (scrollCode) {
        case SB_LINEUP:
            scrollPos = max(0, scrollPos - 10);
            break;
        case SB_LINEDOWN:
            scrollPos = min(LineCount * 30 - window_height, scrollPos + 10);
            break;
        case SB_THUMBPOSITION:
            break;
        case SB_THUMBTRACK:
            scrollPos = HIWORD(wParam);
            break;
        }

        SetScrollPos(hwnd, SB_VERT, scrollPos, TRUE);
        UpdateControlsPositions();
        return 0;

    case WM_MOUSEWHEEL:
                    //  This code handles all of the mouse wheel logic and sends messages to WM_VSCROLL.
                    //  'delta' contains info about whether the user is scrolling up or down with thier mouse wheel.
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
      
        if (delta > 0)
        {
            if (scrollPos <= 0) break;      //  This code prevents the user from scrolling more than the scrollbar can.
            SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
        }
        else
        {
            if (scrollPos >= LineCount * 30 - window_height) break;
            SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
        }
        break;

    case WM_PAINT:
                //  This redraws the background everytime its needed.
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HWND hChild = GetWindow(hwnd, GW_CHILD);
        while (hChild) {
            RECT rect;
            GetWindowRect(hChild, &rect);
            MapWindowPoints(NULL, hwnd, (LPPOINT)&rect, 2);                     // Convert the rectangle from screen coordinates to client coordinates.
            ExcludeClipRect(hdc, rect.left, rect.top, rect.right, rect.bottom); // Exclude the rectangle from the drawing area.
            hChild = GetWindow(hChild, GW_HWNDNEXT);                            // Get the next HWND.
        }

                // Redraw the background color.
        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &ps.rcPaint, brush);
        DeleteObject(brush);
        EndPaint(hwnd, &ps);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void InsertLine(HWND hwnd)
{
            //  This function takes the 'hwnd' and inserts it right after the last interacted line.
    if (last_interacted_get() == NULL)
    {
        SetFocus(GetWindow(hwnd, GW_CHILD));
        return;
    }

    SetWindowPos(hwnd, last_interacted_get(), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    SetFocus(GetWindow(hwnd, GW_CHILD));
}

        //  Only god knows how this fucntion works.
        //  I just know that it can add HWNDs dynamically to an array of HWNDs.
        //  It also resizes that same array.
void addHWND(HWND **hwndList, size_t *size, size_t *capacity, HWND hwnd) {
    if (*size >= *capacity)
    {
        *capacity *= 2;
        *hwndList = (HWND *)realloc(*hwndList, *capacity * sizeof(HWND));
    }
    (*hwndList)[*size] = hwnd;
    (*size)++;
}

void UpdateControlsPositions()
{
            //  This function stores all of the lines and then sorts their position accordingly.
    HWND hwndChild = GetWindow(hContainer, GW_CHILD);
    int pos_y = 0;

    HWND *hwndList = (HWND *)malloc(10 * sizeof(HWND));
    size_t size = 0;
    size_t capacity = 10;

    while (hwndChild)
    {       //  Store all of the Lines.
        addHWND(&hwndList, &size, &capacity, hwndChild);
        hwndChild = GetWindow(hwndChild, GW_HWNDNEXT);
    }

    for (size_t i = 0; i < size; ++i)
    {       //  Sort all of the Lines.
        int new_y = (pos_y * 30) - scrollPos;
        SetWindowPos(hwndList[i], NULL, 0, new_y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

        pos_y++;
    }

    free(hwndList);     // Cleanup memory.

    InvalidateRect(hContainer, NULL, TRUE);
}

void CreateCheckBox(HWND hwnd, LPCSTR lpcstr, BOOL CHECKED)
{
            //  This function creates the 'Lines' (refered to as 'CheckBoxes').
    int pos_y = 30 * LineCount;
    int ID = 202 + LineCount;

    HMENU hmenu = (HMENU)(uintptr_t)ID;

            //  This if statement is for debugging purposes.
            //  Do CreateCheckBox() with "*test" as the string.
    if (lstrcmp(lpcstr, "*test") == 0)
    {
        char buffer[100];
        sprintf(buffer, "Button ID %d", hmenu);
        lpcstr = (LPCSTR)buffer;
    }

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);
    int width = windowRect.right - windowRect.left;

            //  Create the checkbutton and check it if it's suppose to (BOOL CHECKED).
    HWND checkbox_hwnd = CreateWindowEx(0, "BUTTON", NULL,
        WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
        0, pos_y, width - 17, 30, hwnd, hmenu, NULL, NULL);
    
    if (CHECKED == TRUE)
    {
        SendMessage(checkbox_hwnd, BM_SETCHECK, BST_CHECKED, 0);
    }
            //  Add custom userdata to the button for additional identification.
    SetWindowLongPtr(checkbox_hwnd, GWLP_USERDATA, ID);

            //  Create the edit control (writeable text area), and input text (LPCSTR lpcstr).
    HWND edit_hwnd = CreateWindowEx(0, "EDIT", lpcstr,
        WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL,
        20, 5, width - 37 - 27, 25,
        checkbox_hwnd, (HMENU)1, NULL, NULL);
            //  This sets a custom procedure for the edit control. See 'EditProc()'.
    OriginalEditProc = (WNDPROC)SetWindowLongPtr(edit_hwnd, GWLP_WNDPROC, (LONG_PTR)EditProc);

            //  Create the move line up button.
    HWND button_hwnd = CreateWindowEx(0, "BUTTON", NULL,
        WS_VISIBLE | WS_CHILD,
        width - 37, 0, 20, 15,
        checkbox_hwnd, (HMENU)(uintptr_t)ID_BUTTON_MOVEUP, NULL, NULL);
            //  Custom procedure, see 'MoveButtonProc()';
    SetWindowLongPtr(button_hwnd, GWLP_WNDPROC, (LONG_PTR)MoveButtonProc);

            //  Create the move line down button.
    button_hwnd = CreateWindowEx(0, "BUTTON", NULL,
        WS_VISIBLE | WS_CHILD,
        width - 37, 15, 20, 15,
        checkbox_hwnd, (HMENU)ID_BUTTON_MOVEDOWN, NULL, NULL);
            //  Custom procedure, see 'MoveButtonProc()';
    SetWindowLongPtr(button_hwnd, GWLP_WNDPROC, (LONG_PTR)MoveButtonProc);
    

    InsertLine(checkbox_hwnd);
    UpdateControlsPositions();
    LineCount++;
    UpdateScrollBar(hwnd, LineCount);
}

LRESULT CALLBACK EditProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{           //  This is the edit's (text area) logic handler.
    switch (uMsg) {
    case WM_SETFOCUS:
        last_interacted_update(GetParent(GetParent(hwnd)), GetDlgCtrlID(GetParent(hwnd)));
        break;

    case WM_KEYDOWN:
        if ((GetKeyState(VK_CONTROL) & 0x8000) && wParam == 'A')    //  CTRL + A shortcut.
        {
            SendMessage(hwnd, EM_SETSEL, 0, -1);    // Select all text in the edit control.
            return 0;
        }
        break;
    }
            // Call 'OriginalEditProc'(EditProc()) for default processing.
    return CallWindowProc(OriginalEditProc, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MoveButtonProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int HoverState = 0;
    //  HoverState represents the state of the button in terms of mouse interacion,
    //  0 - default.
    //  1 - mouse is hovering over.
    //  2 - mouse is pressing the button.

    switch (uMsg) {
    case WM_MOUSEMOVE:
        if (HoverState != 1) {
            HoverState = 1;
            InvalidateRect(hwnd, NULL, TRUE); // Request a redraw
        }

        //  Track the mouse's movement even outside of the control.
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = hwnd;
        TrackMouseEvent(&tme);
        break;

    case WM_LBUTTONDOWN:
        HoverState = 2;
        InvalidateRect(hwnd, NULL, TRUE); // Request a redraw
        break;

    case WM_MOUSELEAVE:
        HoverState = 0;
        InvalidateRect(hwnd, NULL, TRUE); // Request a redraw
        break;

    case WM_LBUTTONUP:    //  Forward every message from here, to the main function "GetIt.C"
        HoverState = 0;
        InvalidateRect(hwnd, NULL, TRUE); // Request a redraw

                //  These next if statements responsible for moving the Line up or down.
                //  It changes their Z-order so that 'UpdateControlsPositions()' can sort them accuratally.
        if (GetDlgCtrlID(hwnd) == ID_BUTTON_MOVEUP)
        {
            HWND neighborHwnd = GetWindow(GetParent(hwnd), GW_HWNDPREV);
            if (neighborHwnd)
            {
                SetWindowPos(neighborHwnd, GetParent(hwnd), 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                
            }
        }
        if (GetDlgCtrlID(hwnd) == ID_BUTTON_MOVEDOWN)
        {
            HWND neighborHwnd = GetWindow(GetParent(hwnd), GW_HWNDNEXT);
            if (neighborHwnd)
            {
                SetWindowPos(GetParent(hwnd), neighborHwnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
            }
        }
        UpdateControlsPositions();
        break;

    case WM_PAINT:
                //  This just calls the 'DrawFlatButton()' function that handles all the graphic's logic.
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DrawFlatButton(hwnd, hdc, HoverState);
        EndPaint(hwnd, &ps);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void clear_line_count()
{
    LineCount = 0;
    scrollPos = 0;
}

void LoadImagesFromRes()
{
            //  Loads images from the resource file.
    if (Img_ArrowUp != NULL && Img_ArrowDown != NULL) return;
    Img_ArrowUp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ARWUP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    Img_ArrowUp_h = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ARWUP_H), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    Img_ArrowUp_p = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ARWUP_P), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    Img_ArrowDown = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ARWDN), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    Img_ArrowDown_h = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ARWDN_H), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    Img_ArrowDown_p = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ARWDN_P), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
}