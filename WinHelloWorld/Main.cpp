#pragma warning(disable: 28251)

#include <Windows.h>
#include <new>

const wchar_t CLASS_NAME[] = L"MyWindow65756756755675755667656";

struct StateInfo {
	bool cool;
	void* pointtothevoid;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	// Make window class
	HCURSOR cursor = LoadCursor(hInstance, IDC_IBEAM);
	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = cursor;

	RegisterClass(&wc);

	// Make a state info
	StateInfo* pState = new (std::nothrow) StateInfo;
	if (pState == NULL) {
		return 1;
	}

	HWND hwnd = CreateWindowEx(
		0, // Optional Styles
		CLASS_NAME, // Class Name
		L"My Blank Window", // Window Text
		WS_OVERLAPPEDWINDOW, // Window Style

		// Size and Position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL, // Parent Window
		NULL, // Menu
		hInstance, // Instance Handle
		pState // Additional data
	);

	if (hwnd == NULL) {
		return 1;
	}

	ShowWindow(hwnd, nCmdShow);

	MSG msg = {};

	while (GetMessage(&msg, hwnd, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	// Painting the window
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_3DFACE + 1));
		EndPaint(hwnd, &ps);
	}
	return 0;

	// User requesting to close the window
	case WM_CLOSE:
		if (MessageBox(hwnd, L"Close window?", L"Notice", MB_OKCANCEL) == IDOK) {
			DestroyWindow(hwnd);
		}
		return 0;

	// Destroying the window
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	// Anything else
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam); // why is w before l wtf
	}
}