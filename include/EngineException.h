#pragma once

#include <string>
#include <source_location>

#define GET_EXCEPTION_FILE __FILEW__  //! StringConversion::Utf8ToWstring(std::source_location::current().file_name())
#define GET_EXCEPTION_LINE std::to_wstring(__LINE__)  //! std::to_wstring(std::source_location::current().line())
#define GET_EXCEPTION_COLUMN                                                                                           \
    std::to_wstring(__builtin_COLUMN())  //! std::to_wstring(std::source_location::current().column())

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

    [[nodiscard]] const constexpr std::wstring& GetFile() const {
        return file;
    }

    [[nodiscard]] const constexpr std::wstring& GetLine() const {
        return line;
    }

    [[nodiscard]] const constexpr std::wstring& GetColumn() const {
        return column;
    }

    [[nodiscard]] const constexpr std::wstring& GetNote() const {
        return note;
    }

    [[nodiscard]] const constexpr std::wstring GetLocation() const {
        return std::wstring(L"Line [") + line + L"] in " + file;
    }

    [[nodiscard]] virtual std::wstring GetFullMessage() const = 0;
    [[nodiscard]] virtual std::wstring GetExceptionType() const = 0;

private:
    std::wstring file;
    std::wstring line;
    std::wstring column;
    std::wstring note;
};
