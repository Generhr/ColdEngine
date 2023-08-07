#include "MainWindow.h"

#include "Engine.h"
#include "Graphics.h"
#include "Resource.h"

#include <cassert>


MainWindow::MainWindow(HINSTANCE hInst, wchar_t* pArgs) : hInst(hInst), args(pArgs) {
    // Register window class
    WNDCLASSEX wc = {sizeof(WNDCLASSEX),
        CS_CLASSDC,
        __HandleMsgSetup,
        0,
        0,
        hInst,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        wndClassName,
        nullptr};
    wc.hIconSm = reinterpret_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, 0));
    wc.hIcon = reinterpret_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClassEx(&wc)) {
        MessageBox(nullptr, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    }

    // Create window & get hWnd
    RECT wr;
    wr.left = 350;
    wr.right = Graphics::ScreenWidth + wr.left;
    wr.top = 100;
    wr.bottom = Graphics::ScreenHeight + wr.top;

    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    hWnd = CreateWindow(wndClassName,
        "Cold DirectX Framework",
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        wr.left,
        wr.top,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        hInst,
        this);

    // Throw an exception if something went terribly wrong
    if (hWnd == nullptr) {
        throw Exception(ENGINE_EXCEPTION_FILE,
            ENGINE_EXCEPTION_LINE,
            ENGINE_EXCEPTION_COLUMN,
            L"Failed to get valid window handle.");
    }

    // Show and update
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);
}

MainWindow::~MainWindow() {
    // Unregister window class
    UnregisterClass(static_cast<LPCSTR>(wndClassName), hInst);
}

bool MainWindow::IsActive() const {
    return GetActiveWindow() == hWnd;
}

bool MainWindow::IsMinimized() const {
    return IsIconic(hWnd) != 0;
}

void MainWindow::ShowMessageBox(const std::wstring& title, const std::wstring& message) const {
    MessageBoxW(hWnd, message.c_str(), title.c_str(), MB_OK);
}

bool MainWindow::ProcessMessage() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT) {
            return false;
        }
    }

    return true;
}

LRESULT WINAPI MainWindow::__HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side.
    if (msg == WM_NCCREATE) {
        // Extract ptr to window class from creation data.
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        auto const pWnd = reinterpret_cast<MainWindow*>(pCreate->lpCreateParams);

        // Sanity check
        assert(pWnd != nullptr);

        // Set WinAPI-managed user data to store ptr to window class.
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // Set message proc to normal (non-setup) handler now that setup is finished.
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&MainWindow::__HandleMsgThunk));

        // Forward message to window class handler.
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    // If we get a message before the WM_NCCREATE message, handle with default handler.
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI MainWindow::__HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Retrieve ptr to window class.
    auto const pWnd = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    // Forward message to window class handler.
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT MainWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);

            break;

        // ************ KEYBOARD MESSAGES ************ //
        case WM_KEYDOWN:
            if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) {  // No thank you on the autorepeat
                kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
            }

            break;
        case WM_KEYUP:
            kbd.OnKeyReleased(static_cast<unsigned char>(wParam));

            break;
        case WM_CHAR:
            kbd.OnChar(static_cast<unsigned char>(wParam));

            break;

        // ************ MOUSE MESSAGES ************ //
        case WM_MOUSEMOVE: {
            POINTS pt = MAKEPOINTS(lParam);

            if (pt.x > 0 && pt.x < Graphics::ScreenWidth && pt.y > 0 && pt.y < Graphics::ScreenHeight) {
                mouse.OnMouseMove(pt.x, pt.y);

                if (!mouse.IsInWindow()) {
                    SetCapture(hWnd);
                    mouse.OnMouseEnter();
                }
            }
            else {
                if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
                    pt.x = std::max(static_cast<short>(0), pt.x);
                    pt.x = std::min(static_cast<short>(Graphics::ScreenWidth - 1), pt.x);
                    pt.y = std::max(static_cast<short>(0), pt.y);
                    pt.y = std::min(static_cast<short>(Graphics::ScreenHeight - 1), pt.y);
                    mouse.OnMouseMove(pt.x, pt.y);
                }
                else {
                    ReleaseCapture();
                    mouse.OnMouseLeave();
                    mouse.OnLeftReleased(pt.x, pt.y);
                    mouse.OnRightReleased(pt.x, pt.y);
                }
            }

            break;
        }
        case WM_LBUTTONDOWN: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnLeftPressed(pt.x, pt.y);

            break;
        }
        case WM_RBUTTONDOWN: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnRightPressed(pt.x, pt.y);

            break;
        }
        case WM_LBUTTONUP: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnLeftReleased(pt.x, pt.y);

            break;
        }
        case WM_RBUTTONUP: {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnRightReleased(pt.x, pt.y);

            break;
        }
        case WM_MOUSEWHEEL: {
            const POINTS pt = MAKEPOINTS(lParam);

            if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
                mouse.OnWheelUp(pt.x, pt.y);
            }
            else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
                mouse.OnWheelDown(pt.x, pt.y);
            }

            break;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
