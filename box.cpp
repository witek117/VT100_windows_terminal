#include "box.hpp"

Box::~Box() {}

void Box::init() {}

Box::Box(const char *title, bool isEditable) : title(title) { this->isEditable = isEditable; }

Box::Box(const char *title, bool isEditable, void (*event_function)(), uint8_t height) : title(title) {
    this->isEditable = isEditable;
    this->height = height;
    if (event_function) {
        this->event_function_pointer = event_function;
    }
}

void Box::init(int positionX, int positionY, int width) {
    this->width = width;
    init(positionX, positionY);
}

void Box::init(int positionX, int positionY) {
    this->positionX = positionX;
    this->positionY = positionY;
    init();
}

void Box::refresh() {
    refresh_frame();
    refresh_value(false);
}

void Box::set_active() {
    refreshFrameBool = true;
    isActive = true;
}

void Box::set_unactive(bool delPrt) {
    this->delPrt = delPrt;
    refreshFrameBool = true;
    isActive = false;
}

bool Box::in_loop() {
    if (refreshFrameBool) {
        refresh_frame();
        refreshFrameBool = false;
    }

    if (refreshValueBool) {
        event_function();
        refresh_value(false);
        refreshValueBool = false;
    }

    if (delPrt) {
        delPrt = false;
        return true;
    }
    return false;
}