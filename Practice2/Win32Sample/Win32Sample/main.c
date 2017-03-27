/*  Project:	Win32Sample
    File:		main.c
	Author:		刘芳
	Date:		2015-9-16
	Purpose:	Win32应用：窗口创建  */

#include <Windows.h>
#include <tchar.h>
#include <stdbool.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 程序入口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wndClass;		// 窗口类：定义属性
	HWND hWnd;				// 窗口实例：创建
	MSG msg;				// 消息
	bool loop=true;

	// Part1 窗口定义
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	// 水平重绘和垂直重绘
	wndClass.lpfnWndProc = WndProc;				// 指向窗口过程函数（回调函数）
	wndClass.cbClsExtra = 0;					// 分配给该窗口类的附加内存
	wndClass.cbWndExtra = 0;					// 分配给窗口实例的附加内存
	wndClass.hInstance = hInstance;				// 程序的实例句柄，WinMain的第1个参数
	wndClass.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);  // 图标句柄，窗口左上角的图标，可以使用自己的图标资源
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	   // 光标句柄
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景画刷
	wndClass.lpszMenuName = NULL;				// 字符串，菜单资源的名字
	wndClass.lpszClassName = _T("My Window Class");     // 指定窗口类的名字						

	// Part2 窗口注册
	RegisterClass(&wndClass);

	/* Part3 窗口创建
	hWnd = CreateWindow( wndClass.lpszClassName, 
						"Win32窗口应用示例",		 窗口标题 
						WS_OVERLAPPEDWINDOW,		 窗口风格
						200，						 窗口左上角X坐标，屏幕左上角是原点(0,0)
						100,						 窗口左上角Y坐标
						640，						 窗口宽度
						480，						 窗口高度
						NULL，						 父窗口的句柄（如果该窗口是子窗口）
						NULL，						 菜单句柄
						hInstance,					 WinMain的第1个参数
						NULL );						 WM_Create消息传递用到的*/
	hWnd = CreateWindow(wndClass.lpszClassName,_T("Win32窗口应用示例"),WS_OVERLAPPEDWINDOW,200,100,640,480,NULL,NULL,hInstance,NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Part4 消息
	// 如果接收的消息是WM_QUIT，返回0，否则返回非0
	/*while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/
	/*while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/
	while(loop)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				loop = false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//Game_Update(); Game_Render();
	}

	// Part5 窗口类注销
	UnregisterClass(wndClass.lpszClassName, hInstance);

	return 0;
}

// Part6 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	HDC dc;           // 设备
	PAINTSTRUCT ps;   // 重绘结构
	HFONT hFont;	 

	switch (msg)
	{
		//  窗口创建
		case WM_CREATE:
			break;

		case WM_LBUTTONDOWN:
			break;
		
		case WM_MOUSEMOVE:
			break;

		// 重绘
		case WM_PAINT:
			dc = BeginPaint(hWnd, &ps);

			// 以下代码用于文本“test”的绘制
			hFont = CreateFont(30,0,0,0,0,0,0,0,GB2312_CHARSET,0,0,0,0,L"微软雅黑");
			SelectObject(dc,hFont);
			SetBkMode(dc,TRANSPARENT);
			SetTextColor(dc,RGB(0,255,255));
			TextOut(dc,20,200,L"test",wcslen(L"test"));


			EndPaint(hWnd, &ps);
			break;

		// 窗口关闭
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
				PostQuitMessage(0);
			break;

		// 窗口发生移动
		case WM_MOVE:
			/* Invalidate the rect to force a redraw */
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		
		// 调用默认窗口过程
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}