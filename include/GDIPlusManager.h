#pragma once

#include "Win.h" /* Windows.h */


class GDIPlusManager {
public:
    GDIPlusManager();
    ~GDIPlusManager();

    GDIPlusManager(const GDIPlusManager&) = delete;
    GDIPlusManager& operator=(const GDIPlusManager&) = delete;

    GDIPlusManager(GDIPlusManager&&) = delete;
    GDIPlusManager& operator=(GDIPlusManager&&) = delete;

private:
    ULONG_PTR token = 0;
    int refCount = 0;
};
