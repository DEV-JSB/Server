// __Client.cpp : Define About Application Entry Point
//

#include "framework.h"
#include "__Client.h"

#define MAX_LOADSTRING 100
#define WM_ASYNC WM_USER+2

#define BULLET 3

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


// 전역 변수:
HINSTANCE hInst;                                // Current Instance
WCHAR szTitle[MAX_LOADSTRING];                  // About Titlebar Text
WCHAR szWindowClass[MAX_LOADSTRING];            // Basic Window Class Name

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // Moved to WndProc ( WM_CREATE )
    /*/
    WSADATA wsadata;
    SOCKET  s;
    SOCKADDR_IN addr = { 0 };

    WSAStartup(MAKEWORD(2, 2), &wsadata);

    s = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = 20;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == -1)
        return 0;   // Connect Failed

    // Connect Success
    // TODO : Write Code in this area.
    send(s, "Hello Server!", strlen("Hello Server!"), 0);

    closesocket(s);
    WSACleanup();

    return 1;
    //*/
    //*/ // Basic wWinMain
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
    //*/
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT rc = { 0, 0, 900, 600 };
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
typedef struct tagObject
{
    tagObject() { x = 0; y = 0; }
    tagObject(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    int x = 0;
    int y = 0;
    int sizex = 0;
    int sizey = 0;
    int type = 0;
}Object;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Object* Mine;
    static Object* enemy;
    static Object* bullet;

    static vector<Object*> vecObject;

    static POINT mouseUpdatePos;

    

    PAINTSTRUCT ps;
    static WSADATA wsadata;
    static SOCKET s;
    static SOCKADDR_IN addr = { 0 };
    int msgLen;
    char buffer[5000];

    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 0, 100, nullptr);
        WSAStartup(MAKEWORD(2, 2), &wsadata);
        s = socket(AF_INET, SOCK_STREAM, 0);
        addr.sin_family = AF_INET;
        addr.sin_port = 20;
        addr.sin_addr.s_addr = inet_addr("172.30.1.27");
        WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ);

        Mine = new Object(850, 300);
        enemy = new Object(50, 300);

        vecObject.push_back(Mine);
		vecObject.push_back(enemy);


        if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == -1)
            return 0;
        break;
    case WM_TIMER:
        {
            
        }
        break;
    case WM_PAINT:
        {
            HDC hdc = BeginPaint(hWnd, &ps);

            Ellipse(hdc, Mine->x - 50, Mine->y - 50, Mine->x + 50, Mine->y + 50);

            if (enemy != nullptr)
                Ellipse(hdc, enemy->x - 50, enemy->y - 50, enemy->x + 50, enemy->y + 50);


            for (int i = 0; i < vecObject.size();++i)
                Ellipse(hdc, vecObject[i]->x - 50, vecObject[i]->y - 50, vecObject[i]->x + 50, vecObject[i]->y + 50);



            EndPaint(hWnd, &ps);
        }
        break;
    case WM_MOUSEMOVE:
    {
        mouseUpdatePos.x = LOWORD(lParam);
        mouseUpdatePos.y = HIWORD(lParam);
        char* temp = reinterpret_cast<char*>(&mouseUpdatePos);

        if (send(s, temp, sizeof(Object), 0) == SOCKET_ERROR)
            int iSIBAL = 10;

    }
        break;
    case WM_ASYNC:
        switch (lParam)
        {
        case FD_READ:
            {
                size_t iCnt = 0;
                msgLen = recv(s, buffer, 5000, 0);
                
                Object* tmp = new Object;


                int* test1 = (int*)(*(reinterpret_cast<int*>(buffer)));


                *tmp = Object(*(reinterpret_cast<Object*>(buffer)));

                if (BULLET == tmp->type)
                    vecObject.push_back(tmp);
                else
                    *enemy = *tmp;


		    	InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        default:
            break;
        }
        break;
    case WM_RBUTTONDOWN:
        {
            POINT tmp;
            tmp.x = LOWORD(lParam);
            tmp.y = HIWORD(lParam);
            bullet = new Object();
            bullet->x = tmp.x;
            bullet->y = tmp.y;
            bullet->type = BULLET;

            
            char* temp = reinterpret_cast<char*>(bullet);

            if (send(s, temp, sizeof(Object), 0) == SOCKET_ERROR)
                int iSIBAL = 10;
            printf("생성이벤트 발생 ! \n");
            InvalidateRect(hWnd, nullptr, true);
        }
        break;
    case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case VK_ESCAPE:
                PostQuitMessage(0);
                break;
            case VK_UP:
                {
                    Mine->y -= 10;

                    char* temp = reinterpret_cast<char*>(Mine);

                    if (send(s, temp, sizeof(Object), 0) == SOCKET_ERROR)
                        int iSIBAL = 10;

                    InvalidateRect(hWnd, nullptr, true);
                }
                break;
			case VK_DOWN:
			    {
			    	Mine->y += 10;

			    	char* temp = reinterpret_cast<char*>(Mine);

			    	if (send(s, temp, sizeof(Object), 0) == SOCKET_ERROR)
			    		int iSIBAL = 10;

			    	InvalidateRect(hWnd, nullptr, true);
			    }
			    break;
            case VK_SPACE:
                {
                

                }
                break;
            }
        }
        break;
    case WM_DESTROY:
        closesocket(s);
        WSACleanup();
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 정보 대화 상자의 메시지 처리기입니다.
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
