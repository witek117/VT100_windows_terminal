#include "bar.hpp"

void Bar::init() {
    yellowStart = width / 2;
    redStart = width * 3;
    redStart /= 4;
}

Bar::Bar(const char *title, const char *unit, bool isEditable, void (*event_function)(), uint16_t divider, int min,
         int max)
        : Box(title, isEditable, event_function, 2), Number(unit, divider, min, max) {
    type = Box::TYPE::BAR;
}

void Bar::refreshFrame() {
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

void Bar::refreshValue(bool refreshHard = false) {
    if (prevValue == value && !refreshHard) {
        return;
    }
    prevValue = value;
    VT::saveCursor();

    decltype(oldBarStatus) newBarStatus = static_cast<decltype(oldBarStatus)>(map(value, minValue, maxValue, 0,
                                                                                  width - 2));

    if (newBarStatus > yellowStart && newBarStatus < redStart) {
        VT::setColour(VT::COLOUR::YELLOW);
    } else if (newBarStatus >= redStart) {
        VT::setColour(VT::COLOUR::RED);
    } else {
        VT::setColour(VT::COLOUR::GREEN);
    }

    VT::moveTo(positionX + width - 8, positionY); // to przydałoby się kolorować w zależności od wartości
    printValue();
    VT::print(unit);
    VT::print("  ");

    if (newBarStatus == oldBarStatus) {
        VT::restoreCursor();
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

            VT::moveTo(positionX + i + 2, positionY + 1);
            VT::setBackground(temColour);
            VT::print(' ');
        }
    } else {
        VT::print(' ');
        VT::print(' ');
        decltype(oldBarStatus) i = newBarStatus + 1;
        VT::moveTo(positionX + i + 2, positionY + 1);
        for (; i <= oldBarStatus; i++) {
            VT::print(' ');
        }
    }
    VT::restoreCursor();
    oldBarStatus = newBarStatus;
}

void Bar::keyboardEvent(const uint8_t *buff, uint8_t numOfKeys) {
    if (numOfKeys == 3) {
        if (buff[2] == 67) {
            increase();
        } else if (buff[2] == 68) {
            decrease();
        }
        refreshValueBool = true;
    }
}