#define FULL_WINTARD

#include "GDIPlusManager.h"

#include "Win.h" /* Windows.h */

#include <algorithm>

// namespace Gdiplus {
//     using std::max;
//     using std::min;
// }  // namespace Gdiplus

#include <gdiplus.h>  //~ https://stackoverflow.com/questions/7305614/include-gdiplus-h-causes-error

#pragma comment(lib,                                                                                                   \
        "gdiplus.lib")  //~ Sometimes if a new header file is added, and this error starts coming due to that, you need
                        //~ to add library as well to get rid of unresolved external symbol.

GDIPlusManager::GDIPlusManager() {
    if (refCount++ == 0) {
        Gdiplus::GdiplusStartupInput input;

        input.GdiplusVersion = 1;
        input.DebugEventCallback = nullptr;
        input.SuppressBackgroundThread = false;
        input.SuppressExternalCodecs = false;

        Gdiplus::GdiplusStartup(&token, &input, nullptr);
    }
}

GDIPlusManager::~GDIPlusManager() {
    if (--refCount == 0) {
        Gdiplus::GdiplusShutdown(token);
    }
}
