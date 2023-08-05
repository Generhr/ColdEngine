#include "Engine.h"
#include "MainWindow.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

struct game_state {
    // this contains the state of your game, such as positions and velocities
};

void update(game_state*) {
    // update game logic here
}

void render(game_state const&) {
    // render stuff here
}

game_state interpolate(game_state const& current, game_state const& previous, float alpha) {
    game_state interpolated_state;

    // interpolate between previous and current by alpha here

    return interpolated_state;
}

int main(int argc, char* argv[]) {
    HINSTANCE hInst = GetModuleHandle(nullptr);
    LPWSTR pArgs = GetCommandLineW();

    try {
        MainWindow wnd(hInst, pArgs);

        wnd.ShowMessageBox(L"Beep boop", L"Thumbs up!");

        try {
            using clock = std::chrono::high_resolution_clock;

            auto previousTime = clock::now();
            auto delta = std::chrono::duration<float> {0ms};

            constexpr auto timestep =
                std::chrono::duration<float, std::ratio<1, 60>> {1};  // Fixed timestep of 1/60th of a second.

            game_state current_state;
            game_state previous_state;
            Engine engine(wnd);

            auto lastFpsUpdate = previousTime;
            constexpr auto oneSecond = std::chrono::duration<float> {1000ms};
            float framesThisSecond = 0;
            float fps = 60.0F;

            while (wnd.ProcessMessage()) {
                auto const currentTime = clock::now();
                delta += std::chrono::duration<float> {currentTime - previousTime};
                previousTime = currentTime;

                // Handle Events

                // Update

                int numUpdateSteps = 0;

                while (delta >= timestep) {
                    delta -= timestep;

                    previous_state = current_state;
                    update(&current_state);  // Update at a fixed rate each time.

                    engine.UpdateModel();

                    if (++numUpdateSteps >= 240) {
                        std::cerr << "PANIC" << '\n';

                        // Panic

                        std::cin.get();
                    }
                }

                // FPS

                if (std::chrono::duration<float> {currentTime - lastFpsUpdate} >= oneSecond) {  // Update every second.
                    fps = (1 - 0.25f) * fps + 0.25f * (framesThisSecond / oneSecond.count());   // Compute the new FPS.

                    std::cout << static_cast<int>(fps) << " ("
                              << std::chrono::duration<float> {currentTime - lastFpsUpdate} << ")\n";

                    lastFpsUpdate = currentTime;
                    framesThisSecond = 0;
                }

                framesThisSecond++;

                // Render

                // Calculate how close or far we are from the next timestep:
                auto const alpha = delta / timestep;
                auto interpolated_state = interpolate(current_state, previous_state, alpha);

                render(interpolated_state);

                engine.ComposeFrame();

                // Throttle

                auto const frameTime = std::chrono::duration<float> {clock::now() - currentTime};
                static constexpr auto targetFrameTime =
                    std::chrono::duration<float, std::ratio<1, 60>> {1} -
                    5ms;  // You'll have to experiment with how much early. Too early and you burn too much CPU (and
                          // battery). Too little, and `sleep_for` occasionally wakes up too late when the CPU is under
                          // a high load.

                if (frameTime < targetFrameTime) {
                    auto const sleepDuration = targetFrameTime - frameTime;

                    std::this_thread::sleep_for(sleepDuration);
                }
            }
        }
        catch (const ColdException& e) {
            const std::wstring eMsg = e.GetFullMessage() + L"\n\nException caught at Windows message loop.";

            wnd.ShowMessageBox(e.GetExceptionType(), eMsg);
        }
        catch (const std::exception& e) {
            // need to convert std::exception what() string from narrow to wide string
            const std::string whatStr(e.what());
            const std::wstring eMsg =
                std::wstring(whatStr.begin(), whatStr.end()) + L"\n\nException caught at Windows message loop.";

            wnd.ShowMessageBox(L"Unhandled STL Exception", eMsg);
        }
        catch (...) {
            wnd.ShowMessageBox(L"Unhandled Non-STL Exception", L"\n\nException caught at Windows message loop.");
        }
    }
    catch (const ColdException& e) {
        const std::wstring eMsg = e.GetFullMessage() + L"\n\nException caught at main window creation.";

        // MessageBox(nullptr, eMsg.c_str(), e.GetExceptionType().c_str(), MB_OK);
    }
    catch (const std::exception& e) {
        // need to convert std::exception what() string from narrow to wide string
        const std::string whatStr(e.what());
        const std::wstring eMsg =
            std::wstring(whatStr.begin(), whatStr.end()) + L"\n\nException caught at main window creation.";

        // MessageBox(nullptr, eMsg.c_str(), L"Unhandled STL Exception", MB_OK);
    }
    catch (...) {
        // MessageBox(nullptr, L"\n\nException caught at main window creation.", L"Unhandled Non-STL Exception", MB_OK);
    }

    return 0;
}
