/*  Project:	Win32Sample
    File:		main.c
	Author:		����
	Date:		2015-9-16
	Purpose:	Win32Ӧ�ã����ڴ���  */

#include <Windows.h>
#include <tchar.h>
#include <stdbool.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// �������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wndClass;		// �����ࣺ��������
	HWND hWnd;				// ����ʵ��������
	MSG msg;				// ��Ϣ
	bool loop=true;

	// Part1 ���ڶ���
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	// ˮƽ�ػ�ʹ�ֱ�ػ�
	wndClass.lpfnWndProc = WndProc;				// ָ�򴰿ڹ��̺������ص�������
	wndClass.cbClsExtra = 0;					// ������ô�����ĸ����ڴ�
	wndClass.cbWndExtra = 0;					// ���������ʵ���ĸ����ڴ�
	wndClass.hInstance = hInstance;				// �����ʵ�������WinMain�ĵ�1������
	wndClass.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);  // ͼ�������������Ͻǵ�ͼ�꣬����ʹ���Լ���ͼ����Դ
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	   // �����
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //������ˢ
	wndClass.lpszMenuName = NULL;				// �ַ������˵���Դ������
	wndClass.lpszClassName = _T("My Window Class");     // ָ�������������						

	// Part2 ����ע��
	RegisterClass(&wndClass);

	/* Part3 ���ڴ���
	hWnd = CreateWindow( wndClass.lpszClassName, 
						"Win32����Ӧ��ʾ��",		 ���ڱ��� 
						WS_OVERLAPPEDWINDOW,		 ���ڷ��
						200��						 �������Ͻ�X���꣬��Ļ���Ͻ���ԭ��(0,0)
						100,						 �������Ͻ�Y����
						640��						 ���ڿ��
						480��						 ���ڸ߶�
						NULL��						 �����ڵľ��������ô������Ӵ��ڣ�
						NULL��						 �˵����
						hInstance,					 WinMain�ĵ�1������
						NULL );						 WM_Create��Ϣ�����õ���*/
	hWnd = CreateWindow(wndClass.lpszClassName,_T("Win32����Ӧ��ʾ��"),WS_OVERLAPPEDWINDOW,200,100,640,480,NULL,NULL,hInstance,NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Part4 ��Ϣ
	// ������յ���Ϣ��WM_QUIT������0�����򷵻ط�0
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

	// Part5 ������ע��
	UnregisterClass(wndClass.lpszClassName, hInstance);

	return 0;
}

// Part6 ���ڹ��̺���
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	HDC dc;           // �豸
	PAINTSTRUCT ps;   // �ػ�ṹ
	HFONT hFont;	 

	switch (msg)
	{
		//  ���ڴ���
		case WM_CREATE:
			break;

		case WM_LBUTTONDOWN:
			break;
		
		case WM_MOUSEMOVE:
			break;

		// �ػ�
		case WM_PAINT:
			dc = BeginPaint(hWnd, &ps);

			// ���´��������ı���test���Ļ���
			hFont = CreateFont(30,0,0,0,0,0,0,0,GB2312_CHARSET,0,0,0,0,L"΢���ź�");
			SelectObject(dc,hFont);
			SetBkMode(dc,TRANSPARENT);
			SetTextColor(dc,RGB(0,255,255));
			TextOut(dc,20,200,L"test",wcslen(L"test"));


			EndPaint(hWnd, &ps);
			break;

		// ���ڹر�
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
				PostQuitMessage(0);
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