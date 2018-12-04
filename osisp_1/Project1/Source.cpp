#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
int drawObj();
int refreshScreen();
int timerTick(HWND hWnd);
void mouseWheel(WPARAM wParam);
int close(HWND hWnd);
void correctXY();
int correct(int value, int minValue, int maxValue, int len, int* speed, int boost);
void resize(HWND hWnd);

HICON hi = LoadIconA(NULL, MAKEINTRESOURCE(IDI_ERROR));
byte curColorWindow = COLOR_WINDOW;
int mouseDownFlag = 0;




int X, Y = 0;
int step = 5;
int width = 900; int height = 600;
int window_width, window_height;
int rect_width = 48; int rect_height = 48;
int X_speed = 0, Y_speed = 0;
int X_boost = 50, Y_boost = 50;
RECT win_rect{ 0, 0, window_width, window_height };
HDC hdc;
HINSTANCE HInstance;

////////////////////////////////////////////////////////////////////////// 
// ������� ��������� ��������� WinMain
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE         hPrevInstance,
	LPSTR             lpCmdLine,
	int               nCmdShow)
{
	//HICON hIcon = (HICON)LoadImageA(hInstance, "bitmap3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT);

	//DrawIconEx(hdc, 300, 300, hIcon, 0, 0, 0, NULL, DI_NORMAL);
	//DestroyIcon(hIcon);
	
	//LPSTR A = MAKEINTRESOURCE(134);

	HInstance = hInstance;
	// ��� �������� ������
	LPCTSTR lpzClass = TEXT("lab-1");

	// ����������� ������
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1;

	// ���������� ��������� ������ ������
	RECT screen_rect, window_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect); // ���������� ������
	//AdjustWindowRect(
	int x = screen_rect.right / 2 - width/2;
	int y = screen_rect.bottom / 2 - height/2;

	// �������� ����
	HWND hWnd = CreateWindow(lpzClass, TEXT("lab-1"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER, x, y, width, height, NULL, NULL,
		hInstance, NULL);

	GetClientRect(hWnd, &window_rect); // ���������� ������
	window_width = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;

	// ���� ���� �� �������, ��������� ����� ����� 0
	if (!hWnd) return 2;

	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);
	//MoveToEx(hdc, X, Y, NULL);
	drawObj();
	//LineTo(hdc, 100, 100);
	//CBrush brush;
	
	//ReleaseDC(hWnd, hdc);

	/*
	HCURSOR hCursor = LoadCursorA(NULL, "IDC_CURSOR1");
	////Get device context
	HDC hDeviceContext = GetDC(hWnd);
	HDC hDCMem = CreateCompatibleDC(hDeviceContext);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDCMem, 50, 50);
	HGDIOBJ hbmOld = SelectObject(hDCMem, hBitmap);
	DrawIcon(hDCMem, 500, 500, hCursor);
	SelectObject(hDCMem, hbmOld);
	*/

	//static BITMAP bmpCirc;
	//PAINTSTRUCT pnst;
	//x = 600, y = 300;
	//static RECT rec;
	///static BLENDFUNCTION bfn;

	//HDC hDC = GetDC(hWnd);
	//GetClientRect(hWnd, &rec);

	//HDC hMemDC = CreateCompatibleDC(hDC);
	//HINSTANCE hInst = (HINSTANCE)GetClassLong(hWnd, GCL_HMODULE);

	//HBITMAP hBmpCirc = LoadBitmap(hInst, "bitmap3.bpm");

	//if (hBmpCirc == NULL)
	//{
		//MessageBox(NULL, "Error", "", MB_OK
	//}
	//hDC = BeginPaint(hWnd, &pnst);

	//hMemDC = CreateCompatibleDC(hDC);
	//SelectObject(hMemDC, hBmpCirc);
	//BitBlt(hDC, x, y, bmpCirc.bmWidth, bmpCirc.bmHeight, hMemDC, 0, 0, SRCCOPY);

	//if (!AlphaBlend(hDC, x, y, bmpCirc.bmWidth, bmpCirc.bmHeight, hMemDC, 0, 0, bmpCirc.bmWidth, bmpCirc.bmHeight, bfn))
//MessageBox(NULL, "Error", "", MB_OK);
	//DeleteDC(hMemDC);

//	EndPaint(hWnd, &pnst);

	//GetObject(hBmpCirc, sizeof(bmpCirc), (LPSTR)&bmpCirc);

	//ReleaseDC(hWnd, hDC);

	SetTimer(hWnd, 1, 80, NULL);
	MSG msg = { 0 };    // ��������� ���������
	int iGetOk = 0;   // ���������� ���������
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0) // ���� ���������
	{
		if (iGetOk == -1) return 3;  // ���� GetMessage ������ ������ - �����
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}



	return msg.wParam;  // ���������� ��� ���������� ���������
}

////////////////////////////////////////////////////////////////////////// 
// ������� ����������� ������ ����
ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
	WNDCLASS wcWindowClass = { 0 };
	// ����� �-��� ��������� ���������
	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	// ����� ����
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	// ���������� ���������� ����������
	wcWindowClass.hInstance = hInst;
	// �������� ������
	wcWindowClass.lpszClassName = lpzClassName;
	// �������� �������
	wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	// �������� ����� ����
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	return RegisterClass(&wcWindowClass); // ����������� ������
}

////////////////////////////////////////////////////////////////////////// 
// ������� ��������� ���������
LRESULT CALLBACK WndProc(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:

		timerTick(hWnd);
		break;

	case WM_DESTROY:
		close(hWnd);
		PostQuitMessage(0);  // ������� �� ���������
		break;
	
	case WM_LBUTTONDOWN: // mouse click
		mouseDownFlag = 1;
		break;

	case WM_LBUTTONUP:
		mouseDownFlag = 0;
		break;

	case WM_MOUSEWHEEL:
		mouseWheel(wParam);
		break;

	case WM_SIZE:
		resize(hWnd);
		break;


	case WM_KEYDOWN: // kolesiko
		switch (wParam)
		{
			case VK_RIGHT: // move right key
				//X = X + step;
				X_speed += 5;
				refreshScreen();
				break;
			case VK_LEFT: // move left key
				//X = X - step;
				X_speed -= 5;
				refreshScreen();
				break;
			case VK_UP: // move up key
				//Y = Y - step;
				Y_speed -= 5;
				refreshScreen();
				break;
			case VK_DOWN: // move down key
				//Y = Y + step;
				Y_speed += 5;
				refreshScreen();
				break;
			case VK_TAB: // change backgroud color
				curColorWindow = (curColorWindow++) % 10;
				refreshScreen();
				break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	/*
	BITMAP bm;
	//HBITMAP hBMP = (HBITMAP)::LoadImage(NULL, "BMP.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP hBMP = LoadBitmapA(HInstance, "BMP.bmp");
	HDC hdcMem = CreateCompatibleDC(hdc);
	HGDIOBJ hbmOld = SelectObject(hdcMem, hBMP);
	GetObject(hBMP, sizeof(bm), &bm);
	BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
	*/
	return 0;
}

int close(HWND hWnd)
{
	KillTimer(hWnd, 1);
	return 0;
}

int refreshScreen()
{
	FillRect(hdc, &win_rect, HBRUSH(curColorWindow));
	correctXY();
	//HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, "bitmap3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//BITMAP bitmap;
	//HDC hdcMem = CreateCompatibleDC(hdc);
	//HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmap);
	//GetObject(hBitmap, sizeof(bitmap), &bitmap);
	//BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

	//SelectObject(hdcMem, oldBitmap);

	return drawObj();
	//return 0;
}

void mouseWheel(WPARAM wParam)
{
	int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	int fwKeys = GET_KEYSTATE_WPARAM(wParam);
	if (fwKeys) {
		if (zDelta > 0) {
			X = X + step;
		}
		if (zDelta < 0) {
			X = X - step;
		}
	}
	else {
		if (zDelta > 0) {
			Y = Y + step;
		}
		if (zDelta < 0) {
			Y = Y - step;
		}
	}
	refreshScreen();
}

int timerTick(HWND hWnd)
{
	if (mouseDownFlag) {
		POINT pos; RECT rc;
		GetCursorPos(&pos);
		GetClientRect(hWnd, &rc);
		MapWindowPoints(hWnd, GetParent(hWnd), (LPPOINT)&rc, 2);
		X = pos.x - rc.left;
		Y = pos.y - rc.top;
		refreshScreen();
	}
	if ((X_speed == 0) && (Y_speed == 0)) { return 0; }
	X = X + X_speed;
	Y = Y + Y_speed;
	X_speed = div(X_speed, 2).quot; 
	Y_speed = div(Y_speed, 2).quot;
	return refreshScreen();
}

void correctXY()
{
	X = correct(X, 0, window_width, rect_width, &X_speed, X_boost);
	Y = correct(Y, 0, window_height, rect_height, &Y_speed, Y_boost);
	return;
}

int correct(int value, int minValue, int maxValue, int len, int* speed, int boost)
{
	if ((value >= minValue) && (value <= maxValue - len)) {
		return value;
	}
	if (value < minValue)
	{
		*speed += boost;
		return minValue;
	}
	*speed -= boost;
	return maxValue - len;
}

void resize(HWND hWnd)
{
	RECT window_rect;
	GetClientRect(hWnd, &window_rect); // ���������� ������
	window_width = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;
	correctXY();
	win_rect = { 0, 0, window_width, window_height };
	refreshScreen();
}

int drawObj()
{
	///DrawIconEx(hdc, X, Y, hi, rect_width, rect_height, 0, NULL, DI_NORMAL);
	//BLENDFUNCTION Blend { AC_SRC_OVER, 0, 50, AC_SRC_ALPHA };
	
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hBmpCirc = (HBITMAP)LoadImage(NULL, "bitmap1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//static BITMAP bmpCirc;
	//GetObject(hBmpCirc, sizeof(bmpCirc), (LPSTR)&bmpCirc);
	SelectObject(hMemDC, hBmpCirc);
	//BitBlt(hdc, 0, 0, bmpCirc.bmWidth, bmpCirc.bmHeight, hMemDC, 0, 0, SRCCOPY);
	//bool ans = AlphaBlend(hdc, X, Y, 48, 48, hMemDC, 0, 0, 48, 48, Blend);
	
	bool ans = TransparentBlt(hdc, X, Y, 48, 48, hMemDC, 0, 0, 48, 48, RGB(255, 255, 0));


	return 0;
	//RECT rect{ X, Y, X + rect_width, Y + rect_height };
	//return FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 3));
}




