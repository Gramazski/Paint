#include <windows.h>
#include <string.h>
#include "resource.h"

#define ID_FILE_NEW 1001
#define ID_FILE_SAVE 1002
#define ID_FILE_OPEN 1003
#define ID_FILE_PRINT 1004
#define ID_FILE_EXIT 1005

#define ID_BUTTON_LINE 2001
#define ID_BUTTON_PEN 2002
#define ID_BUTTON_POLYLINE 2003
#define ID_BUTTTON_RECTANGLE 2004
#define ID_BUTTON_ELLIPSE 2005
#define ID_BUTTON_POLYGON 2006
#define ID_BUTTON_TEXT 2007

#define ID_BUTTON_ZOOM 3000

#define ID_BUTTON_WIDTH1 4001
#define ID_BUTTON_WIDTH2 4002
#define ID_BUTTON_WIDTH3 4003
#define ID_BUTTON_WIDTH4 4004
#define ID_BUTTON_WIDTH5 4005
#define ID_BUTTON_WIDTH6 4006

#define ID_BUTTON_ABOUT 5000

#define ID_BUTTON_COLOR 6001
#define ID_BUTTON_COLOR_NONE 6002
#define ID_BUTTON_COLOR_FILL 6003

#define HK_SAVE 8000
#define HK_OPEN 8001
#define HK_PRINT 8002
#define HK_NEW 8003

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void CreateProgramMenu(HWND);
ATOM RegisterWndClass(HINSTANCE);
BOOL CreateWndInstance(HINSTANCE, int);

POINT DrawWithPen(HDC, HWND, POINT, int, int, int, COLORREF);
void DrawWithLine(HDC, HWND, POINT, int, int, int, COLORREF, HBITMAP);
void DrawWithRect(HDC, HWND, POINT, int, int, int, COLORREF, HBITMAP, COLORREF);
void DrawWithEll(HDC, HWND, POINT, int, int, int, COLORREF, HBITMAP, COLORREF);
void DrawWithPolyline(HDC, HWND, POINT*, int, int, COLORREF);
void DrawWithPolygone(HDC, HWND, POINT*, int, int, COLORREF, COLORREF);
void DrawWithText(HDC, HWND, COLORREF, PTCHAR, int, int, int, int);

HBITMAP CreateHBitmap(HDC, int, int);
HBITMAP CreateClientWndBitmap(HWND, HDC);

COLORREF SetNewColor(HWND);

HBITMAP OpenEnhFile(HWND, HDC);
void SaveEnhFile(HWND, HDC, HBITMAP);
void PrintFile(HWND, HDC, HBITMAP, double);

void ZoomPict(HWND, HDC, double, HBITMAP);
POINT MovePict(HWND, HDC, double, POINT, POINT, int, int, HBITMAP);

INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
void CreateHotKeys(HWND);

										  
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) 
{
	HWND hMainWnd; 
	MSG msg; 

	if (!RegisterWndClass(hInst)) {
		MessageBox(NULL, L"Не получилось зарегистрировать класс!", L"Ошибка", MB_OK);
		return NULL; 
	}

	if (!CreateWndInstance(hInst, nCmdShow))
	{
		return FALSE;
	}

	while (GetMessage(&msg, NULL, NULL, NULL)) { 
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	}
	return msg.wParam; 
}

void CreateProgramMenu(HWND hwnd)
{
	HMENU mainMenu = CreateMenu();
	HMENU filePopupMenu = CreatePopupMenu();
	HMENU figuresPopupMenu = CreatePopupMenu();
	HMENU widthPopupMenu = CreatePopupMenu();
	HMENU colorPopupMenu = CreatePopupMenu();

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)filePopupMenu, L"&Файл");
	{
		AppendMenu(filePopupMenu, MF_STRING, ID_FILE_NEW, L"Новый(CTRL+N)");
		AppendMenu(filePopupMenu, MF_STRING, ID_FILE_SAVE, L"Сохранить(CTRL+S)");
		AppendMenu(filePopupMenu, MF_STRING, ID_FILE_OPEN, L"Открыть(CTRL+O)");
		AppendMenu(filePopupMenu, MF_STRING | MF_CHECKED, ID_FILE_PRINT, L"Печать(CTRL+P)");
		AppendMenu(filePopupMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(filePopupMenu, MF_STRING, ID_FILE_EXIT, L"Выход");
	}

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)figuresPopupMenu, L"&Фигуры");
	{
		AppendMenu(figuresPopupMenu, MF_STRING, ID_BUTTON_LINE, L"Линия");
		AppendMenu(figuresPopupMenu, MF_STRING, ID_BUTTON_PEN, L"Карандаш");
		AppendMenu(figuresPopupMenu, MF_STRING, ID_BUTTON_POLYLINE, L"Ломаная");
		AppendMenu(figuresPopupMenu, MF_STRING, ID_BUTTTON_RECTANGLE, L"Прямоугольник");
		AppendMenu(figuresPopupMenu, MF_STRING, ID_BUTTON_ELLIPSE, L"Эллипс");
		AppendMenu(figuresPopupMenu, MF_STRING, ID_BUTTON_POLYGON, L"Многоугольник");
		AppendMenu(figuresPopupMenu, MF_STRING | MF_CHECKED, ID_BUTTON_TEXT, L"Текст");
	}

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, ID_BUTTON_ZOOM, L"&Zoom");

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)widthPopupMenu, L"&Толщина");
	{
		AppendMenu(widthPopupMenu, MF_STRING, ID_BUTTON_WIDTH1, L"1");
		AppendMenu(widthPopupMenu, MF_STRING, ID_BUTTON_WIDTH2, L"2");
		AppendMenu(widthPopupMenu, MF_STRING, ID_BUTTON_WIDTH3, L"3");
		AppendMenu(widthPopupMenu, MF_STRING, ID_BUTTON_WIDTH4, L"4");
		AppendMenu(widthPopupMenu, MF_STRING, ID_BUTTON_WIDTH5, L"5");
		AppendMenu(widthPopupMenu, MF_STRING, ID_BUTTON_WIDTH6, L"6");
	}

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, ID_BUTTON_ABOUT, L"&?");

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)colorPopupMenu, L"&Палитра");
	{
		AppendMenu(colorPopupMenu, MF_STRING, ID_BUTTON_COLOR, L"Цвета");
		AppendMenu(colorPopupMenu, MF_STRING, ID_BUTTON_COLOR_FILL, L"Заливка");
		AppendMenu(colorPopupMenu, MF_STRING, ID_BUTTON_COLOR_NONE, L"Убрать заливку");
	}

	SetMenu(hwnd, mainMenu);
}

ATOM RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc; 

	wc.cbSize = sizeof(wc); 
	wc.style = CS_HREDRAW | CS_VREDRAW; 
	wc.lpfnWndProc = WndProc; 
	wc.lpszMenuName = NULL; 
	wc.lpszClassName = L"Мой класс"; 
	wc.cbWndExtra = NULL; 
	wc.cbClsExtra = NULL; 
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	wc.hInstance = hInstance; 

	return RegisterClassEx(&wc);
}

BOOL CreateWndInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hMainWnd;

	hInst = hInstance; 

	hMainWnd = CreateWindow(L"Мой класс", L"My Paint", WS_OVERLAPPEDWINDOW | WS_VSCROLL, 
		CW_USEDEFAULT, NULL, CW_USEDEFAULT, NULL, (HWND)NULL, NULL, HINSTANCE(hInstance), NULL); 

	if (!hMainWnd) {
		MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
		return FALSE;
	}

	CreateProgramMenu(hMainWnd);

	ShowWindow(hMainWnd, nCmdShow); 
	UpdateWindow(hMainWnd); 

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HDC hDC; 
	PAINTSTRUCT ps; 
	RECT rect; 
	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(128, 0, 0));
	COLORREF colorText = RGB(255, 0, 30); 
	static PTCHAR string;
	static int size;
	static int width;
	static bool buttonFlag;
	static int id;
	int static  pointsCount;
	static POINT startPoint, movePoint;
	static POINT* points;
	POINT* newPoints;
	int wmId;
	static COLORREF color;
	static COLORREF brushColor = NULL;
	static double scale = 1;
	int zDelta;

	HDC hMemDC;
	static HBITMAP hBitmap, EnhBitmap;

	switch (uMsg) {
	case WM_CREATE:
		ShowWindow(hWnd, SW_MAXIMIZE);
		string = (PTCHAR)GlobalAlloc(GPTR, 50000 * sizeof(TCHAR));
		hDC = GetDC(hWnd);
		width = 1;
		color = RGB(128, 0, 0);
		CreateHotKeys(hWnd);
		break;
	case WM_ERASEBKGND:
		GetClientRect(hWnd, &rect);
		FillRect(hDC, &rect, WHITE_BRUSH);
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		id = wmId;
		switch (wmId)
		{
		case ID_FILE_NEW:
			hDC = GetDC(hWnd);
			GetClientRect(hWnd, &rect);
			FillRect(hDC, &rect, WHITE_BRUSH);
			width = 1;
			color = RGB(128, 0, 0);
			hBitmap = 0;
			ReleaseDC(hWnd, hDC);
			break;
		case ID_FILE_OPEN:
			hBitmap = OpenEnhFile(hWnd, hDC);
			break;
		case ID_FILE_PRINT:
			hDC = GetDC(hWnd);
			PrintFile(hWnd, hDC, hBitmap, scale);
			ReleaseDC(hWnd, hDC);
			break;
		case ID_FILE_SAVE:
			SaveEnhFile(hWnd, hDC, hBitmap);
			break;
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOGABOUT), hWnd, About);
			break;
		case ID_BUTTON_ELLIPSE:
		case ID_BUTTON_LINE:
		case ID_BUTTON_PEN:
		case ID_BUTTTON_RECTANGLE:
			break;
		case ID_BUTTON_POLYGON:
		case ID_BUTTON_POLYLINE:
			pointsCount = 1;
			break;
		case ID_BUTTON_TEXT:
			break;
		case ID_BUTTON_WIDTH1:
			width = 1;
			break;
		case ID_BUTTON_WIDTH2:
			width = 2;
			break;
		case ID_BUTTON_WIDTH3:
			width = 3;
			break;
		case ID_BUTTON_WIDTH4:
			width = 4;
			break;
		case ID_BUTTON_WIDTH5:
			width = 5;
			break;
		case ID_BUTTON_WIDTH6:
			width = 6;
			break;
		case ID_BUTTON_COLOR:
			color = SetNewColor(hWnd);
			break;
		case ID_BUTTON_ZOOM:
			scale = 1;
			break;
		case ID_BUTTON_COLOR_FILL:
			brushColor = SetNewColor(hWnd);
			break;
		case ID_BUTTON_COLOR_NONE:
			brushColor = NULL;
		default:
			break;
		}
		break;
	case WM_HOTKEY:
		switch (LOWORD(wParam))
		{
		case HK_OPEN:
			hBitmap = OpenEnhFile(hWnd, hDC);
			break;
		case HK_SAVE:
			SaveEnhFile(hWnd, hDC, hBitmap);
			break;
		case HK_PRINT:
			hDC = GetDC(hWnd);
			PrintFile(hWnd, hDC, hBitmap, scale);
			ReleaseDC(hWnd, hDC);
			break;
		case HK_NEW:
			hDC = GetDC(hWnd);
			GetClientRect(hWnd, &rect);
			FillRect(hDC, &rect, WHITE_BRUSH);
			width = 1;
			color = RGB(128, 0, 0);
			hBitmap = 0;
			ReleaseDC(hWnd, hDC);
			break;
		default:
			break;
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		
		GetClientRect(hWnd, &rect);
		hMemDC = CreateCompatibleDC(hDC);
		SelectObject(hMemDC, hBitmap);
		BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);

		DeleteDC(hMemDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		hDC = GetDC(hWnd);
		if (id = ID_BUTTON_TEXT)
		{
			switch (wParam)
			{
			case VK_HOME:case VK_END:case VK_PRIOR:
			case VK_NEXT:case VK_LEFT:case VK_RIGHT:
			case VK_UP:case VK_DOWN:case VK_DELETE:
			case VK_SHIFT:case VK_SPACE:case VK_CONTROL:
			case VK_CAPITAL:case VK_MENU:case VK_TAB:
				break;
			case VK_RETURN:
				break;
			case VK_BACK:
				if (size>0)
					size--;
				DrawWithText(hDC, hWnd, color, string, size, startPoint.x, startPoint.y, width);
				break;
			default:
				string[size++] = (char)wParam;
				DrawWithText(hDC, hWnd, color, string, size, startPoint.x, startPoint.y, width);
				break;
			}
		}
		hBitmap = CreateClientWndBitmap(hWnd, hDC);
		ReleaseDC(hWnd, hDC);
		break;
	case WM_LBUTTONDOWN:
		GetClientRect(hWnd, &rect);
		hDC = GetDC(hWnd);
		if (id != ID_BUTTON_ZOOM)
		{
			hBitmap = CreateClientWndBitmap(hWnd, hDC);
		}
		switch (id)
		{
		case ID_BUTTON_LINE:
		case ID_BUTTON_PEN:
		case ID_BUTTTON_RECTANGLE:
		case ID_BUTTON_ELLIPSE:
			startPoint.x = (short)LOWORD(lParam);
			startPoint.y = (short)HIWORD(lParam);
			break;
		case ID_BUTTON_POLYGON:
		case ID_BUTTON_POLYLINE:
			if (pointsCount == 1)
			{
				points = (POINT*)malloc(sizeof(POINT));
				points[0].x = (short)LOWORD(lParam);
				points[0].y = (short)HIWORD(lParam);
			}
			break;
		case ID_BUTTON_TEXT:
			startPoint.x = (short)LOWORD(lParam);
			startPoint.y = (short)HIWORD(lParam);
			size = 0;
			break;
		case ID_BUTTON_ZOOM:
			startPoint.x = (short)((short)LOWORD(lParam) / scale);
			startPoint.y = (short)((short)HIWORD(lParam) / scale);
			movePoint.x = 0;
			movePoint.y = 0;
			break;
		default:
			break;
		}
		buttonFlag = true;
		break;
	case WM_MOUSEMOVE:
		GetClientRect(hWnd, &rect);
		if (buttonFlag)
		{
			switch (id)
			{
			case ID_BUTTON_ELLIPSE:
				DrawWithEll(hDC, hWnd, startPoint, LOWORD(lParam), HIWORD(lParam), width, color, hBitmap, brushColor);
				break;
			case ID_BUTTON_LINE:
				DrawWithLine(hDC, hWnd, startPoint, LOWORD(lParam), HIWORD(lParam), width, color, hBitmap);
				break;
			case ID_BUTTON_PEN:
				startPoint = DrawWithPen(hDC, hWnd, startPoint, LOWORD(lParam), HIWORD(lParam), width, color);
				break;
			case ID_BUTTTON_RECTANGLE:
				DrawWithRect(hDC, hWnd, startPoint, LOWORD(lParam), HIWORD(lParam), width, color, hBitmap, brushColor);
				break;
			case ID_BUTTON_POLYGON:
			case ID_BUTTON_POLYLINE:
				DrawWithLine(hDC, hWnd, points[pointsCount - 1], LOWORD(lParam), HIWORD(lParam), width, color, hBitmap);
				DrawWithPolyline(hDC, hWnd, points, pointsCount, width, color);
				break;
			case ID_BUTTON_ZOOM:
				movePoint = MovePict(hWnd, hDC, scale, startPoint, movePoint, LOWORD(lParam), HIWORD(lParam), hBitmap);
				startPoint.x = (short)(LOWORD(lParam) / scale);
				startPoint.y = (short)(HIWORD(lParam) / scale);
				break;
			default:
				break;
			}
		}
		break;
	case WM_LBUTTONUP:
		buttonFlag = false;
		GetClientRect(hWnd, &rect);
		switch (id)
		{
		case ID_BUTTON_ELLIPSE:
			break;
		case ID_BUTTON_POLYGON:
		case ID_BUTTON_POLYLINE:
			pointsCount++;
			newPoints = (POINT*)malloc(pointsCount * sizeof(POINT));
			memcpy(newPoints, points, (pointsCount - 1) * sizeof(POINT));
			newPoints[pointsCount - 1].x = LOWORD(lParam);
			newPoints[pointsCount - 1].y = HIWORD(lParam);
			points = (POINT*)malloc(pointsCount * sizeof(POINT));
			memcpy(points, newPoints, pointsCount * sizeof(POINT));
			free(newPoints);
			DrawWithPolyline(hDC, hWnd, points, pointsCount, width, color);
			break;
		case ID_BUTTON_TEXT:
			startPoint.x = LOWORD(lParam);
			startPoint.y = HIWORD(lParam);
			break;
		default:
			break;
		}
		if (id != ID_BUTTON_ZOOM)
		{
			hBitmap = CreateClientWndBitmap(hWnd, hDC);
		}
		ReleaseDC(hWnd, hDC);
		break;
	case WM_RBUTTONUP:
		hDC = GetDC(hWnd);
		switch (id)
		{
		case ID_BUTTON_POLYGON:
			DrawWithPolygone(hDC, hWnd, points, pointsCount, width, color, brushColor);
			break;
		default:
			break;
		}
		ReleaseDC(hWnd, hDC);
		break;
	case WM_MOUSEWHEEL:
		hDC = GetDC(hWnd);
		switch (id)
		{
		case ID_BUTTON_ZOOM:
			zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			if ((zDelta>0) && (scale<3))
				scale = scale + 0.03;
			if ((zDelta<0) && (scale>0.3))
				scale = scale - 0.03;
			ZoomPict(hWnd, hDC, scale, hBitmap);
			break;
		default:
			break;
		}
		ReleaseDC(hWnd, hDC);
		break;
	case WM_DESTROY: 
		ReleaseDC(hWnd, hDC);
		PostQuitMessage(NULL); 
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam); 
	}
	return NULL; 
}

POINT DrawWithPen(HDC hDC, HWND hWnd, POINT stPoint, int x2, int y2, int width, COLORREF color)
{
	HPEN hPen = CreatePen(PS_SOLID, width, color);
	SelectObject(hDC, hPen);
	MoveToEx(hDC, stPoint.x, stPoint.y, NULL);
	LineTo(hDC, x2, y2);

	POINT newPoint;
	newPoint.x = x2;
	newPoint.y = y2;

	return newPoint;
}

void DrawWithLine(HDC hDC, HWND hWnd, POINT stPoint, int x2, int y2, int width, COLORREF color, HBITMAP hBitmap)
{
	HPEN hPen = CreatePen(PS_SOLID, width, color);
	RECT rect;
	HDC hMemDC;

	GetClientRect(hWnd, &rect);
	hMemDC = CreateCompatibleDC(hDC);
	SelectObject(hMemDC, hBitmap);
	BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);

	DeleteDC(hMemDC);

	SelectObject(hDC, hPen);
	MoveToEx(hDC, stPoint.x, stPoint.y, NULL);
	LineTo(hDC, x2, y2);
}

void DrawWithRect(HDC hDC, HWND hWnd, POINT stPoint, int x2, int y2, int width, COLORREF color, HBITMAP hBitmap, COLORREF brushColor)
{
	HPEN hPen = CreatePen(PS_SOLID, width, color);
	HBRUSH hBrush = CreateSolidBrush(brushColor);
	RECT rect, brushRect;
	HDC hMemDC;

	GetClientRect(hWnd, &rect);
	hMemDC = CreateCompatibleDC(hDC);
	SelectObject(hMemDC, hBitmap);
	BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);

	DeleteDC(hMemDC);

	SelectObject(hDC, hPen);
	MoveToEx(hDC, stPoint.x, stPoint.y, NULL);
	if (brushColor)
	{
		SetRect(&brushRect, stPoint.x, stPoint.y, x2, y2);
		FillRect(hDC, &brushRect, hBrush);
	}
	else
	{
		Rectangle(hDC, stPoint.x, stPoint.y, x2, y2);
	}
}

void DrawWithEll(HDC hDC, HWND hWnd, POINT stPoint, int x2, int y2, int width, COLORREF color, HBITMAP hBitmap, COLORREF brushColor)
{
	HPEN hPen = CreatePen(PS_SOLID, width, color);
	HBRUSH hBrush = CreateSolidBrush(brushColor);
	RECT rect;
	HDC hMemDC;

	GetClientRect(hWnd, &rect);
	hMemDC = CreateCompatibleDC(hDC);
	SelectObject(hMemDC, hBitmap);
	BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);

	DeleteDC(hMemDC);
	
	MoveToEx(hDC, stPoint.x, stPoint.y, NULL);
	if (brushColor)
	{
		SelectObject(hDC, hBrush);
	}
	else
	{
		SelectObject(hDC, hPen);
	}
	
	Ellipse(hDC, stPoint.x, stPoint.y, x2, y2);
}

void DrawWithPolyline(HDC hDC, HWND hWnd, POINT* stPoints, int pointsCount, int width, COLORREF color)
{
	HPEN hPen = CreatePen(PS_SOLID, width, color);

	SelectObject(hDC, hPen);
	MoveToEx(hDC, stPoints[0].x, stPoints[0].y, NULL);
	PolylineTo(hDC, stPoints, pointsCount);
}

void DrawWithPolygone(HDC hDC, HWND hWnd, POINT* stPoints, int pointsCount, int width, COLORREF color, COLORREF brushColor)
{
	HPEN hPen = CreatePen(PS_SOLID, width, color);
	HBRUSH hBrush = CreateSolidBrush(brushColor);

	SelectObject(hDC, hPen);
	MoveToEx(hDC, stPoints[0].x, stPoints[0].y, NULL);

	if (brushColor)
	{
		SelectObject(hDC, hBrush);
	}
	else
	{
		SelectObject(hDC, hPen);
	}

	Polygon(hDC, stPoints, pointsCount);
}

void DrawWithText(HDC hDC, HWND hWnd, COLORREF color, PTCHAR string, int size, int x, int y, int width)
{
	HFONT hFont;
	RECT rect;

	SetRect(&rect, x - 1, y - 1, x + 7 * width * (size + 1), y + 10 * width);
	FillRect(hDC, &rect, WHITE_BRUSH);

	SetTextColor(hDC, color);
	hFont = CreateFont((10 * width), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial Bold");
	SelectObject(hDC, hFont);
	SetRect(&rect, x , y, x + 7 * width * size, y + 10 * width);
	Rectangle(hDC, x - 1, y - 1, x + 7 * width * size, y + 10 * width);
	DrawText(hDC, (LPCWSTR)string, size, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

HBITMAP CreateHBitmap(HDC hDC, int w, int h)
{
	HDC hDCmem;
	HBITMAP hbm, holdBM;

	hDCmem = CreateCompatibleDC(hDC);
	hbm = CreateCompatibleBitmap(hDC, w, h);
	holdBM = (HBITMAP)SelectObject(hDCmem, hbm);
	BitBlt(hDCmem, 0, 0, w, h, hDC, 0, 0, SRCCOPY);
	SelectObject(hDCmem, holdBM);
	DeleteDC(hDCmem);
	DeleteObject(holdBM);

	return hbm;
}

HBITMAP CreateClientWndBitmap(HWND hWND, HDC hDC)
{
	RECT r;

	GetClientRect(hWND, &r);

	return(CreateHBitmap(hDC, r.right, r.bottom));
}

COLORREF SetNewColor(HWND hWnd)
{
	static CHOOSECOLOR curColor;
	static COLORREF  crCustColor[16];
	curColor.lStructSize = sizeof(CHOOSECOLOR);
	curColor.hInstance = NULL;
	curColor.hwndOwner = hWnd;
	curColor.lpCustColors = crCustColor;
	curColor.Flags = CC_RGBINIT | CC_FULLOPEN;
	curColor.lCustData = 0L;
	curColor.lpfnHook = NULL;
	curColor.rgbResult = RGB(0x80, 0x80, 0x80);
	curColor.lpTemplateName = NULL;

	if (ChooseColor(&curColor))
	{
		return curColor.rgbResult;
	}

	return curColor.rgbResult;
}

HBITMAP OpenEnhFile(HWND hWnd, HDC hDC)
{
	static OPENFILENAME ofn;
	static TCHAR fullpath[256], filename[256], dir[256];
	HENHMETAFILE hEnhMtf;
	ENHMETAHEADER emh;
	RECT rect;
	HBITMAP hBitmap = 0;

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = hInst;
	ofn.lpstrFilter = L"Metafile (*.emf)\0*.emf\0Все файлы (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = fullpath;
	ofn.nMaxFile = sizeof(fullpath);
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = sizeof(filename);
	ofn.lpstrInitialDir = dir;
	ofn.lpstrTitle = L"Open file...";
	ofn.Flags = OFN_EXPLORER | OFN_CREATEPROMPT | OFN_ALLOWMULTISELECT;
	if (GetOpenFileName(&ofn))
	{
		hDC = GetDC(hWnd);
		hEnhMtf = GetEnhMetaFile(fullpath);
		GetEnhMetaFileHeader(hEnhMtf, sizeof(ENHMETAHEADER), &emh);
		SetRect(&rect, emh.rclBounds.left, emh.rclBounds.top, emh.rclBounds.right, emh.rclBounds.bottom);
		PlayEnhMetaFile(hDC, hEnhMtf, &rect);

		hBitmap = CreateClientWndBitmap(hWnd, hDC);
		DeleteEnhMetaFile(hEnhMtf);

		ReleaseDC(hWnd, hDC);
	}

	return hBitmap;
}

void SaveEnhFile(HWND hWnd, HDC hDC, HBITMAP hBitmap)
{
	static OPENFILENAME ofn;
	static TCHAR fullpath[256], filename[256], dir[256];
	HDC hdcEMF;
	HENHMETAFILE hemf;
	RECT rect;

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.hInstance = hInst; 
	ofn.lpstrFilter = L"Metafile (*.emf)\0*.emf\0Все файлы (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = fullpath;
	ofn.nMaxFile = sizeof(fullpath);
	ofn.lpstrFileTitle = filename;
	ofn.nMaxFileTitle = sizeof(filename);
	ofn.lpstrInitialDir = dir;
	ofn.lpstrTitle = L"Сохранить...";
	ofn.Flags = OFN_EXPLORER | OFN_CREATEPROMPT | OFN_ALLOWMULTISELECT;
	if (GetSaveFileName(&ofn))
	{
		hdcEMF = CreateEnhMetaFile(NULL, fullpath, NULL, L"CreateMetaFile\OPictl\0");
		hDC = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		SelectObject(hDC, hBitmap);
		BitBlt(hdcEMF, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);

		hemf = CloseEnhMetaFile(hdcEMF);
		DeleteEnhMetaFile(hemf);
		ReleaseDC(hWnd, hDC);
	}
}

void PrintFile(HWND hWnd, HDC hDC, HBITMAP hBitmap, double scale)
{
	PRINTDLG printDlg;
	DOCINFO docInf;
	RECT rect;

	//ZeroMemory(&printDlg, sizeof(printDlg));
	printDlg.lStructSize = sizeof(printDlg);
	printDlg.hwndOwner = hWnd;
	printDlg.hDevMode = NULL; 
	printDlg.hDevNames = NULL; 
	printDlg.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
	printDlg.nCopies = 1;
	printDlg.nFromPage = 0xFFFF;
	printDlg.nToPage = 0xFFFF;
	printDlg.nMinPage = 1;
	printDlg.nMaxPage = 0xFFFF;

	if (PrintDlg(&printDlg) == TRUE)
	{
		int Rx = GetDeviceCaps(printDlg.hDC, LOGPIXELSX);
		int Ry = GetDeviceCaps(printDlg.hDC, LOGPIXELSY);
		docInf.cbSize = sizeof(DOCINFO);
		docInf.lpszDocName = L"Print Picture";
		docInf.fwType = NULL;
		docInf.lpszDatatype = NULL;
		docInf.lpszOutput = NULL;
		
		StartDoc(printDlg.hDC, &docInf);
		StartPage(printDlg.hDC);
		GetClientRect(hWnd, &rect);

		SelectObject(hDC, hBitmap);
		int Rx1 = GetDeviceCaps(hDC, LOGPIXELSX);
		int Ry1 = GetDeviceCaps(hDC, LOGPIXELSY);

		StretchBlt(printDlg.hDC, 0, 0, (int)((float)(rect.right*((float)(Rx / Rx1)))), (int)((float)(rect.bottom*((float)(Ry / Ry1)))),
			hDC, 0, 0, rect.right, rect.bottom, SRCCOPY);

		EndPage(printDlg.hDC);
		EndDoc(printDlg.hDC);
		DeleteDC(printDlg.hDC);
	}
}

void ZoomPict(HWND hWnd, HDC hDC, double scale, HBITMAP hBitmap)
{
	RECT rect;
	HDC hMemDC;

	GetClientRect(hWnd, &rect);
	hMemDC = CreateCompatibleDC(hDC);
	SelectObject(hMemDC, hBitmap);
	FillRect(hDC, &rect, WHITE_BRUSH);
	StretchBlt(hDC, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale), hMemDC, 0, 0, rect.right, rect.bottom, SRCCOPY);

	DeleteDC(hMemDC);
}

POINT MovePict(HWND hWnd, HDC hDC, double scale, POINT stPoint, POINT movePoint, int x, int y, HBITMAP hBitmap)
{
	RECT rect;
	HDC hMemDC;

	x = (short)(x / scale);
	y = (short)(y / scale);
	movePoint.x += (stPoint.x - x);
	movePoint.y += (stPoint.y - y);

	GetClientRect(hWnd, &rect);
	hMemDC = CreateCompatibleDC(hDC);
	SelectObject(hMemDC, hBitmap);
	FillRect(hDC, &rect, WHITE_BRUSH);
	StretchBlt(hDC, 0, 0, (int)(rect.right*scale), (int)(rect.bottom*scale), hMemDC, movePoint.x, movePoint.y, rect.right, rect.bottom, SRCCOPY);

	DeleteDC(hMemDC);

	return movePoint;
}

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

void CreateHotKeys(HWND hWnd)
{
	RegisterHotKey(hWnd, HK_SAVE, MOD_CONTROL, 'S');
	RegisterHotKey(hWnd, HK_OPEN, MOD_CONTROL, 'O');
	RegisterHotKey(hWnd, HK_NEW, MOD_CONTROL, 'N');
	RegisterHotKey(hWnd, HK_PRINT, MOD_CONTROL, 'P');
}

