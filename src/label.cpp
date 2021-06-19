#include "label.hpp"

Label::Label(const char *title, const char *unit, bool isEditable, void (*eventFunction)(), uint16_t divider, int min,
             int max)
        : Box(title, isEditable, eventFunction, 1), Number(unit, divider, min, max) {
    type = Box::TYPE::LABEL;
}

void Label::refreshFrame() {
    VT::saveCursor();

    if (isActive) {
        VT::setColour(VT::COLOUR::GREEN);
    } else {
        VT::setColour(VT::COLOUR::WHITE);
    }

    VT::moveTo(positionX + 1, positionY);
    VT::print(title);

    VT::setColour(VT::COLOUR::YELLOW);
    VT::moveTo(positionX + width - 8, positionY);
    VT::print('<');
    VT::moveTo(positionX + width, positionY);
    VT::print('>');

    VT::restoreCursor();
}

void Label::refreshValue() {
    if (prevValue == value)
        return;
    prevValue = value;
    VT::saveCursor();

    VT::setColour(VT::COLOUR::YELLOW);
    VT::moveTo(positionX + width - 6, positionY);
    printValue();

    VT::print(unit);
    VT::print(' ');

    VT::restoreCursor();
}

void Label::keyboardEvent(const uint8_t *buff, uint8_t numOfKeys) {
    if (numOfKeys == 3) {
        if (buff[2] == 67) {
            increase();
        } else if (buff[2] == 68) {
            decrease();
        }
        refreshValueBool = true;
    }
}


