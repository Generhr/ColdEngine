#pragma once

#include <locale>
#include <codecvt>
#include <string>


std::string ConvertWStringToUTF8(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    return converter.to_bytes(wstr);
}
