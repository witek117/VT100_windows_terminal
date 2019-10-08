#include "label.hpp"

Label::Label(const char *title, const char *unit, bool isEditable, void (*event_function)(), uint16_t divider, int min, int max)
    : Box(title, isEditable, event_function, 1), Number(unit, divider, min, max) {
    type = Box::TYPE::LABEL;
}

void Label::refresh_frame() {
    VT::save_cursor();

    if (isActive) {
        VT::set_colour(VT::COLOUR::GREEN);
    } else {
        VT::set_colour(VT::COLOUR::WHITE);
    }

    VT::move_to(positionX + 1, positionY);
    VT::print(title);

    VT::set_colour(VT::COLOUR::YELLOW);
    VT::move_to(positionX + width - 8, positionY);
    VT::print('<');
    VT::move_to(positionX + width, positionY);
    VT::print('>');

    VT::restore_cursor();
}

void Label::refresh_value() {
    if (prevValue == value)
        return;
    prevValue = value;
    VT::save_cursor();

    VT::set_colour(VT::COLOUR::YELLOW);
    VT::move_to(positionX + width - 6, positionY);
    printValue();

    VT::print(unit);
    VT::print(' ');

    VT::restore_cursor();
}

void Label::keyboard_event(const uint8_t *buff, uint8_t numOfKeys) {
    if (numOfKeys == 3) {
        if (buff[2] == 67) {
            increase();
        } else if (buff[2] == 68) {
            decrease();
        }
        refreshValueBool = true;
    }
}


