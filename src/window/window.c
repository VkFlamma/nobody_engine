#include "window.h"

#include <stdlib.h>
#include <assert.h>

#include "util/defines.h"

struct Window* globalWindow;

void windowHandleInput(struct Window* window, HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool down) {
	bool repeat = (HIWORD(lParam) & KF_REPEAT);
	switch (wParam) {
	case VK_ESCAPE:
		window->close = true;
		break;
	case VK_SPACE:
		keyboardUpdate(&window->keyboard, KEY_SPACE, down, repeat);
		break;
	case 0x30: // 0
		keyboardUpdate(&window->keyboard, KEY_ZERO, down, repeat);
		break;
	case 0x31: // 1
		keyboardUpdate(&window->keyboard, KEY_ONE, down, repeat);
		break;
	case 0x32: // 2
		keyboardUpdate(&window->keyboard, KEY_TWO, down, repeat);
		break;
	case 0x33: // 3
		keyboardUpdate(&window->keyboard, KEY_THREE, down, repeat);
		break;
	case 0x34: // 4
		keyboardUpdate(&window->keyboard, KEY_FOUR, down, repeat);
		break;
	case 0x35: // 5
		keyboardUpdate(&window->keyboard, KEY_FIVE, down, repeat);
		break;
	case 0x36: // 6
		keyboardUpdate(&window->keyboard, KEY_SIX, down, repeat);
		break;
	case 0x37: // 7
		keyboardUpdate(&window->keyboard, KEY_SEVEN, down, repeat);
		break;
	case 0x38: // 8
		keyboardUpdate(&window->keyboard, KEY_EIGHT, down, repeat);
		break;
	case 0x39: // 9
		keyboardUpdate(&window->keyboard, KEY_NINE, down, repeat);
		break;
	case 0x41: // A
		keyboardUpdate(&window->keyboard, KEY_A, down, repeat);
		break;
	case 0x42: // B
		keyboardUpdate(&window->keyboard, KEY_B, down, repeat);
		break;
	case 0x43: // C
		keyboardUpdate(&window->keyboard, KEY_C, down, repeat);
		break;
	case 0x44: // D
		keyboardUpdate(&window->keyboard, KEY_D, down, repeat);
		break;
	case 0x45: // E
		keyboardUpdate(&window->keyboard, KEY_E, down, repeat);
		break;
	case 0x46: // F
		keyboardUpdate(&window->keyboard, KEY_F, down, repeat);
		break;
	case 0x47: // G
		keyboardUpdate(&window->keyboard, KEY_G, down, repeat);
		break;
	case 0x48: // H
		keyboardUpdate(&window->keyboard, KEY_H, down, repeat);
		break;
	case 0x49: // I
		keyboardUpdate(&window->keyboard, KEY_I, down, repeat);
		break;
	case 0x4A: // J
		keyboardUpdate(&window->keyboard, KEY_J, down, repeat);
		break;
	case 0x4B: // K
		keyboardUpdate(&window->keyboard, KEY_K, down, repeat);
		break;
	case 0x4C: // L
		keyboardUpdate(&window->keyboard, KEY_L, down, repeat);
		break;
	case 0x4D: // M
		keyboardUpdate(&window->keyboard, KEY_M, down, repeat);
		break;
	case 0x4E: // N
		keyboardUpdate(&window->keyboard, KEY_N, down, repeat);
		break;
	case 0x4F: // O
		keyboardUpdate(&window->keyboard, KEY_O, down, repeat);
		break;
	case 0x50: // P
		keyboardUpdate(&window->keyboard, KEY_O, down, repeat);
		break;
	case 0x51: // Q
		keyboardUpdate(&window->keyboard, KEY_Q, down, repeat);
		break;
	case 0x52: // R
		keyboardUpdate(&window->keyboard, KEY_R, down, repeat);
		break;
	case 0x53: // S
		keyboardUpdate(&window->keyboard, KEY_S, down, repeat);
		break;
	case 0x54: // T
		keyboardUpdate(&window->keyboard, KEY_T, down, repeat);
		break;
	case 0x55: // U
		keyboardUpdate(&window->keyboard, KEY_U, down, repeat);
		break;
	case 0x56: // V
		keyboardUpdate(&window->keyboard, KEY_V, down, repeat);
		break;
	case 0x57: // W
		keyboardUpdate(&window->keyboard, KEY_W, down, repeat);
		break;
	case 0x58: // X
		keyboardUpdate(&window->keyboard, KEY_X, down, repeat);
		break;
	case 0x59: // Y
		keyboardUpdate(&window->keyboard, KEY_Y, down, repeat);
		break;
	case 0x5A: // Z
		keyboardUpdate(&window->keyboard, KEY_Z, down, repeat);
		break;
	default:
		break;
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY:
		globalWindow->close = true;
		break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
		break;
	case WM_KEYDOWN:
		windowHandleInput(globalWindow, hwnd, uMsg, wParam, lParam, true);
		break;
	case WM_KEYUP:
		windowHandleInput(globalWindow, hwnd, uMsg, wParam, lParam, false);
		break;
	case WM_MOUSEMOVE:
		mouseMove(&globalWindow->mouse, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONDOWN:
		mouseUpdate(&globalWindow->mouse, BUTTON_LEFT, true);
		break;
	case WM_LBUTTONUP:
		mouseUpdate(&globalWindow->mouse, BUTTON_LEFT, false);
		break;
	case WM_RBUTTONDOWN:
		mouseUpdate(&globalWindow->mouse, BUTTON_RIGHT, true);
		break;
	case WM_RBUTTONUP:
		mouseUpdate(&globalWindow->mouse, BUTTON_RIGHT, false);
		break;
	case WM_MOUSEWHEEL:
		mouseWheelUpdate(&globalWindow->mouse, GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	default:
		break;
	}

	if (globalWindow->close) {
		DestroyWindow(hwnd);
		PostQuitMessage(0);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

struct Window* windowInit() {
	struct Window* window = malloc(sizeof(struct Window));
	assert(window);

	globalWindow = window;
	window->close = false;
	window->hInstance = GetModuleHandle(NULL);

	const wchar_t CLASS_NAME[] = L"Voxel";

	WNDCLASSEXW wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = window->hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	
	window->x = WINDOW_MARGIN;
	window->y = WINDOW_MARGIN;
	window->width = desktop.right - WINDOW_MARGIN * 2;
	window->height = desktop.bottom - WINDOW_MARGIN * 2;

	window->hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		CLASS_NAME,
		L"Vulkan",
		WS_OVERLAPPEDWINDOW,
		window->x, window->y, window->width, window->height,
		NULL,
		NULL,
		window->hInstance,
		NULL
	);

	ShowWindow(window->hwnd, SW_SHOWNORMAL);

	keyboardInit(&window->keyboard);
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(window->hwnd, &p);
	mouseInit(&window->mouse, p.x, p.y);

	return window;
}

void windowDestroy(struct Window* window) {
	free(window);
}

bool windowLoop(struct Window* window) {
	keyboardReset(&window->keyboard);
	mouseReset(&window->mouse);
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		return !window->close;
	}
	return true;
}
