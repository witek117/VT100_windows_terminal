#include "VT100.hpp"

#include <cstdio>

void (*printCharFunction)(char);

int actualX;
int actualY;

namespace VT {
    void reset() {
        print(esc);
        print('c');
    }

    bool refresh() {
        if (printCharFunction) {
            reset();
            hideCursor();
            return true;
        }
        return false;
    }

    bool init(void (*printCharFunct)(char)) {
        if (printCharFunct) {
            printCharFunction = printCharFunct;
            refresh();
            return true;
        }
        return false;
    }

    void print(char c) {
        if (printCharFunction) {
            printCharFunction(c);
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

    void beginUnicode() { print("\x1b(0"); }

    void endUnicode() { print("\x1b(B"); }

    void setAttribute(ATTRIBUTE attr) {
        print(esc);
        print('[');
        print((int) attr);
        print('m');
    }

    void moveTo(int16_t x, int16_t y) {
        actualX = x;
        actualY = y;

        print(esc);
        print('[');
        print(actualY + 1);
        print(';');
        print(actualX + 1);
        print('H');
    }

    void hideCursor() {
        print(esc);
        print("[?25l");
    }

    void showCursor() {
        print(esc);
        print("[?25h");
    }

    void saveCursor() {
        print(esc);
        print('7');
    }

    void restoreCursor() {
        print(esc);
        print('8');
        setBackground(COLOUR::BLACK);
    }

    void del(int16_t len) {
        print(esc);
        print('[');
        print(len);
        print('P');
    }

    void del() { del(1); }

    void del(int16_t x, int16_t y) {
        moveTo(x, y);
        del(1);
    }

    void del(int16_t x, int16_t y, int16_t len) {
        moveTo(x, y);
        del(len);
    }

    void delLine() { print(deleteLine); }

    void setColour(COLOUR col) {
        print(esc);
        print('[');
        print((int) col);
        print('m');
    }

    void setBackground(COLOUR back) {
        print(esc);
        print('[');
        print((int) back + 10);
        print('m');
    }

    void clean() { print(erase); }

    void move(int x, int y) {
        if (y != 0) {
            print(esc);
            print('[');
            print((int) abs(y));
            if (y >= 0)
                print('B');
            else
                print('A');

            actualY += y;
        }

        if (x != 0) {
            print(esc);
            print('[');
            print((int) abs(x));
            if (x >= 0)
                print('C');
            else
                print('D');

            actualX += x;
        }
    }

    void repeat(char c, size_t num) {
        for (size_t i = 0; i < num; i++) {
            print(c);
        }
    }

    void repeat(UNICODE uni, size_t num) { repeat(static_cast<char>(uni), num); }

    void disable() {
        printCharFunction = nullptr;
    }

}; // namespace VT