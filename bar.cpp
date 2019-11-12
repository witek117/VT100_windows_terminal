#include "bar.hpp"

void Bar::init() {
    yellowStart = width / 2;
    redStart = width * 3;
    redStart /= 4;
}

Bar::Bar(const char *title, const char *unit, bool isEditable, void (*event_function)(), uint16_t divider, int min , int max )
    : Box(title, isEditable, event_function, 2), Number(unit, divider, min, max) {
    type = Box::TYPE::BAR;
}

void Bar::refresh_frame() {
    VT::save_cursor();

    if (isActive) {
        VT::set_colour(VT::COLOUR::GREEN);
    } else {
        VT::set_colour(VT::COLOUR::WHITE);
    }

    VT::move_to(positionX + 1, positionY);

    VT::print(title);

    VT::move_to(positionX + 1, positionY + 1);
    VT::print('<');
    VT::move_to(positionX + 1 + width, positionY + 1);
    VT::print('>');

    VT::restore_cursor();
}

void Bar::refresh_value(bool refresh_hard = false) {
    if (prevValue == value && !refresh_hard)
        return;
    prevValue = value;

    VT::save_cursor();

    decltype(oldBarStatus) newBarStatus = static_cast<decltype(oldBarStatus)>(map(value, minValue, maxValue, 0, width - 2));

    if (newBarStatus > yellowStart && newBarStatus < redStart) {
        VT::set_colour(VT::COLOUR::YELLOW);
    } else if (newBarStatus >= redStart) {
        VT::set_colour(VT::COLOUR::RED);
    } else {
        VT::set_colour(VT::COLOUR::GREEN);
    }
    VT::move_to(positionX + width - 8, positionY); // to przydałoby się kolorować w zależności od wartości
    VT::print(value);
    VT::print(unit);

    VT::print("  ");

    if (newBarStatus == oldBarStatus) {
        VT::restore_cursor();
        return;
    }

    VT::COLOUR temColour = VT::COLOUR::GREEN;
    int diff = newBarStatus - oldBarStatus;
    if (diff > 0) {
        for (decltype(oldBarStatus) i = oldBarStatus; i <= newBarStatus; i++) {
            if (i > yellowStart && i < redStart) {
                temColour = VT::COLOUR::YELLOW;
            } else if (i >= redStart) {
                temColour = VT::COLOUR::RED;
            }
                
            VT::move_to(positionX + i + 2, positionY + 1);
            VT::set_background(temColour);
            VT::print(' ');
        }
    } else {
        VT::print(' ');
        VT::print(' ');
        decltype(oldBarStatus) i = newBarStatus + 1;
        VT::move_to(positionX + i + 2, positionY + 1);
        for (; i <= oldBarStatus; i++) {
            VT::print(' ');
        }
    }
    VT::restore_cursor();
    oldBarStatus = newBarStatus;
}

void Bar::keyboard_event(const uint8_t *buff, uint8_t numOfKeys) {
    if (numOfKeys == 3) {
        if (buff[2] == 67) {
            increase();
        } else if (buff[2] == 68) {
            decrease();
        }
        refreshValueBool = true;
    }
}