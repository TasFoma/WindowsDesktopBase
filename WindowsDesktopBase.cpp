// WindowsDesktopBase.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "WindowsDesktopBase.h"

#define MAX_LOADSTRING 100
struct WindowData
{
    bool isGreen; // Цвет крестика для этого окна 
    int mouseX;   // Координата X мыши 
    int mouseY;   // Координата Y мыши
};
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
bool g_isGreen = false; // Флаг текущего цвета (false - красный, true - зеленый)

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSDESKTOPBASE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSDESKTOPBASE));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSDESKTOPBASE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSDESKTOPBASE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    // Создаем данные для первого окна
    WindowData* pWindowData1 = new WindowData();
    pWindowData1->isGreen = false;
    pWindowData1->mouseX = 0;
    pWindowData1->mouseY = 0;

    HWND hWnd1 = CreateWindowW(szWindowClass, L"Окно 1", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, pWindowData1); // Последний параметр - наши данные!

    if (!hWnd1)
    {
        delete pWindowData1; // Если окно не создалось, чистим память
        return FALSE;
    }

    ShowWindow(hWnd1, nCmdShow);
    UpdateWindow(hWnd1);

    // Создаем данные для второго окна
    WindowData* pWindowData2 = new WindowData();
    pWindowData2->isGreen = false;
    pWindowData2->mouseX = 0;
    pWindowData2->mouseY = 0;

    HWND hWnd2 = CreateWindowW(szWindowClass, L"Окно 2", WS_OVERLAPPEDWINDOW,
        500, 100, 400, 400, nullptr, nullptr, hInstance, pWindowData2); // Другое положение

    if (!hWnd2)
    {
        delete pWindowData2;
        return FALSE;
    }

    ShowWindow(hWnd2, nCmdShow);
    UpdateWindow(hWnd2);

    return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_MOUSEMOVE:
    {
        WindowData* pData = (WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (pData)
        {
            pData->mouseX = LOWORD(lParam); // Обновляем координаты
            pData->mouseY = HIWORD(lParam);
            InvalidateRect(hWnd, NULL, TRUE); // Перерисовываем
        }
    }
    break;
    case WM_LBUTTONDOWN:
    {
        g_isGreen = !g_isGreen; // Меняем цвет
        InvalidateRect(hWnd, NULL, TRUE); // Принудительно перерисовываем окно
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps); // Получаем контекст устройства для рисования

        // Получаем указатель на данные окна
        WindowData* pData = (WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

        // Получаем размеры клиентской области окна
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        // Создаем перо толщиной 3 пикселя выбираем цвет в зависимости от состояния флага
        HPEN hRedPen = CreatePen(PS_SOLID, 3, g_isGreen ? RGB(0, 255, 0) : RGB(255, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hRedPen); // Выбираем перо в контекст  

        // Рисуем первую диагональ (из левого верхнего в правый нижний угол)
        MoveToEx(hdc, clientRect.left, clientRect.top, NULL);
        LineTo(hdc, clientRect.right, clientRect.bottom);

        // Рисуем вторую диагональ (из правого верхнего в левый нижний угол)
        MoveToEx(hdc, clientRect.right, clientRect.top, NULL);
        LineTo(hdc, clientRect.left, clientRect.bottom);

        // Восстанавливаем старое перо и удаляем созданное
        SelectObject(hdc, hOldPen);
        DeleteObject(hRedPen);

        // Формируем и выводим строку с координатами (ДО EndPaint!)
        if (pData) // Проверяем, что данные есть
        {
            WCHAR coordsText[64];
            swprintf_s(coordsText, L"X: %d, Y: %d", pData->mouseX, pData->mouseY);
            TextOut(hdc, 10, 10, coordsText, wcslen(coordsText));
        }

        EndPaint(hWnd, &ps); // Завершаем рисование
    } 
    break;
    case WM_CREATE:
    {
        // Сохраняем переданные данные
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        WindowData* pData = (WindowData*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pData);
    }
    break;

    case WM_DESTROY:
    {
        // Освобождаем память
        WindowData* pData = (WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        delete pData;
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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
