#include "slider.hpp"

Slider::Slider(const char *title, const char *unit, bool isEditable, void (*event_function)(), uint16_t divider, int min, int max)
    : Box(title, isEditable, event_function, 2), Number(unit, divider, min, max) {
    type = Box::TYPE::SLIDER;
}

void Slider::refresh_frame()  {
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

void Slider::refresh_value()  {
    if (value == prevValue) {
        return;
    }

    VT::save_cursor();

    VT::set_colour(VT::COLOUR::YELLOW);
    VT::move_to(positionX + width - 8, positionY);
    VT::print(value);
    VT::print(unit);
    VT::print("  ");

    decltype(oldBarStatus) newBarStatus = map(value, minValue, maxValue, 0, width - 2);
    prevValue = value;
    if (newBarStatus == oldBarStatus) {
        VT::restore_cursor();
        return;
    } else {

        VT::set_background(VT::COLOUR::GREEN);
        VT::move_to(positionX + 2 + newBarStatus, positionY + 1);
        VT::print(' ');
        VT::set_background(VT::COLOUR::BLACK);
        // VT::set_colour(VT::COLOUR::BLACK);

        //
        if (oldBarStatus != std::numeric_limits<short>::max()) {
            VT::move_to(positionX + 2 + oldBarStatus, positionY + 1);
            VT::print(' ');
        }

        oldBarStatus = newBarStatus;
        VT::restore_cursor();
    }
}

void Slider::keyboard_event(const uint8_t *buff, uint8_t numOfKeys)  {
    if (numOfKeys == 3) {
        if (buff[2] == 67) {
            increase();
        } else if (buff[2] == 68) {
            decrease();
        }
        refreshValueBool = true;
    }
}