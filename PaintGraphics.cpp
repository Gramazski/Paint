#include <windows.h>
#include <string.h>


#define ID_FILE_NEW 1001
#define ID_FILE_SAVE 1002
#define ID_FILE_OPEN 1003
#define ID_FILE_PRINT 1004

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

#define FILE_NAME L"Metafile1.emf"

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

HBITMAP CreateHBitmap(HDC, int, int);
HBITMAP CreateClientWndBitmap(HWND, HDC);

COLORREF SetNewColor(HWND);

HBITMAP OpenEnhFile(HWND, HDC);
void SaveEnhFile(HWND, HDC);

void ZoomPict(HWND, HDC, double, HBITMAP);

										  
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
		AppendMenu(filePopupMenu, MF_STRING, ID_FILE_NEW, L"Новый");
		AppendMenu(filePopupMenu, MF_STRING, ID_FILE_SAVE, L"Сохранить");
		AppendMenu(filePopupMenu, MF_STRING, ID_FILE_OPEN, L"Открыть");
		AppendMenu(filePopupMenu, MF_STRING | MF_CHECKED, ID_FILE_PRINT, L"Печать");
		/*AppendMenu(filePopupMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(filePopupMenu, MF_STRING, 1000, L"Выход");*/
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

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, 0, L"&?");

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
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); 
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	wc.hInstance = hInstance; 

	return RegisterClassEx(&wc);
}

BOOL CreateWndInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hMainWnd;

	hInst = hInstance; 

	hMainWnd = CreateWindow(L"Мой класс", L"Полноценная оконная процедура", WS_OVERLAPPEDWINDOW | WS_VSCROLL, 
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
	RECT r;
	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(128, 0, 0));
	COLORREF colorText = RGB(255, 0, 30); 
	HFONT hFont;
	static PTCHAR string;
	static int size;
	static int width;
	static bool buttonFlag;
	static int id;
	int static x0, y0, x1, y1, x2, y2, oldMixMode, pointsCount;
	static POINT startPoint;
	static POINT* points;
	POINT* newPoints;
	int wmId;
	static COLORREF color;
	static COLORREF brushColor = NULL;
	static double scale = 1;
	int zDelta;

	static HDC hdcEMF;
	HENHMETAFILE hemf;

	static HDC hCompatibleDC = 0;
	HDC hMemDC;
	static HDC hBitmapDC = 0;
	static HBITMAP hCompatibleBitmap, hBitmap, EnhBitmap;
	HANDLE hOld;

	switch (uMsg) {
	case WM_CREATE:
		ShowWindow(hWnd, SW_MAXIMIZE);
		string = (PTCHAR)GlobalAlloc(GPTR, 50000 * sizeof(TCHAR));
		hDC = GetDC(hWnd);
		width = 1;
		color = RGB(128, 0, 0);
		hdcEMF = CreateEnhMetaFile(NULL, FILE_NAME, NULL,L"CreateMetaFile\OPictl\0");
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
			break;
		case ID_FILE_OPEN:
			hDC = GetDC(hWnd);
			EnhBitmap = OpenEnhFile(hWnd, hDC);
			GetClientRect(hWnd, &rect);
			hMemDC = CreateCompatibleDC(hDC);
			SelectObject(hMemDC, EnhBitmap);
			BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);

			DeleteDC(hMemDC);
			break;
		case ID_FILE_PRINT:
		case ID_FILE_SAVE:
			SaveEnhFile(hWnd, hDC);
			break;
		case ID_BUTTON_ABOUT:
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
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		SetTextColor(hDC, NULL);
		hFont = CreateFont(90, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial Bold");
		SelectObject(hDC, hFont);
		DrawText(hDC, (LPCWSTR)string, size, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_HOME:case VK_END:case VK_PRIOR:
		case VK_NEXT:case VK_LEFT:case VK_RIGHT:
		case VK_UP:case VK_DOWN:case VK_DELETE:
		case VK_SHIFT:case VK_SPACE:case VK_CONTROL:
		case VK_CAPITAL:case VK_MENU:case VK_TAB:
			break;
		case VK_BACK:
			if (size>0)
				size--;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_RETURN:
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		default:
			string[size++] = (char)wParam;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}break;
	case WM_LBUTTONDOWN:
		GetClientRect(hWnd, &rect);
		hDC = GetDC(hWnd);
		hBitmap = CreateClientWndBitmap(hWnd, hDC);
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
			/*DrawWithEll(hdcEMF, hWnd, startPoint, LOWORD(lParam), HIWORD(lParam), width, color, hBitmap);
			BitBlt(hdcEMF, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);*/
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
		break;
	case WM_RBUTTONUP:
		switch (id)
		{
		case ID_BUTTON_POLYGON:
			DrawWithPolygone(hDC, hWnd, points, pointsCount, width, color, brushColor);
			break;
		default:
			break;
		}
		break;
	case WM_MOUSEWHEEL:
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
	RECT rect;

	SelectObject(hDC, hPen);
	MoveToEx(hDC, stPoints[0].x, stPoints[0].y, NULL);
	PolylineTo(hDC, stPoints, pointsCount);
}

void DrawWithPolygone(HDC hDC, HWND hWnd, POINT* stPoints, int pointsCount, int width, COLORREF color, COLORREF brushColor)
{
	HPEN hPen = CreatePen(PS_SOLID, width, color);
	HBRUSH hBrush = CreateSolidBrush(brushColor);
	RECT rect;

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
	HDC hdc1;
	HENHMETAFILE hEnhMtf;
	ENHMETAHEADER emh;
	RECT rect, rect1;
	HDC hBitmapDC = 0;
	HBITMAP hBitmap = 0;
	PAINTSTRUCT ps;

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
		hdc1 = BeginPaint(hWnd, &ps);
		hEnhMtf = GetEnhMetaFile(fullpath);
		GetEnhMetaFileHeader(hEnhMtf, sizeof(ENHMETAHEADER), &emh);
		SetRect(&rect, emh.rclBounds.left, emh.rclBounds.top, emh.rclBounds.right, emh.rclBounds.bottom);
		PlayEnhMetaFile(hdc1, hEnhMtf, &rect);

		hBitmap = CreateClientWndBitmap(hWnd, hdc1);

		/*InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);*/
		DeleteEnhMetaFile(hEnhMtf);

		EndPaint(hWnd, &ps);
	}

	return hBitmap;
}

void SaveEnhFile(HWND hWnd, HDC hDC)
{
	static OPENFILENAME ofn;
	static TCHAR fullpath[256], filename[256], dir[256];
	HDC hdcEMF;
	HENHMETAFILE hemf;
	ENHMETAHEADER emf;
	RECT rect, rect1;
	HDC hBitmapDC = 0;
	HBITMAP hBitmap = 0;
	PAINTSTRUCT ps;

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
	ofn.lpstrTitle = L"Save file...";
	ofn.Flags = OFN_EXPLORER | OFN_CREATEPROMPT | OFN_ALLOWMULTISELECT;
	if (GetSaveFileName(&ofn))
	{
		hdcEMF = CreateEnhMetaFile(NULL, fullpath, NULL, L"CreateMetaFile\OPictl\0");

		hBitmap = CreateClientWndBitmap(hWnd, hDC);
		GetClientRect(hWnd, &rect);
		//hMemDC = CreateCompatibleDC(hDC);
		SelectObject(hDC, hBitmap);
		BitBlt(hdcEMF, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);


		hemf = CloseEnhMetaFile(hdcEMF);
		DeleteEnhMetaFile(hemf);
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
	//BitBlt(hDC, 0, 0, rect.right, rect.bottom, hMemDC, 0, 0, SRCCOPY);

	DeleteDC(hMemDC);
}
