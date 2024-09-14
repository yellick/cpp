#include <windows.h>
#include <stdlib.h>
#include <time.h>

// Глобальные переменные
const char g_szClassName[] = "myWindowClass";
const int LIGHT_RADIUS = 30;
const int LIGHT_SPACING = 70;
const int TIMER_ID = 1;
int TIMER_INTERVAL = 500; // 1 секунда

bool switcher = true;

// Перечисление для состояний светофора
enum class TrafficLightState {
	RED,
	YELLOW,
	GREEN
};

// Перечисление для состояний второго светофора
enum class TrafficLightState2 {
	RED,
	GREEN
};

// Текущее состояние первого светофора
TrafficLightState currentState = TrafficLightState::RED;

// Текущее состояние второго светофора
TrafficLightState2 currentState2 = TrafficLightState2::GREEN;

// Прототипы функций
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DrawTrafficLight(HDC hdc, int x, int y, TrafficLightState state);
void DrawTrafficLight2(HDC hdc, int x, int y, TrafficLightState2 state);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	// Регистрация класса окна
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Создание окна
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Traffic Light Simulator",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 480, 360,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Установка таймера
	SetTimer(hwnd, TIMER_ID, TIMER_INTERVAL, NULL);

	// Основной цикл сообщений
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		DrawTrafficLight(hdc, 80, 50, currentState);
		DrawTrafficLight2(hdc, 240, 50, currentState2);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_TIMER:
		if (wParam == TIMER_ID)
		{
			if (currentState == TrafficLightState::RED) {
				switcher = true;
			}
			if (currentState == TrafficLightState::GREEN) {
				switcher = false;
			}
			
			if (switcher) {
				currentState = (TrafficLightState)(int(currentState) + 1);
			}
			else {
				currentState = (TrafficLightState)(int(currentState) - 1);
			}


			// Обновляем состояние второго светофора
			if (currentState == TrafficLightState::RED || currentState == TrafficLightState::GREEN)
			{
				currentState2 = (currentState == TrafficLightState::RED) ? TrafficLightState2::GREEN : TrafficLightState2::RED;
			}

			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;
	case WM_DESTROY:
		KillTimer(hwnd, TIMER_ID);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void DrawTrafficLight(HDC hdc, int x, int y, TrafficLightState state)
{
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
	HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0));
	HBRUSH grayBrush = CreateSolidBrush(RGB(128, 128, 128));

	// Рисуем красный свет
	SelectObject(hdc, (state == TrafficLightState::RED) ? redBrush : grayBrush);
	Ellipse(hdc, x - LIGHT_RADIUS, y - LIGHT_RADIUS, x + LIGHT_RADIUS, y + LIGHT_RADIUS);

	// Рисуем желтый свет
	SelectObject(hdc, (state == TrafficLightState::YELLOW) ? yellowBrush : grayBrush);
	Ellipse(hdc, x - LIGHT_RADIUS, y + LIGHT_SPACING - LIGHT_RADIUS, x + LIGHT_RADIUS, y + LIGHT_SPACING + LIGHT_RADIUS);

	// Рисуем зеленый свет
	SelectObject(hdc, (state == TrafficLightState::GREEN) ? greenBrush : grayBrush);
	Ellipse(hdc, x - LIGHT_RADIUS, y + 2 * LIGHT_SPACING - LIGHT_RADIUS, x + LIGHT_RADIUS, y + 2 * LIGHT_SPACING + LIGHT_RADIUS);

	DeleteObject(redBrush);
	DeleteObject(yellowBrush);
	DeleteObject(greenBrush);
	DeleteObject(grayBrush);
}

void DrawTrafficLight2(HDC hdc, int x, int y, TrafficLightState2 state)
{
	HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH greenBrush = CreateSolidBrush(RGB(0, 255, 0));
	HBRUSH grayBrush = CreateSolidBrush(RGB(128, 128, 128));

	// Рисуем красный свет
	SelectObject(hdc, (state == TrafficLightState2::RED) ? redBrush : grayBrush);
	Ellipse(hdc, x - LIGHT_RADIUS, y - LIGHT_RADIUS, x + LIGHT_RADIUS, y + LIGHT_RADIUS);

	// Рисуем зеленый свет
	SelectObject(hdc, (state == TrafficLightState2::GREEN) ? greenBrush : grayBrush);
	Ellipse(hdc, x - LIGHT_RADIUS, y + LIGHT_SPACING - LIGHT_RADIUS, x + LIGHT_RADIUS, y + LIGHT_SPACING + LIGHT_RADIUS);

	DeleteObject(redBrush);
	DeleteObject(greenBrush);
	DeleteObject(grayBrush);
}