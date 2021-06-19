#include "slider.hpp"

Slider::Slider(const char *title, const char *unit, bool isEditable, void (*eventFunction)(), uint16_t divider, int min,
               int max)
        : Box(title, isEditable, eventFunction, 2), Number(unit, divider, min, max) {
    type = Box::TYPE::SLIDER;
}

void Slider::refreshFrame() {
    VT::saveCursor();

    if (isActive) {
        VT::setColour(VT::COLOUR::GREEN);
    } else {
        VT::setColour(VT::COLOUR::WHITE);
    }

    VT::moveTo(positionX + 1, positionY);
    VT::print(title);

    VT::moveTo(positionX + 1, positionY + 1);
    VT::print('<');
    VT::moveTo(positionX + 1 + width, positionY + 1);
    VT::print('>');

    VT::restoreCursor();
}

void Slider::refreshValue(bool refreshHard = false) {
    if (value == prevValue && !refreshHard) {
        return;
    }
    prevValue = value;
    VT::saveCursor();

    VT::setColour(VT::COLOUR::YELLOW);
    VT::moveTo(positionX + width - 8, positionY);
    printValue();
    VT::print(unit);
    VT::print("  ");

    decltype(oldBarStatus) newBarStatus = map(value, minValue, maxValue, 0, width - 2);

    if (newBarStatus == oldBarStatus) {
        VT::restoreCursor();
        return;
    } else {

        VT::setBackground(VT::COLOUR::GREEN);
        VT::moveTo(positionX + 2 + newBarStatus, positionY + 1);
        VT::print(' ');
        VT::setBackground(VT::COLOUR::BLACK);

        if (oldBarStatus != std::numeric_limits<short>::max()) {
            VT::moveTo(positionX + 2 + oldBarStatus, positionY + 1);
            VT::print(' ');
        }

        oldBarStatus = newBarStatus;
        VT::restoreCursor();
    }
}

void Slider::keyboardEvent(const uint8_t *buff, uint8_t numOfKeys) {
    if (numOfKeys == 3) {
        if (buff[2] == 67) {
            increase();
        } else if (buff[2] == 68) {
            decrease();
        }
        refreshValueBool = true;
    }
}