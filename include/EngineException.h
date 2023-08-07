#pragma once

#include <string>
#include <source_location>


#define ENGINE_EXCEPTION_FILE                                                                                          \
    std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(std::source_location::current().file_name())
#define ENGINE_EXCEPTION_LINE std::to_wstring(std::source_location::current().line())
#define ENGINE_EXCEPTION_COLUMN std::to_wstring(std::source_location::current().column())

class EngineException {
public:
    EngineException(std::wstring file, std::wstring line, std::wstring column, std::wstring note = L"")
        : file(std::move(file)), line(std::move(line)), column(std::move(column)), note(std::move(note)) {
    }

    virtual ~EngineException() = default;

    EngineException(const EngineException&) = delete;
    EngineException& operator=(const EngineException&) = delete;
    EngineException(EngineException&&) = delete;
    EngineException& operator=(EngineException&&) = delete;

    const constexpr std::wstring& GetFile() const {
        return file;
    }

    const constexpr std::wstring& GetLine() const {
        return line;
    }

    const constexpr std::wstring& GetColumn() const {
        return column;
    }

    const constexpr std::wstring& GetNote() const {
        return note;
    }

    const constexpr std::wstring GetLocation() const {
        return std::wstring(L"Line [") + line + L"] in " + file;
    }

    virtual std::wstring GetFullMessage() const = 0;
    virtual std::wstring GetExceptionType() const = 0;

private:
    std::wstring file;
    std::wstring line;
    std::wstring column;
    std::wstring note;
};
