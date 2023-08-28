#pragma once

#include "Win.h"

#include "EngineException.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <string>

class HWNDKey {
    friend Graphics::Graphics(HWNDKey&);

public:
    ~HWNDKey() = default;

    HWNDKey(const HWNDKey&) = delete;
    HWNDKey& operator=(const HWNDKey&) = delete;
    HWNDKey(HWNDKey&&) = delete;
    HWNDKey& operator=(HWNDKey&&) = delete;

    [[nodiscard]] HWND GetHWND() const {
        return hWnd;
    }

protected:
    HWNDKey() = default;

protected:
    HWND hWnd = nullptr;
};

class MainWindow : public HWNDKey {
public:
    class Exception : public EngineException {
    public:
        ~Exception() override = default;

        using EngineException::EngineException;

        [[nodiscard]] std::wstring GetFullMessage() const override {
            return GetNote() + L"\nAt: " + GetLocation();
        }

        [[nodiscard]] std::wstring GetExceptionType() const override {
            return L"Windows Exception";
        }
    };

    MainWindow(HINSTANCE hInst, wchar_t* pArgs);

    MainWindow(const MainWindow&) = delete;

    MainWindow& operator=(const MainWindow&) = delete;

    ~MainWindow();

    [[nodiscard]] bool IsActive() const;

    [[nodiscard]] bool IsMinimized() const;

    void ShowMessageBox(const std::wstring& title, const std::wstring& message) const;

    void Kill() {
        PostQuitMessage(0);
    }

    // Returns false if quitting
    static bool ProcessMessage();

    const std::wstring& GetArgs() const {
        return args;
    }

private:
    static LRESULT WINAPI __HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT WINAPI __HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    Keyboard kbd;
    Mouse mouse;

private:
    static constexpr char wndClassName[] = "DirectX Framework Window";
    HINSTANCE hInst = nullptr;
    std::wstring args;
};
