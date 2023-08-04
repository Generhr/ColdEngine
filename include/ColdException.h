#pragma once

#include <string>

class ColdException {
public:
    ColdException(std::wstring file, unsigned int line, std::wstring note = L"")
        : file(std::move(file)), line(line), note(std::move(note)) {
    }

    virtual ~ColdException() = default;

    ColdException(const ColdException&) = delete;
    ColdException& operator=(const ColdException&) = delete;
    ColdException(ColdException&&) = delete;
    ColdException& operator=(ColdException&&) = delete;

    const std::wstring& GetFile() const {
        return file;
    }

    unsigned int GetLine() const {
        return line;
    }

    const std::wstring& GetNote() const {
        return note;
    }

    std::wstring GetLocation() const {
        return std::wstring(L"Line [") + std::to_wstring(line) + L"] in " + file;
    }

    virtual std::wstring GetFullMessage() const = 0;
    virtual std::wstring GetExceptionType() const = 0;

private:
    std::wstring file;
    unsigned int line;
    std::wstring note;
};
