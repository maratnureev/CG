#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

TCHAR const CLASS_NAME[] = _T("MainWndClass");
TCHAR const WINDOW_TITLE[] = _T("My first window");

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int MainLoop();
bool RegisterWndClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPSTR /*lpCmdLine*/,
	int nCmdShow)
{
	// Регистрируем класс главного окна
	if (!RegisterWndClass(hInstance))
	{
		return 1;
	}

	// Создаем главное окно приложения
	HWND hMainWindow = CreateMainWindow(hInstance);
	if (hMainWindow == NULL)
	{
		return 1;
	}

	// Показываем главное окно приложения
	ShowWindow(hMainWindow, nCmdShow);
	UpdateWindow(hMainWindow);

	// Запускаем цикл выборки сообщений, пока не получим 
	// сигнал о завершении приложения
	return MainLoop();
}

HWND CreateMainWindow(HINSTANCE hInstance)
{
	HWND hMainWindow = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	return hMainWindow;
}

int MainLoop()
{
	MSG msg;
	BOOL res;
	while ((res = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (res == -1)
		{
			// произошла ошибка - нужно обработать ее и, вероятно,
			// завершить работу приложения
		}
		else
		{
			// Если это сообщение о нажатии виртуальной клавиши,
			// то добавляем в очередь сообщений сообщения, несущие информацию о
			// коде вводимого пользователем символа
			TranslateMessage(&msg);
			// передаем сообщение в соответствующую оконную процедуру
			DispatchMessage(&msg);
		}
	}

	// сюда мы попадем только в том случае извлечения сообщения WM_QUIT
	// msg.wParam содержит код возврата, помещенный при помощи функции PostQuitMessage()
	return msg.wParam;
}

void OnDestroy(HWND /*hWnd*/)
{
	PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(hwnd, &ps);

	HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));

	// создаем желтую кисть
	LOGBRUSH brushInfo;
	brushInfo.lbStyle = BS_SOLID;
	brushInfo.lbColor = RGB(255, 255, 0);
	brushInfo.lbHatch = 0;
	HBRUSH brush = CreateBrushIndirect(&brushInfo);

	// выбираем перо и кисть в контексте устройства, запоминая ранее выбранные объекты
	HPEN oldPen = SelectPen(dc, pen);
	HBRUSH oldBrush = SelectBrush(dc, brush);
	 
	// Н
	Rectangle(dc,		
		100, 50,	
		150, 350	
	);
	Rectangle(dc,	
		150, 175,	
		250, 225	
	);
	Rectangle(dc,
		250, 50,
		300, 350
	);

	// M

	HPEN pen2 = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

	brushInfo.lbColor = RGB(0, 0, 255);
	HBRUSH brush2 = CreateBrushIndirect(&brushInfo);
	SelectBrush(dc, brush2);
	SelectPen(dc, pen2);

	Rectangle(dc,
		350, 50,
		400, 350
	);

	POINT polygonPoints[4][2] = {
		POINT{350, 50},
		POINT{400, 50},
		POINT{475, 200},
		POINT{450, 200}
	};

	Polygon(dc,
		*polygonPoints,
		4
	);

	POINT polygonPoints2[4][2] = {
		POINT{525, 50},
		POINT{575, 50},
		POINT{475, 200},
		POINT{450, 200}
	};

	Polygon(dc,
		*polygonPoints2,
		4
	);

	Rectangle(dc,
		525, 50,
		575, 350
	);

	//M
	 
	HPEN pen3 = CreatePen(PS_SOLID, 2, RGB(255, 0, 255));

	brushInfo.lbColor = RGB(255, 0, 255);
	HBRUSH brush3 = CreateBrushIndirect(&brushInfo);
	SelectBrush(dc, brush3);
	SelectPen(dc, pen3);

	Rectangle(dc,
		625, 50,
		675, 350
	);

	POINT polygonPoints3[4][2] = {
		POINT{625, 50},
		POINT{675, 50},
		POINT{750, 200},
		POINT{725, 200}
	};

	Polygon(dc,
		*polygonPoints3,
		4
	);

	POINT polygonPoints4[4][2] = {
		POINT{800, 50},
		POINT{850, 50},
		POINT{750, 200},
		POINT{725, 200}
	};

	Polygon(dc,
		*polygonPoints4,
		4
	);

	Rectangle(dc,
		800, 50,
		850, 350
	);

	// выбираем ранее сохраненные объекты GDI в контексте устройства
	// при этом pen и brush автоматически становятся в нем не выбраны
	SelectPen(dc, oldPen);
	SelectBrush(dc, oldBrush);

	// объекты GDI можно удалять если они не выбраны ни в одном из контекстов
	DeletePen(pen);
	DeleteBrush(brush);

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASSEX wndClass =
	{
		sizeof(wndClass),	//UINT cbSize;
		CS_HREDRAW | CS_VREDRAW, //UINT style;
		&WindowProc,		//WNDPROC lpfnWndProc;
		0,					//int cbClsExtra;
		0,					//int cbWndExtra;
		hInstance,			//HINSTANCE hInstance;
		NULL,				//HICON hIcon;
		LoadCursor(NULL, IDC_ARROW), //HCURSOR hCursor;
		(HBRUSH)(COLOR_BTNFACE + 1), //HBRUSH hbrBackground;
		NULL,				//LPCTSTR lpszMenuName;
		CLASS_NAME,			//LPCTSTR lpszClassName;
		NULL,				//HICON hIconSm;
	};

	return RegisterClassEx(&wndClass) != FALSE;
}