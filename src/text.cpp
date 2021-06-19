#include "text.hpp"

Text::Text(const char *title, bool isEditable, void (*event_function)())
        : Box(title, isEditable, event_function, 2) {
    type = Box::TYPE::TEXT;
}

size_t Text::getValueIndex() {
    return current_tab_index;
}

void Text::calculateWidth() {
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

void Text::addText(const char *text) { texts.push_back(text); }

void Text::init(int positionX, int positionY) {
    this->positionX = positionX;
    this->positionY = positionY;
}

void Text::refreshFrame() {
    VT::saveCursor();

    if (isActive) {
        VT::setColour(VT::COLOUR::GREEN);
    } else {
        VT::setColour(VT::COLOUR::WHITE);
    }

    VT::moveTo(positionX, positionY + 1);
    VT::print(title);
    VT::print(": ");
    VT::restoreCursor();
}

void Text::refreshValue(bool refreshHard = false) {
    if (current_tab_index == previous_tab_index && !refreshHard) {
        return;
    }
    VT::saveCursor();
    size_t title_len = strlen(title);
    VT::moveTo(positionX + title_len + 3, positionY + 1);
    VT::setColour(VT::COLOUR::YELLOW);
    VT::print(texts[current_tab_index]);
    VT::repeat(' ', max_text_length - strlen(texts[current_tab_index]));
    VT::restoreCursor();
    previous_tab_index = current_tab_index;
}

void Text::keyboardEvent(const uint8_t *buff, uint8_t numOfKeys) {
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