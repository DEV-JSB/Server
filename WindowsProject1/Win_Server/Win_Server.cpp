// Win_Server.cpp : Defines the entry point for the application.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<WinSock2.h>    
#pragma comment(lib,"ws2_32.lib")

#include "framework.h"
#include "Win_Server.h"

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


// >> : 

#define BUFFER_SIZE 128
#define WM_ASYNC WM_USER + 1

std::list<SOCKET> socketList;

WSADATA wsadata;
SOCKET s, cs;
SOCKADDR_IN addr = { 0 }, c_addr = { 0 };
TCHAR msg[BUFFER_SIZE * 2];
int size, msgLen;
char buffer[BUFFER_SIZE];

int InitServer(HWND _hwnd)
{
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    s = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(20);
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    bind(s, (LPSOCKADDR)&addr, sizeof(addr));
    if (listen(s, 5) == SOCKET_ERROR)
        return 0;

    WSAAsyncSelect(s, _hwnd, WM_ASYNC, FD_ACCEPT);


    return 1;
}
int CloseServer()
{
    closesocket(s);
    WSACleanup();
    return 0;
}
SOCKET AcceptSocket(HWND _hwnd, SOCKET _s, SOCKADDR_IN& _c_addr)
{
    SOCKET cs;
    int size;
    size = sizeof(c_addr);
    cs = accept(s, (LPSOCKADDR)&c_addr, &size);
    WSAAsyncSelect(cs, _hwnd, WM_ASYNC, FD_READ | FD_CLOSE);


    socketList.push_back(cs);

    return cs;
}
void SendMessageToClient(char* _buffer)
{
    for (std::list<SOCKET>::iterator iter = socketList.begin(); iter != socketList.end(); ++iter)
    {
        SOCKET cs = (*iter);
        send(cs, (LPSTR)buffer, strlen(buffer) + 1, 0);
    }
}
void ReadMessage(TCHAR* _msg, char* _buffer)
{
    for (std::list<SOCKET>::iterator iter = socketList.begin(); iter != socketList.end(); ++iter)
    {
        SOCKET cs = (*iter);
        int msgLen = recv(cs, buffer, BUFFER_SIZE, 0);
        if (msgLen > 0)
        {
            buffer[msgLen] = NULL;
#ifdef _UNICODE
            msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), 0, 0);
            MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
            msg[msgLen] = NULL;
            //_stprintf_s(message, _T("Client Message : %s , 서버 종료 ? "), msg);
#else
            strcpy_s(msg, buffer);
            //sprintf(message, _T("Client Message : %s , 서버 종료 ? "), buffer);
#endif
            SendMessageToClient(buffer);
        }
    }
}
void CloseClient(SOCKET _socket)
{
    for (std::list<SOCKET>::iterator iter = socketList.begin(); iter != socketList.end(); ++iter)
    {
        SOCKET cs = (*iter);
        if (cs == _socket)
        {
            closesocket(cs);
            iter = socketList.erase(iter++);
            break;
        }
    }
}


int WinSockTest_Server()
{

    WSAStartup(MAKEWORD(2, 2), &wsadata);
    s = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(20);
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if (bind(s, (LPSOCKADDR)&addr, sizeof(addr)))
    {
        MessageBox(NULL, _T("Binding failded"), _T("Error"), MB_OK);
        return 0;
    }
    else
        MessageBox(NULL, _T("Binding Success"), _T("Success"), MB_OK);

    if (listen(s, 5) == SOCKET_ERROR)
    {
        MessageBox(NULL, _T("Listen failded"), _T("Error"), MB_OK);
        return 0;
    }
    else
        MessageBox(NULL, _T("Listen Success"), _T("Success"), MB_OK);
    TCHAR message[256];

    do
    {
        SOCKADDR_IN c_addr;
        int size = sizeof(c_addr);
        SOCKET cs = accept(s, (LPSOCKADDR)&c_addr, &size);

        char buffer[100];
        int msgLen;
        msgLen = recv(cs, buffer, 100, 0);
        buffer[msgLen] = NULL;

#ifdef _UNICODE
        TCHAR wBuffer[100];
        msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), 0, 0);
        MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), wBuffer, msgLen);
        wBuffer[msgLen] = NULL;
        _stprintf_s(message, _T("Client Message : %s , 서버 종료 ? "), wBuffer);
#else
        sprintf(message, _T("Client Message : %s , 서버 종료 ? "), buffer);
#endif

    } while (MessageBox(NULL, message,
        _T("Server Message"), MB_YESNO) == IDNO);


    closesocket(s);
    WSACleanup();

    return 0;
}
// <<

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    //return WinSockTest_Server();





    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINSERVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINSERVER));

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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINSERVER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINSERVER);
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
        return InitServer(hWnd);


        //        size = sizeof(c_addr);
        //        do
        //        {
        //            cs = accept(s, (LPSOCKADDR)&c_addr, &size);
        //        } while (cs == INVALID_SOCKET);
        //
        //        msgLen = recv(cs, buffer, 100, 0);
        //        buffer[msgLen] = NULL;
        //
        //#ifdef _UNICODE
        //        msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), 0, 0);
        //        MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
        //        msg[msgLen] = NULL;
        //        //_stprintf_s(message, _T("Client Message : %s , 서버 종료 ? "), msg);
        //#else
        //        strcpy_s(msg, buffer);
        //        //sprintf(message, _T("Client Message : %s , 서버 종료 ? "), buffer);
        //#endif
    }
    break;
    case WM_ASYNC:
        switch (lParam)
        {
        case FD_ACCEPT:
            cs = AcceptSocket(hWnd, s, c_addr);
            break;
        case FD_READ:
            ReadMessage(msg, buffer);
            InvalidateRect(hWnd, NULL, TRUE);

            break;
        case FD_CLOSE:
            CloseClient(wParam);
            break;
        }
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
        if (_tcscmp(msg, _T("")))
            TextOut(hdc, 10, 30, msg, (int)_tcslen(msg));
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        CloseServer();
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