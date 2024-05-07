// gcprocess.cpp : 定义应用程序的入口点。

#include "framework.h"
#include "gcprocess.h"
#include "gxControlProcess.h"
#include "gxStr.h"
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
gxControlProcess gx;
gxStr gxstr;

// 所有进程信息
std::vector<ProcessInfo> gxprocesses;
// 窗口列表类句柄
HWND hList;
// 搜索结果列表框句柄
HWND hNewListBox;

void PerformSearch(const std::wstring& searchText);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    
    // bool f = gx.gxPauseProcess(18580);
    // bool f = gx.gxResumeProcess(18580);
    /*if (f) {
        printf("成功关闭");
    }*/
    
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GCPROCESS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GCPROCESS));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GCPROCESS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GCPROCESS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {

        // 创建列表框
        hList = CreateWindowEx(0, L"LISTBOX", nullptr,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_STANDARD,
            gxProcessX, gxProcessY, gxProcessLength, gxProcessWidth,
            hWnd, reinterpret_cast<HMENU>(IDC_PROCESSLISTBOX), nullptr, nullptr);
        // 将字体应用到列表框
        SendMessage(hList, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
        // 获取进程信息
        gxprocesses = gx.gxGetAllProcesses();

        // 将进程信息添加到列表框中
        for (const auto& process : gxprocesses)
        {
            std::wstring processInfo = L"Process ID:" + std::to_wstring(process.processId) + L", " + process.processName;
            SendMessage(hList, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(processInfo.c_str()));
        }

        // 创建搜索框控件
        HWND hSearchBox = CreateWindowEx(0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            gxSearchX, gxSearchY, gxSearchLength, gxSearchWidth, hWnd, reinterpret_cast<HMENU>(IDC_SEARCH_BOX), nullptr, nullptr);

        // 创建搜索按钮控件
        HWND hSearchButton = CreateWindow(L"BUTTON", L"搜索", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            gxSearchButtonX, gxSearchButtonY, gxSearchButtonLength, gxSearchButtonWidth, hWnd, reinterpret_cast<HMENU>(IDC_SEARCH_BUTTON), nullptr, nullptr);
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            // 处理列表框的消息
            case IDC_PROCESSLISTBOX: // 假设列表框的ID是 IDC_YOURLISTBOX
                if (wmEvent == LBN_SELCHANGE) // 用户选择了新的项
                {
                    HWND hListBox = reinterpret_cast<HWND>(lParam);
                    int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                    if (selectedIndex != LB_ERR)
                    {
                        // 获取选定项的文本
                        wchar_t buffer[256]; // 假设文本不超过 256 个字符
                        SendMessage(hListBox, LB_GETTEXT, selectedIndex, reinterpret_cast<LPARAM>(buffer));
                        // 将文本和下标格式化为一个字符串
                        int gxProcessId = gxstr.extractID(buffer);
                        wchar_t message[512]; // 假设消息不超过 512 个字符
                        swprintf_s(message, 512, L"选定项文本：%s\n选定项下标：%d\n 进程号 %d", buffer, selectedIndex, gxProcessId);

                        // 弹出消息框显示选定项的文本和下标
                        MessageBox(hWnd, message, L"选定项信息", MB_OK | MB_ICONINFORMATION);
                    }
                }
                break;

            // 处理搜索请求
            case IDC_SEARCH_BUTTON:
            {
                // 搜索按钮被点击
    
                wchar_t message[512]; // 假设消息不超过 512 个字符
                wchar_t buffer[256];
                GetWindowText(GetDlgItem(hWnd, IDC_SEARCH_BOX), buffer, 256);
                std::wstring gxsearchText(buffer);
                swprintf_s(message, 512, L"选定项文本：%s", buffer);
                MessageBox(hWnd, message, L"选定项信息", MB_OK | MB_ICONINFORMATION);

                // 搜索按钮被点击，执行搜索操作
                PerformSearch(gxsearchText);
                break;
            }

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
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
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

void PerformSearch(const std::wstring& searchText) {
    int n = gxprocesses.size();
    SendMessage(hList, LB_RESETCONTENT, 0, 0);
    // 遍历列表框中的所有项
    for (int i = 0; i < n; ++i) {
        std::wstring processInfo = L"Process ID:" + std::to_wstring(gxprocesses[i].processId) + L", " + gxprocesses[i].processName;
        // 如果当前项包含搜索文本，则选中该项
        if (wcsstr(processInfo.c_str(), searchText.c_str()) != nullptr) {
            SendMessage(hList, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(processInfo.c_str()));
        }
    }

}