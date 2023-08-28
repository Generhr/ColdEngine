#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char keycode) const {
    return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() {
    if (!keybuffer.empty()) {
        Keyboard::Event e = keybuffer.front();
        keybuffer.pop();

        return e;
    }

    return Keyboard::Event();
}

bool Keyboard::KeyIsEmpty() const {
    return keybuffer.empty();
}

char Keyboard::ReadChar() {
    if (!charbuffer.empty()) {
        unsigned char charcode = charbuffer.front();
        charbuffer.pop();

        return charcode;
    }

    return 0;
}

bool Keyboard::CharIsEmpty() const {
    return charbuffer.empty();
}

void Keyboard::FlushKey() {
    std::queue<Event> emptyQueue;
    std::swap(keybuffer, emptyQueue);
}

void Keyboard::FlushChar() {
    std::queue<char> emptyQueue;
    std::swap(charbuffer, emptyQueue);
}

void Keyboard::Flush() {
    FlushKey();
    FlushChar();
}

void Keyboard::EnableAutorepeat() {
    autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() {
    autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const {
    return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) {
    keystates[keycode] = true;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Press, keycode));
    TrimBuffer(keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) {
    keystates[keycode] = false;
    keybuffer.push(Keyboard::Event(Keyboard::Event::Release, keycode));
    TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character) {
    charbuffer.push(character);
    TrimBuffer(charbuffer);
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) {
    while (buffer.size() > bufferSize) {
        buffer.pop();
    }
}
