/*  Project:	Win32AlphaSingleObj
    File:		main.c
	Author:		����
	Date:		2015-9-20
	Purpose:	�ڴ����ﾲ̬����һ������չʾʹ��Alpha��ͼ������ */

#include <Windows.h>
#include <tchar.h>
#include "AEEngine.h"	// AlphaEngine

// Alpha Libray
#pragma comment (lib, "Alpha_Engine.lib")

// �ñ�����ʾ��Ϸ״̬������һ����򵥵���Ϸ���̣��������˳�����״̬��
int GS_Running = 1;

//  Windows���̺��������ڴ����ȡ����Ϣ(�¼�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// �������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wndClass;		// �����ࣺ����
	HWND hWnd;				// ����ʵ��������
	// MSG msg = {0};		// ��Ϣ��ȡ��Alpha��ɣ���˲�ʹ�øñ���

	/* ʹ��Alpha���ƶ���ı������� */
	float obj1X = 0.0f, obj1Y = 0.0f;		// ����1��λ��
	AEGfxVertexList*	pMesh1;				// ����1������(mesh)ģ��
	float alpha = 1.0f;						// ͸���ȣ�ȡֵ��Χ[0,1]

	/* Alphaϵͳ���� */
	AESysInitInfo sysInitInfo;
	sysInitInfo.mAppInstance		= hInstance;	// WinMain�ĵ�1������
	sysInitInfo.mShow				= nCmdShow;		// WinMain�ĵ�4������
	sysInitInfo.mWinWidth			= 800; 
	sysInitInfo.mWinHeight			= 600;
	sysInitInfo.mCreateConsole		= 1;			// �Ƿ���Ҫ�򿪿���̨
	sysInitInfo.mCreateWindow		= 0;			// �Ƿ���Ҫ��������
	sysInitInfo.mMaxFrameRate		= 60;			// ����֡�ʣ����ʹ��Alpha��֡�ʿ��ƹ��ܵĻ���
	sysInitInfo.mpWinCallBack		= NULL;			// ָ�����ڹ��̺�������Ϊû��Alpha�������ڣ��������ΪNULL��
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;		// �����ඨ����ػ淽ʽ									
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;			// ���ڷ��ȡֵ��WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	// Part1 ���ڶ���  ����Alphaϵͳͳһ��
	wndClass.style = sysInitInfo.mClassStyle;	// ˮƽ�ػ�ʹ�ֱ�ػ�
	wndClass.lpfnWndProc = WndProc;				// ָ�򴰿ڹ��̺���
	wndClass.cbClsExtra = 0;					// ������ô�����ĸ����ڴ�
	wndClass.cbWndExtra = 0;					// ���������ʵ���ĸ����ڴ�
	wndClass.hInstance = sysInitInfo.mAppInstance;		// �����ʵ�������WinMain�ĵ�1������
	wndClass.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);  // ͼ�������������Ͻǵ�ͼ�꣬����ʹ���Լ���ͼ����Դ
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	   // �����
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //������ˢ
	wndClass.lpszMenuName = NULL;				// �ַ������˵���Դ������
	wndClass.lpszClassName = _T("Window Class Name");     // ָ�������������						

	// Part2 ����ע��
	RegisterClass(&wndClass);

	// Part3 ���ڴ���
	hWnd = CreateWindow(wndClass.lpszClassName,_T("Win32����Ӧ��ʾ��"),WS_OVERLAPPEDWINDOW,200,100,640,480,NULL,NULL,hInstance,NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// ֪ͨAlphaϵͳ���ھ��		
	sysInitInfo.mWindowHandle = hWnd;

	// Alphaϵͳ��ʼ�� �� ģ������
	if(0 == AESysInit(&sysInitInfo))
		printf("System Init Failed!\n");
	AESysReset();

	// ��ʼ��Ӷ���1��
	// ע��һ��ҪMeshStart��ʼ��MeshEnd����
	AEGfxMeshStart();
	/* ��������Σ�ÿһ����������һ������Ķ��壬�ֱ���
	   ��������X����������Y����ɫ��������εĶ���X��������εĶ���Y
	   ���У�����������ڶ��������������
	         ������ε����϶��㣺0��0�����϶��㣺1��0
	                   ���¶��㣺0��1�����¶��㣺1��1  */
	AEGfxTriAdd(
		-25.5f, -25.5f, 0xFFFF0000, 0.0f, 0.0f, 
		25.5f,  0.0f, 0x00FF0000, 0.0f, 0.0f,
		-25.5f,  25.5f, 0xFFFFFF00, 0.0f, 0.0f);
	// ��������pMesh1
	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 1!!");

	// Ϊ��ʼ��������׼��
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);   // ����ɫRGB
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// Part4 ��Ϣ
	while(GS_Running)
	{
		// Alpha - ֡��ʼ
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

		// Alpha - ֡����
		AESysFrameEnd();
	}

	// Part5 ������ע��,Alpha��Դע��
	AEGfxMeshFree(pMesh1);		// ע�������Ķ���
	AESysExit();				// Alphaϵͳ�˳�
	UnregisterClass(wndClass.lpszClassName, hInstance);

	return 0;
}

// Part6 ���ڹ��̺���
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	HDC dc;           // �豸
	PAINTSTRUCT ps;   // �ػ�ṹ

	switch (msg)
	{
		//  ���ڴ���
		case WM_CREATE:
			break;

		case WM_LBUTTONDOWN:
			//GS_Running = 0;
			break;
		
		case WM_MOUSEMOVE:
			break;

		// �ػ�
		case WM_PAINT:
			dc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		// ���ڹر�
		case WM_DESTROY:
			//PostQuitMessage(0);
			GS_Running = 0;
			break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
				//PostQuitMessage(0);
				GS_Running = 0;
			break;

		// ���ڷ����ƶ�
		case WM_MOVE:
			/* Invalidate the rect to force a redraw */
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		
		// ����Ĭ�ϴ��ڹ���
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}