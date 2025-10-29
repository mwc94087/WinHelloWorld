#pragma warning(disable: 28251)

#include <Windows.h>
#include <new>

// Prototypes
struct StateInfo;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
inline StateInfo* GetStateInfo(HWND);
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int);

// Constansts
const wchar_t CLASS_NAME[] = L"WindowsHelloWorld";

// Data structures
struct StateInfo {
	bool cool = true;
};

// Definitions
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

inline StateInfo* GetStateInfo(HWND hwnd) {
	LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	return (StateInfo*)ptr;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	// Creating / initializing the window
	case WM_CREATE: {
		// Get the custom struct
		CREATESTRUCT* cStruct = (CREATESTRUCT*)lParam;
		StateInfo* pState = (StateInfo*)cStruct->lpCreateParams;

		// Put the custom struct into the window
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);

		return 0;
	}

	// Painting the window
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		RECT rec = ps.rcPaint;

		HBRUSH greenBrush = CreateSolidBrush(RGB(0, 200, 0));
		HBRUSH yellowBrush = CreateSolidBrush(RGB(200, 200, 0));

		FillRect(hdc, &rec, yellowBrush);

		if (rec.bottom - rec.top > 4 && rec.right - rec.left > 4) {
			rec.bottom -= 2;
			rec.top += 2;
			rec.right -= 2;
			rec.left += 2;

			FillRect(hdc, &rec, greenBrush);
		}

		DeleteObject(yellowBrush);
		DeleteObject(greenBrush);

		EndPaint(hwnd, &ps);

		return 0;
	}

	// Closing the window
	case WM_CLOSE: {
		DestroyWindow(hwnd);

		return 0;
	}

	// Destroying the window
	case WM_DESTROY: {
		PostQuitMessage(0);

		return 0;
	}

	// Anything else
	default: {
		return DefWindowProc(hwnd, uMsg, wParam, lParam); // why is w before l wtf}
	}
	}
}