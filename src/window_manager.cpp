#include "window_manager.hpp"

namespace window_manager {
constexpr size_t buff_size = 20;
CyclicBuffer_data<uint8_t, buff_size> uart_buffer;

void change_active_window();

void change_active_box(Keyboard_action action);

void refresh_value();

Window *activeWindow = nullptr;
Window *lastActiveWindow = nullptr;

Box *activeBox = nullptr;
Box *lastActiveBox = nullptr;

uint8_t index = 0;
uint8_t recBuff[10];
// std::array<uint8_t, 10> recBuff;

std::vector<Window> *windows;

void refreshAll() {
    for (auto & window : *windows) {
        window.refresh();
    }
}

void init_all() {
    for (auto & window : *windows) {
        window.init();
    }
}

void refresh_value() {
    for (auto & window : *windows) {
        window.refreshValue();
    }
}

void init(std::vector<Window> *wins) {
    windows = wins;
    init_all();
}

void in_RX_callback(uint8_t received_char) { uart_buffer.append(received_char); }

void read_from_cyclic_Buffer() {
    if (uart_buffer.isNotEmpty()) {

        char received_char = uart_buffer.get_unsafe();
        recBuff[index] = received_char;

        if (received_char == 27 && index == 0) {
            index++;
        } else if (index == 1) {
            index++;
        } else if (index == 2) {
            index++;
            if (activeWindow) {
                if (received_char == static_cast<uint8_t>(Keyboard_action::ARROW_UP) ||
                    received_char == static_cast<uint8_t>(Keyboard_action::ARROW_DOWN)) {

                    lastActiveBox = activeBox;

                    change_active_box(static_cast<Keyboard_action>(received_char));

                    if (lastActiveBox) {
                        lastActiveBox->setNotActive();
                    }

                    if (activeBox) {
                        activeBox->setActive();
                    }
                } else if (activeBox) {
                    activeBox->keyboardEvent(recBuff, index);
                }
            }

            index = 0;

        } else {
            if (received_char == static_cast<uint8_t>(Keyboard_action::TAB)) {
                if (activeBox) {
                    activeBox->setNotActive(true); // tu chyba było bez true
                }
                change_active_window();
            } else if (received_char == static_cast<uint8_t>(Keyboard_action::QUIT)) {
                if (activeBox) {
                    activeBox->setNotActive(true);
                } else {
                    if (activeWindow) {
                        activeWindow->setNonActive(true);
                    }
                }
            } else {
                if (activeBox) {
                    activeBox->keyboardEvent(recBuff, 1);
                }
            }
            index = 0;
        }
    }
}

void change_active_window() {
    static size_t active_window_index = 255;
    size_t windows_size = windows->size();

    if (active_window_index == 255 && windows_size > 0) {
        for (size_t i = 0; i < windows_size; i++) {
            if (windows->at(i).getEditable()) {
                active_window_index = i;
                activeWindow = &(windows->at(active_window_index));
                activeWindow->setActive();
                return;
            }
        }
    }

    size_t index = 0;
    if (windows_size > 0) {
        for (size_t i = 0; i < windows_size; i++) {

            index = (i + active_window_index + 1) % windows_size;

            if (windows->at(index).getEditable()) {
                lastActiveWindow = &(windows->at(active_window_index));
                lastActiveWindow->setNonActive();
                active_window_index = index;
                activeWindow = &(windows->at(active_window_index));
                activeWindow->setActive();
                return;
            }
        }
    }
    activeWindow = nullptr;
}

void change_active_box(Keyboard_action action) {
    size_t bsize = activeWindow->boxes.size();
    for (size_t j = 0; j < bsize; j++) {
        if (activeWindow->boxes[j]->getActive()) { // tu ma być chyba editable

            if (action == Keyboard_action::ARROW_DOWN && j < (bsize - 1)) {
                j++;
            }

            if (action == Keyboard_action::ARROW_UP && j > 0) {
                j--;
            }

            activeBox = activeWindow->boxes[j];
            break;
        }
    }

    if (activeBox == nullptr && bsize > 0) {
        activeBox = activeWindow->boxes[0];
    }
}

void run() {
    read_from_cyclic_Buffer();
    if (activeWindow) {
        if (activeWindow->inLoop()) {
            activeWindow = nullptr;
        }
    }

    if (lastActiveWindow) {
        lastActiveWindow->inLoop();
        lastActiveWindow = nullptr;
    }

    if (activeBox) {
        if (activeBox->inLoop()) {
            activeBox = nullptr;
        }
    }

    if (lastActiveBox) {
        lastActiveBox->inLoop();
        lastActiveBox = nullptr;
    }

    refresh_value();
}

} // namespace window_manager