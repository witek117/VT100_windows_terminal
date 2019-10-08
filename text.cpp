#include "text.hpp"

Text::Text(const char *title, bool isEditable, void (*event_function)())
    : Box(title, isEditable, event_function, 2){
    type = Box::TYPE::TEXT;
}

size_t Text::get_value_index() {
    return current_tab_index;
}

void Text::calculate_width() {
    width += strlen(title);

    size_t length = 0;
    for (size_t i = 0; i < texts.size(); i++) {
        length = strlen(texts[i]);
        if (length > max_text_length) {
            max_text_length = length;
        }
    }
    width += max_text_length;
}

void Text::add_text(const char *text) { texts.push_back(text); }

void Text::init(int positionX, int positionY) {
    this->positionX = positionX;
    this->positionY = positionY;
}

void Text::refresh_frame() {
    VT::save_cursor();

    if (isActive) {
        VT::set_colour(VT::COLOUR::GREEN);
    } else {
        VT::set_colour(VT::COLOUR::WHITE);
    }

    VT::move_to(positionX, positionY + 1);
    VT::print(title);
    VT::print(": ");
    VT::restore_cursor();
}

void Text::refresh_value() {
    if (current_tab_index == previous_tab_index) {
        return;
    }
    VT::save_cursor();
    size_t title_len = strlen(title);
    VT::move_to(positionX + title_len + 3, positionY + 1);
    VT::set_colour(VT::COLOUR::YELLOW);
    VT::print(texts[current_tab_index]);
    VT::repeat(' ', max_text_length - strlen(texts[current_tab_index]));
    VT::restore_cursor();
    previous_tab_index = current_tab_index;
}

void Text::keyboard_event(const uint8_t *buff, uint8_t numOfKeys) {
    if (numOfKeys == 3) {
        if (buff[2] == 67) {
            increase();
        } else if (buff[2] == 68) {
            decrease();
        }
        refreshValueBool = true;
    }
}

void Text::increase() {
    if (current_tab_index < (texts.size() - 1)) {
        current_tab_index++;
    }
}

void Text::decrease() {
    if (current_tab_index > 0) {
        current_tab_index--;
    }
}