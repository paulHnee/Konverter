#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// Function for converting numbers between bases
char* convertNumber(const char* input, int fromBase, int toBase) {
    static char result[65]; // Store the result as a string (up to 64 characters for binary)
    long long number = strtoll(input, NULL, fromBase); // Convert input to integer based on fromBase

    int index = 63;
    result[64] = '\0';

    do {
        int remainder = number % toBase;
        result[index--] = (remainder < 10) ? '0' + remainder : 'A' + (remainder - 10);
        number /= toBase;
    } while (number > 0);

    return &result[index + 1]; // Return the result string starting from the first non-empty character
}

// Window message processing function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hwndInput, hwndFromBase, hwndToBase, hwndResult, hwndConvertButton;
    static char input[32], fromBase[3], toBase[3];

    switch (uMsg) {
        case WM_CREATE: {
            CreateWindow("STATIC", "Enter number:", WS_VISIBLE | WS_CHILD, 20, 20, 100, 20, hwnd, NULL, NULL, NULL);
            hwndInput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 20, 150, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "From base:", WS_VISIBLE | WS_CHILD, 20, 60, 100, 20, hwnd, NULL, NULL, NULL);
            hwndFromBase = CreateWindow("EDIT", "10", WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 60, 50, 20, hwnd, NULL, NULL, NULL);

            CreateWindow("STATIC", "To base:", WS_VISIBLE | WS_CHILD, 20, 100, 100, 20, hwnd, NULL, NULL, NULL);
            hwndToBase = CreateWindow("EDIT", "10", WS_VISIBLE | WS_CHILD | WS_BORDER, 130, 100, 50, 20, hwnd, NULL, NULL, NULL);

            hwndConvertButton = CreateWindow("BUTTON", "Convert", WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 140, 80, 30, hwnd, (HMENU)1, NULL, NULL);
            hwndResult = CreateWindow("STATIC", "Result:", WS_VISIBLE | WS_CHILD, 20, 180, 300, 20, hwnd, NULL, NULL, NULL);
            break;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) { // Convert button clicked
                GetWindowText(hwndInput, input, sizeof(input));
                GetWindowText(hwndFromBase, fromBase, sizeof(fromBase));
                GetWindowText(hwndToBase, toBase, sizeof(toBase));

                int fromBaseInt = atoi(fromBase);
                int toBaseInt = atoi(toBase);

                // Perform the conversion and update the result label
                char* result = convertNumber(input, fromBaseInt, toBaseInt);
                SetWindowText(hwndResult, result);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "BaseConverterWindow";
    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Base Converter",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
