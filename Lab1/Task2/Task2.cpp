#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

TCHAR const CLASS_NAME[] = _T("MainWndClass");
TCHAR const WINDOW_TITLE[] = _T("My first window");

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int MainLoop();
bool RegisterWndClass(HINSTANCE hInstance);
HWND CreateMainWindow(HINSTANCE hInstance);

struct RGBColor {
	byte r;
	byte g;
	byte b;
};

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

void DrawSolidRectangle(HDC dc, RGBColor color, int x1, int y1, int x2, int y2)
{
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(color.r, color.g, color.b));

	LOGBRUSH brushInfo;
	brushInfo.lbStyle = BS_SOLID;
	brushInfo.lbColor = RGB(color.r, color.g, color.b);
	brushInfo.lbHatch = 0;
	HBRUSH brush = CreateBrushIndirect(&brushInfo);

	HPEN oldPen = SelectPen(dc, pen);
	HBRUSH oldBrush = SelectBrush(dc, brush);

	Rectangle(dc,
		x1, y1,
		x2, y2
	);

	SelectPen(dc, oldPen);
	SelectBrush(dc, oldBrush);

	DeletePen(pen);
	DeleteBrush(brush);
}

void DrawSolidTriangle(HDC dc, RGBColor color, POINT* vertexes)
{
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(color.r, color.g, color.b));

	LOGBRUSH brushInfo;
	brushInfo.lbStyle = BS_SOLID;
	brushInfo.lbColor = RGB(color.r, color.g, color.b);
	brushInfo.lbHatch = 0;
	HBRUSH brush = CreateBrushIndirect(&brushInfo);

	HPEN oldPen = SelectPen(dc, pen);
	HBRUSH oldBrush = SelectBrush(dc, brush);

	Polygon(dc,
		vertexes,
		3
	);

	SelectPen(dc, oldPen);
	SelectBrush(dc, oldBrush);

	DeletePen(pen);
	DeleteBrush(brush);
}

void DrawSolidEllipse(HDC dc, RGBColor color, int left, int top, int right, int bottom)
{
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(color.r, color.g, color.b));

	LOGBRUSH brushInfo;
	brushInfo.lbStyle = BS_SOLID;
	brushInfo.lbColor = RGB(color.r, color.g, color.b);
	brushInfo.lbHatch = 0;
	HBRUSH brush = CreateBrushIndirect(&brushInfo);

	HPEN oldPen = SelectPen(dc, pen);
	HBRUSH oldBrush = SelectBrush(dc, brush);

	Ellipse(dc, left, top, right, bottom);

	SelectPen(dc, oldPen);
	SelectBrush(dc, oldBrush);

	DeletePen(pen);
	DeleteBrush(brush);
}

void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(hwnd, &ps);

	DrawSolidRectangle(dc, RGBColor{ 135,206,235 }, 0, 0, 800, 250);
	DrawSolidRectangle(dc, RGBColor{ 0,120,0 }, 0, 250, 800, 500);
	DrawSolidRectangle(dc, RGBColor{ 139,69,19 }, 200, 200 , 400, 400);
	POINT roofVertexes[3][2] = {
		POINT{165, 200},
		POINT{300, 75},
		POINT{435, 200}
	};
	DrawSolidRectangle(dc, RGBColor{ 255, 0, 0 }, 210, 80, 230, 200);
	DrawSolidRectangle(dc, RGBColor{ 255, 255, 0 }, 230, 275, 270, 325);
	DrawSolidRectangle(dc, RGBColor{ 244, 164, 96 }, 375, 220, 300, 400);
	DrawSolidTriangle(dc, RGBColor{ 139,69,19 }, *roofVertexes);
	DrawSolidEllipse(dc, RGBColor{ 210,105,30 }, 365, 305, 350, 315);
	
	for (int i = 0; i < 6; i++)
		DrawSolidRectangle(dc, RGBColor{ 244,164,96 }, 410 + 40 * i, 260, 430 + 40 * i, 400);
	DrawSolidRectangle(dc, RGBColor{ 244,164,96 }, 400, 290, 630, 295);
	DrawSolidRectangle(dc, RGBColor{ 244,164,96 }, 400, 370, 630, 375);

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