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
int rect_width = 100; int rect_height = 100;
int X_speed = 0, Y_speed = 0;
int X_boost = 50,
Y_boost = 50;
RECT win_rect{ 0, 0, window_width, window_height };
HDC hdc;
HINSTANCE HInstance;

////////////////////////////////////////////////////////////////////////// 
// функци€ вхождений программы WinMain
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE         hPrevInstance,
	LPSTR             lpCmdLine,
	int               nCmdShow)
{
	//HICON hIcon = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(0), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	//DrawIconEx(hdc, 300, 300, hIcon, 0, 0, 0, NULL, DI_NORMAL);
	//DestroyIcon(hIcon);
	
	HInstance = hInstance;
	// им€ будущего класса
	LPCTSTR lpzClass = TEXT("lab-1");

	// регистраци€ класса
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1;

	// вычисление координат центра экрана
	RECT screen_rect, window_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect); // разрешение экрана
	//AdjustWindowRect(
	int x = screen_rect.right / 2 - width/2;
	int y = screen_rect.bottom / 2 - height/2;

	// создание окна
	HWND hWnd = CreateWindow(lpzClass, TEXT("lab-1"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER, x, y, width, height, NULL, NULL,
		hInstance, NULL);

	GetClientRect(hWnd, &window_rect); // разрешение экрана
	window_width = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;

	// если окно не создано, описатель будет равен 0
	if (!hWnd) return 2;

	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);
	MoveToEx(hdc, X, Y, NULL);
	drawObj();
	//LineTo(hdc, 100, 100);
	//CBrush brush;
	
	//ReleaseDC(hWnd, hdc);

	SetTimer(hWnd, 1, 80, NULL);
	MSG msg = { 0 };    // структура сообщени€
	int iGetOk = 0;   // переменна€ состо€ни€
	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0) // цикл сообщений
	{
		if (iGetOk == -1) return 3;  // если GetMessage вернул ошибку - выход
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}



	return msg.wParam;  // возвращаем код завершени€ программы
}

////////////////////////////////////////////////////////////////////////// 
// функци€ регистрации класса окон
ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
	WNDCLASS wcWindowClass = { 0 };
	// адрес ф-ции обработки сообщений
	wcWindowClass.lpfnWndProc = (WNDPROC)WndProc;
	// стиль окна
	wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	// дискриптор экземпл€ра приложени€
	wcWindowClass.hInstance = hInst;
	// название класса
	wcWindowClass.lpszClassName = lpzClassName;
	// загрузка курсора
	wcWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	// загрузка цвета окон
	wcWindowClass.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;
	return RegisterClass(&wcWindowClass); // регистраци€ класса
}

////////////////////////////////////////////////////////////////////////// 
// функци€ обработки сообщений
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
		PostQuitMessage(0);  // реакци€ на сообщение
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
	return drawObj();
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
	GetClientRect(hWnd, &window_rect); // разрешение экрана
	window_width = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;
	correctXY();
	win_rect = { 0, 0, window_width, window_height };
	refreshScreen();
}

int drawObj()
{
	DrawIconEx(hdc, X, Y, hi, rect_width, rect_height, 0, NULL, DI_NORMAL);
	return 0;
	//RECT rect{ X, Y, X + rect_width, Y + rect_height };
	//return FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 3));
}




