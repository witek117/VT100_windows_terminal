#include "VT100.hpp"

#include <utility>

std::function<void(char)> print_char_function = nullptr;

int actualX;
int actualY;

namespace VT {
bool refresh() {
    if (print_char_function) {
        reset();
        hide_cursor();
        return true;
    }
    return false;
}

bool init(std::function<void(char)> handler) {
    if (print_char_function == nullptr) {
        print_char_function = std::move(handler);
        reset();
        hide_cursor();
        return true;
    }
    return false;
}

void reset(void) {
    print(esc);
    print('c');
}

void print(char c) {
    if (print_char_function) {
        print_char_function(c);
    }
}

size_t print(const char *s) {
    size_t size = 0;
    char c;
    while ((c = *s) != '\0') {
        print(c);
        s++;
        size++;
    }
    return size;
}

void print(int num) {
    char buff[11];
    sprintf(buff, "%d", num);
    print(buff);
}

void print(UNICODE uni) { print(static_cast<char>(uni)); }

void begin_unicode() { print("\x1b(0"); }

void end_unicode() { print("\x1b(B"); }

void set_attritute(ATTRIBUTE attr) {
    print(esc);
    print('[');
    print((int)attr);
    print('m');
}

void move_to(int16_t x, int16_t y) {
    actualX = x;
    actualY = y;

    print(esc);
    print('[');
    print(actualY + 1);
    print(';');
    print(actualX + 1);
    print('H');
}

void hide_cursor() {
    print(esc);
    print("[?25l");
}

void show_cursor() {
    print(esc);
    print("[?25h");
}

void save_cursor() {
    print(esc);
    print('7');
}

void restore_cursor() {
    print(esc);
    print('8');
}

void del(int16_t len) {
    print(esc);
    print('[');
    print(len);
    print('P');
}

void del() { del(1); }

void del(int16_t x, int16_t y) {
    move_to(x, y);
    del(1);
}

void del(int16_t x, int16_t y, int16_t len) {
    move_to(x, y);
    del(len);
}

void delLine() { print(deleteLine); }

void set_colour(COLOUR col) {
    print(esc);
    print('[');
    print((int)col);
    print('m');
}

void set_background(COLOUR back) {
    print(esc);
    print('[');
    print((int)back + 10);
    print('m');
}

void clean() { print(erase); }

void move(int x, int y) {
    if (y != 0) {
        print(esc);
        print('[');
        print((int)abs(y));
        if (y >= 0)
            print('B');
        else
            print('A');

        actualY += y;
    }

    if (x != 0) {
        print(esc);
        print('[');
        print((int)abs(x));
        if (x >= 0)
            print('C');
        else
            print('D');

        actualX += x;
    }
}

void repeat(char c, size_t num) {
    for (size_t i = 0; i < num; i++){
        print(c);
    } 
}

void repeat(UNICODE uni, size_t num) { repeat(static_cast<char>(uni), num); }

void disable() {
    print_char_function = nullptr;
}

}; // namespace VT