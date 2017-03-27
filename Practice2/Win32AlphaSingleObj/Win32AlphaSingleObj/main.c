/*  Project:	Win32AlphaSingleObj
    File:		main.c
	Author:		刘芳
	Date:		2015-9-20
	Purpose:	在窗口里静态绘制一个对象，展示使用Alpha绘图的流程 */

#include <Windows.h>
#include <tchar.h>
#include "AEEngine.h"	// AlphaEngine

// Alpha Libray
#pragma comment (lib, "Alpha_Engine.lib")

// 该变量表示游戏状态，控制一个最简单的游戏流程（运行与退出两个状态）
int GS_Running = 1;

//  Windows过程函数，用于处理获取的消息(事件)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 程序入口
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wndClass;		// 窗口类：定义
	HWND hWnd;				// 窗口实例：创建
	// MSG msg = {0};		// 消息获取由Alpha完成，因此不使用该变量

	/* 使用Alpha绘制对象的变量声明 */
	float obj1X = 0.0f, obj1Y = 0.0f;		// 对象1的位置
	AEGfxVertexList*	pMesh1;				// 对象1的网格(mesh)模型
	float alpha = 1.0f;						// 透明度，取值范围[0,1]

	/* Alpha系统定义 */
	AESysInitInfo sysInitInfo;
	sysInitInfo.mAppInstance		= hInstance;	// WinMain的第1个参数
	sysInitInfo.mShow				= nCmdShow;		// WinMain的第4个参数
	sysInitInfo.mWinWidth			= 800; 
	sysInitInfo.mWinHeight			= 600;
	sysInitInfo.mCreateConsole		= 1;			// 是否需要打开控制台
	sysInitInfo.mCreateWindow		= 0;			// 是否需要创建窗口
	sysInitInfo.mMaxFrameRate		= 60;			// 设置帧率（如果使用Alpha的帧率控制功能的话）
	sysInitInfo.mpWinCallBack		= NULL;			// 指定窗口过程函数（因为没用Alpha创建窗口，因此设置为NULL）
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;		// 窗口类定义的重绘方式									
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;			// 窗口风格，取值：WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	// Part1 窗口定义  （与Alpha系统统一）
	wndClass.style = sysInitInfo.mClassStyle;	// 水平重绘和垂直重绘
	wndClass.lpfnWndProc = WndProc;				// 指向窗口过程函数
	wndClass.cbClsExtra = 0;					// 分配给该窗口类的附加内存
	wndClass.cbWndExtra = 0;					// 分配给窗口实例的附加内存
	wndClass.hInstance = sysInitInfo.mAppInstance;		// 程序的实例句柄，WinMain的第1个参数
	wndClass.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);  // 图标句柄，窗口左上角的图标，可以使用自己的图标资源
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	   // 光标句柄
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景画刷
	wndClass.lpszMenuName = NULL;				// 字符串，菜单资源的名字
	wndClass.lpszClassName = _T("Window Class Name");     // 指定窗口类的名字						

	// Part2 窗口注册
	RegisterClass(&wndClass);

	// Part3 窗口创建
	hWnd = CreateWindow(wndClass.lpszClassName,_T("Win32窗口应用示例"),WS_OVERLAPPEDWINDOW,200,100,640,480,NULL,NULL,hInstance,NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 通知Alpha系统窗口句柄		
	sysInitInfo.mWindowHandle = hWnd;

	// Alpha系统初始化 及 模块重置
	if(0 == AESysInit(&sysInitInfo))
		printf("System Init Failed!\n");
	AESysReset();

	// 开始添加对象1，
	// 注意一定要MeshStart开始，MeshEnd结束
	AEGfxMeshStart();
	/* 添加三角形，每一行是三角形一个顶点的定义，分别是
	   顶点坐标X，顶点坐标Y，颜色，纹理矩形的顶点X，纹理矩形的顶点Y
	   其中，纹理矩形用于定义纹理加载区域，
	         纹理矩形的左上顶点：0，0；右上顶点：1，0
	                   左下顶点：0，1；右下顶点：1，1  */
	AEGfxTriAdd(
		-25.5f, -25.5f, 0xFFFF0000, 0.0f, 0.0f, 
		25.5f,  0.0f, 0x00FF0000, 0.0f, 0.0f,
		-25.5f,  25.5f, 0xFFFFFF00, 0.0f, 0.0f);
	// 创建对象pMesh1
	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 1!!");

	// 为开始画对象做准备
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);   // 背景色RGB
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// Part4 消息
	while(GS_Running)
	{
		// Alpha - 帧开始
		AESysFrameStart();	

		/*while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				GS_Running = 0;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}*/

		// Drawing object 1
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		// Set position for object 1
		AEGfxSetPosition(obj1X, obj1Y);
		// No texture for object 1
		AEGfxTextureSet(NULL, 0, 0);
		// Drawing the mesh (list of triangles)
		AEGfxSetTransparency(alpha);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);

		// Alpha - 帧结束
		AESysFrameEnd();
	}

	// Part5 窗口类注销,Alpha资源注销
	AEGfxMeshFree(pMesh1);		// 注销创建的对象
	AESysExit();				// Alpha系统退出
	UnregisterClass(wndClass.lpszClassName, hInstance);

	return 0;
}

// Part6 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	HDC dc;           // 设备
	PAINTSTRUCT ps;   // 重绘结构

	switch (msg)
	{
		//  窗口创建
		case WM_CREATE:
			break;

		case WM_LBUTTONDOWN:
			//GS_Running = 0;
			break;
		
		case WM_MOUSEMOVE:
			break;

		// 重绘
		case WM_PAINT:
			dc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		// 窗口关闭
		case WM_DESTROY:
			//PostQuitMessage(0);
			GS_Running = 0;
			break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
				//PostQuitMessage(0);
				GS_Running = 0;
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