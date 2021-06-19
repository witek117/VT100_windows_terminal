#include "box.hpp"

Box::~Box() {}

void Box::init() {}

Box::Box(const char *title, bool isEditable) : title(title) { this->isEditable = isEditable; }

Box::Box(const char *title, bool isEditable, void (*eventFunction)(), uint8_t height) : title(title) {
    this->isEditable = isEditable;
    this->height = height;
    if (eventFunction) {
        this->eventFunctionPointer = eventFunction;
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
    refreshFrame();
    refreshValue();
}

void Box::setActive() {
    refreshFrameBool = true;
    isActive = true;
}

void Box::setNotActive(bool delPrt) {
    this->delPrt = delPrt;
    refreshFrameBool = true;
    isActive = false;
}

bool Box::inLoop() {
    if (refreshFrameBool) {
        refreshFrame();
        refreshFrameBool = false;
    }

    if (refreshValueBool) {
        eventFunction();
        refreshValue();
        refreshValueBool = false;
    }

    if (delPrt) {
        delPrt = false;
        return true;
    }
    return false;
}