#pragma once

#include <bitset>
#include <queue>

class Keyboard {
    friend class MainWindow;

public:
    class Event {
    public:
        enum Type { Press, Release, Invalid };

    private:
        Type type;
        unsigned char code;

    public:
        Event() : type(Invalid), code(0u) {
        }

        Event(Type type, unsigned char code) : type(type), code(code) {
        }

        [[nodiscard]] bool IsPress() const {
            return type == Press;
        }

        [[nodiscard]] bool IsRelease() const {
            return type == Release;
        }

        [[nodiscard]] bool IsValid() const {
            return type != Invalid;
        }

        [[nodiscard]] unsigned char GetCode() const {
            return code;
        }
    };

public:
    Keyboard() = default;

    ~Keyboard() = default;

    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;
    Keyboard(Keyboard&&) = delete;
    Keyboard& operator=(Keyboard&&) = delete;

    [[nodiscard]] bool KeyIsPressed(unsigned char keycode) const;
    Event ReadKey();
    [[nodiscard]] bool KeyIsEmpty() const;
    char ReadChar();
    [[nodiscard]] bool CharIsEmpty() const;
    void FlushKey();
    void FlushChar();
    void Flush();
    void EnableAutorepeat();
    void DisableAutorepeat();
    [[nodiscard]] bool AutorepeatIsEnabled() const;

private:
    void OnKeyPressed(unsigned char keycode);
    void OnKeyReleased(unsigned char keycode);
    void OnChar(char character);
    template<typename T>
    void TrimBuffer(std::queue<T>& buffer);

private:
    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 4u;
    bool autorepeatEnabled = false;
    std::bitset<nKeys> keystates;
    std::queue<Event> keybuffer;
    std::queue<char> charbuffer;
};
