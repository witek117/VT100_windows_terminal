#include "Window.hpp"

Window::Window(const Window &win)
    : title(win.title), positionX(win.positionX), positionY(win.positionY), width(win.width), height(win.height), isEditable(win.isEditable),
        boxes(win.boxes) {}

Window::Window(const char *title, int positionX, int positionY, int width, int height, bool isEditlable)
    : title(title), positionX(positionX), positionY(positionY), width(width), height(height) {
    this->isEditable = isEditlable;
}

void Window::init() {
    titleLen = strlen(title);

    if (boxes.empty()) {
        return;
    }

    bool text_box_found = false;

    int16_t maxLenLabel = 0;

    for (uint8_t i = 0; i < boxes.size(); i++) {
        if (boxes[i]->type == Box::TYPE::TEXT) {
            text_box_found = true;
            boxes[i]->calculate_width();
            if (boxes[i]->get_width() > maxLenLabel) {
                maxLenLabel = boxes[i]->get_width();
            }
        }
    }

    maxLenLabel += 5;
    uint8_t colNum = width / maxLenLabel;
    uint16_t posX = 2, posY = 1;

    for (uint8_t i = 0; i < boxes.size(); i++) {
        if (boxes[i]->type == Box::TYPE::TEXT) {
            boxes[i]->init(positionX + posX, positionY + posY);
            boxes[i]->refresh();

            if (((i + 1) % colNum) == 0) {
                posX = 2;
                posY++;
            } else {
                posX += maxLenLabel;
            }
        }
    }

    VT::save_cursor();
    VT::set_colour(VT::COLOUR::BLUE);

    if (text_box_found) {
        posY += 2;
        VT::move_to(positionX + 1, positionY + posY);
        VT::repeat('-', width - 1);
    }

    for (uint8_t i = 0; i < boxes.size(); i++) {
        if (boxes[i]->type != Box::TYPE::TEXT) {
            boxes[i]->init(positionX + 1, positionY + posY + 1, width - 4);

            posY += boxes[i]->get_height() + 1;

            VT::move_to(positionX + 1, positionY + posY);

            VT::repeat('-', boxes[i]->get_width() + 3);
        }
    }

    VT::restore_cursor();
}

void Window::print_frame(VT::COLOUR frameColour) {
    VT::save_cursor();
    VT::set_colour(frameColour);
    VT::set_attritute(VT::ATTRIBUTE::BRIGHT);

    VT::move_to(positionX, positionY);
    VT::begin_unicode();
    VT::print(VT::UNICODE::LU_CORNER);        // left up corner
    VT::repeat(VT::UNICODE::DASH, width - 1); // bar
    VT::print(VT::UNICODE::RU_CORNER);        // right up corner

    for (int i = 1; i < height; i++) {
        VT::move_to(positionX, positionY + i);
        VT::print(VT::UNICODE::PIPE);
    }

    VT::move_to(positionX, positionY + height);
    VT::print(VT::UNICODE::LD_CORNER);        // left down corner
    VT::repeat(VT::UNICODE::DASH, width - 1); // up bar
    VT::print(VT::UNICODE::RD_CORNER);        // right down corner

    for (int i = 1; i < height; i++) {
        VT::move_to(positionX + width, positionY + i);
        VT::print(VT::UNICODE::PIPE);
    }
    VT::end_unicode();
    VT::move_to(positionX + (width / 2) - (titleLen / 2) - 1, positionY);
    VT::print(' ');
    VT::set_colour(VT::COLOUR::WHITE);
    VT::print(title);
    VT::print(' ');
    VT::restore_cursor();
}

void Window::refresh_value(bool refresh_hard = false) {
    for (uint8_t i = 0; i < boxes.size(); i++) {
        boxes[i]->refresh_value(refresh_hard);
    }
}

void Window::refresh() {
    refresh_frame();

    for (uint8_t i = 0; i < boxes.size(); i++) {
        boxes[i]->refresh();
    }
}

void Window::refresh_frame() {
    VT::save_cursor();
    if (isActive) {
        print_frame(VT::COLOUR::GREEN);
    } else {
        print_frame(VT::COLOUR::BLUE);
    }
    VT::restore_cursor();
}

void Window::add_box(Box *box) { boxes.push_back(box); }

bool Window::in_loop() {
    if (refreshFrameBool == true) {
        refresh_frame();
        refreshFrameBool = false;
    }

    if (delPrt) {
        delPrt = false;
        return true;
    }
    return false;
}

void Window::set_active() {
    isActive = true;
    refreshFrameBool = true;
}

void Window::set_unactive(bool delPrt_) {
    delPrt = delPrt_;
    isActive = false;
    refreshFrameBool = true;
}
