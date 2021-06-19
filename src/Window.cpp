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

    for (auto & box : boxes) {
        if (box->type == Box::TYPE::TEXT) {
            text_box_found = true;
            box->calculateWidth();
            if (box->getWidth() > maxLenLabel) {
                maxLenLabel = box->getWidth();
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

    VT::saveCursor();
    VT::setColour(VT::COLOUR::BLUE);

    if (text_box_found) {
        posY += 2;
        VT::moveTo(positionX + 1, positionY + posY);
        VT::repeat('-', width - 1);
    }

    for (auto & box : boxes) {
        if (box->type != Box::TYPE::TEXT) {
            box->init(positionX + 1, positionY + posY + 1, width - 4);

            posY += box->getHeight() + 1;

            VT::moveTo(positionX + 1, positionY + posY);

            VT::repeat('-', box->getWidth() + 3);
        }
    }

    VT::restoreCursor();
}

void Window::printFrame(VT::COLOUR frameColor) {
    VT::saveCursor();
    VT::setColour(frameColor);
    VT::setAttribute(VT::ATTRIBUTE::BRIGHT);

    VT::moveTo(positionX, positionY);
    VT::beginUnicode();
    VT::print(VT::UNICODE::LU_CORNER);        // left up corner
    VT::repeat(VT::UNICODE::DASH, width - 1); // bar
    VT::print(VT::UNICODE::RU_CORNER);        // right up corner

    for (int i = 1; i < height; i++) {
        VT::moveTo(positionX, positionY + i);
        VT::print(VT::UNICODE::PIPE);
    }

    VT::moveTo(positionX, positionY + height);
    VT::print(VT::UNICODE::LD_CORNER);        // left down corner
    VT::repeat(VT::UNICODE::DASH, width - 1); // up bar
    VT::print(VT::UNICODE::RD_CORNER);        // right down corner

    for (int i = 1; i < height; i++) {
        VT::moveTo(positionX + width, positionY + i);
        VT::print(VT::UNICODE::PIPE);
    }
    VT::endUnicode();
    VT::moveTo(positionX + (width / 2) - (titleLen / 2) - 1, positionY);
    VT::print(' ');
    VT::setColour(VT::COLOUR::WHITE);
    VT::print(title);
    VT::print(' ');
    VT::restoreCursor();
}

void Window::refreshValue(bool refreshHard = false) {
    for (auto & box : boxes) {
        box->refreshValue(refreshHard);
    }
}

void Window::refresh() {
    refreshFrame();

    for (auto & box : boxes) {
        box->refresh();
    }
}

void Window::refreshFrame() {
    VT::saveCursor();
    if (isActive) {
        printFrame(VT::COLOUR::GREEN);
    } else {
        printFrame(VT::COLOUR::BLUE);
    }
    VT::restoreCursor();
}

void Window::addBox(Box *box) { boxes.push_back(box); }

bool Window::inLoop() {
    if (refreshFrameBool) {
        refreshFrame();
        refreshFrameBool = false;
    }

    if (delPrt) {
        delPrt = false;
        return true;
    }
    return false;
}

void Window::setActive() {
    isActive = true;
    refreshFrameBool = true;
}

void Window::setNonActive(bool delPrt_) {
    delPrt = delPrt_;
    isActive = false;
    refreshFrameBool = true;
}
