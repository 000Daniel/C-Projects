#include "GetIt.h"
#include "Elements.h"
#include "Functions.h"

const char g_szClassName[] = "MainWindow";
HWND g_hToolbar = NULL;

HWND hContainer;

const int Window_Width = 360;
const int Window_Height = 500;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:
        {
            hContainer = CreateContainer(hwnd,lParam);
        }
        case WM_COMMAND:
                    //  Every button thats pressed in the title bar, is handled here.
            switch (LOWORD(wParam))
            {
            case ID_BUTTON1:    //  Exit button
                DestroyWindow(hwnd);
                break;
            case ID_BUTTON2:    // Open button
                Read_File_Function(hwnd);
                break;
            case ID_BUTTON3:    //  Delete button
                last_interacted_delete();
                break;
            case ID_BUTTON4:    //  Add button
                CreateCheckBox(hContainer, NULL, FALSE);
                break;
            case ID_BUTTON5:    //  Save button
                Save_File_Function(hwnd);
                break;
            case ID_BUTTON6:    //  Minimize button
                SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
                break;
            }
            break;

        case WM_LBUTTONDOWN:    //  Allow the user to drag around the program.
           SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        break;

        case WM_PAINT:          //  Draw funtion: redraw the background and redraw the title bar.
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rect;          
            GetClientRect(hwnd, &rect);
            HBRUSH hBrush = CreateSolidBrush(COL_BACKGROUND);
            FillRect(hdc, &rect, hBrush);   //  Redraw the background color with 'COL_BACKGROUND' as the color.
      
            SetRect(&rect, 200,0,rect.right - 50,25);
            hBrush = CreateSolidBrush(COL_TITLEBAR);
            FillRect(hdc, &rect, hBrush);   //  Redraw the titlebar with 'COL_TITLEBAR' as the color.
            DeleteObject(hBrush);

            EndPaint(hwnd, &ps);
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
        break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = WHITE_BRUSH;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MAINICON));

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        g_szClassName,
        "Get It",
        WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT, Window_Width, Window_Height,
        NULL, NULL, hInstance, NULL);

        //  'CreateButton()' creates buttons.
        //  Depending on their ID it knows what properties to assign.
        //  Learn more in Elements.C.
    CreateButton(hwnd, hInstance, Window_Width, Window_Height, ID_BUTTON1);
    CreateButton(hwnd, hInstance, Window_Width, Window_Height, ID_BUTTON2);
    CreateButton(hwnd, hInstance, Window_Width, Window_Height, ID_BUTTON3);
    CreateButton(hwnd, hInstance, Window_Width, Window_Height, ID_BUTTON4);
    CreateButton(hwnd, hInstance, Window_Width, Window_Height, ID_BUTTON5);
    CreateButton(hwnd, hInstance, Window_Width, Window_Height, ID_BUTTON6);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    RecenterWindow(hwnd);       //  Bring the program to the center of the screen on startup.
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}