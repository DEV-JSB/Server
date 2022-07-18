// Win_Client.cpp : Defines the entry point for the application.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<WinSock2.h>    
#pragma comment(lib,"ws2_32.lib")

#include "framework.h"
#include "framework.h"
#include "Win_Client.h"

#include <list>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




#define BUFFER_SIZE 128
#define WM_ASYNC WM_USER + 1


WSADATA wsadata;
SOCKET s;
SOCKADDR_IN addr = { 0 };
TCHAR msg[BUFFER_SIZE * 2], str[BUFFER_SIZE];
int size, msgLen, msgCount = 0;
char buffer[BUFFER_SIZE];




int InitClient(HWND _hwnd)
{
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    s = socket(AF_INET, SOCK_STREAM, 0);


    addr.sin_family = AF_INET;
    addr.sin_port = htons(20);
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)));
    WSAAsyncSelect(s, _hwnd, WM_ASYNC, FD_READ);

    return 1;
}
int CloseClient()
{
    closesocket(s);
    WSACleanup();
    return 0;
}
//SOCKET AcceptSocket(HWND _hwnd, SOCKET _s, SOCKADDR_IN& _c_addr)
//{
//
//}
int SendMessageToServer()
{
    if (s == INVALID_SOCKET)
        return 0;
#ifdef _UNICODE
    msgLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, str, -1, buffer, msgLen, NULL, NULL);
    msg[msgLen] = NULL;
    //_stprintf_s(message, _T("Client Message : %s , 서버 종료 ? "), msg);
#else
    strcpy_s(buffer, str);
    msgLen = strlen(buffer);
    //sprintf(message, _T("Client Message : %s , 서버 종료 ? "), buffer);
#endif
    send(s, (LPSTR)buffer, msgLen + 1, 0);
    msgCount = 0;
    return 1;
}
void ReadMessageToServer()
{
}
void CloseClient(SOCKET _socket)
{
}


int WinSockTest_Client()
{
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s == INVALID_SOCKET)
    {
        MessageBox(NULL, _T("Socket failded"), _T("Error"), MB_OK);
        return 0;

    }
    else
        MessageBox(NULL, _T("Socket Success"), _T("Success"), MB_OK);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(20);
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        MessageBox(NULL, _T("Connect failded"), _T("Error"), MB_OK);
        return 0;

    }
    else
        MessageBox(NULL, _T("Connect Success"), _T("Success"), MB_OK);

    send(s, "안녕 server !", 12, 0);

    closesocket(s);
    WSACleanup();

    return 1;
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    //return WinSockTest_Client();





    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINCLIENT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINCLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINCLIENT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        InitClient(hWnd);
    }
    break;
    case WM_ASYNC:
    {
        switch (lParam)
        {
        case FD_READ:
            msgLen = recv(s, buffer, BUFFER_SIZE, 0);
            buffer[msgLen] = NULL;
#ifdef _UNICODE
            msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), 0, 0);
            MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
            msg[msgLen] = NULL;
            //_stprintf_s(message, _T("Client Message : %s , 서버 종료 ? "), msg);
#else
            strcpy_s(msg, buffer);
#endif
            InvalidateRect(hWnd, NULL, true);
            break;
        }
    }
    break;
    case WM_CHAR:
        if (wParam == VK_RETURN)
        {

            return SendMessageToServer();
        }
        str[msgCount++] = wParam;
        str[msgCount] = NULL;
        InvalidateRect(hWnd, NULL, true);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 10, 10, str, _tcslen(str));
        if (_tcscmp(msg, _T("")))
            TextOut(hdc, 20, 20, msg, _tcslen(msg));

        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        CloseClient();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}