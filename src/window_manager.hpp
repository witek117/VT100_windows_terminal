#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include "Window.hpp"
#include "box.hpp"
#include "cyclicBuffer.hpp"

template<int windowsMaxCount>
class window_manager {
public:
    /**
     * @brief This enum class contains special characters using in window_manager
     */
    enum class Keyboard_action {
        TAB = 9, QUIT = 'q', ARROW_UP = 65, ARROW_DOWN = 66, ARROW_LEFT = 68, ARROW_RIGHT = 67
    };

private:
    Window *activeWindow = nullptr;
    Window *lastActiveWindow = nullptr;

    Box *activeBox = nullptr;
    Box *lastActiveBox = nullptr;

    Window *windows[windowsMaxCount] = {nullptr};
    uint8_t actualWindowsCount = 0;

    uint8_t index = 0;
    uint8_t recBuff[10] = {0};

    constexpr static size_t buff_size = 20;
    CyclicBuffer_data<uint8_t, buff_size> uart_buffer;

    void initAll() {
        for (uint8_t i = 0; i < actualWindowsCount; i++) {
            windows[i]->init();
        }
    }

    void readFromCyclicBuffer() {
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

        if (active_window_index == 255 && actualWindowsCount > 0) {
            for (size_t i = 0; i < actualWindowsCount; i++) {
                if (windows[i]->getEditable()) {
                    active_window_index = i;
                    activeWindow = windows[active_window_index];
                    activeWindow->setActive();
                    return;
                }
            }
        }

        size_t index = 0;
        if (actualWindowsCount > 0) {
            for (size_t i = 0; i < actualWindowsCount; i++) {

                index = (i + active_window_index + 1) % actualWindowsCount;

                if (windows[index]->getEditable()) {
                    lastActiveWindow = windows[active_window_index];
                    lastActiveWindow->setNonActive();
                    active_window_index = index;
                    activeWindow = windows[active_window_index];
                    activeWindow->setActive();
                    return;
                }
            }
        }
        activeWindow = nullptr;
    }

    void change_active_box(Keyboard_action action) {
        size_t bsize = activeWindow->getBoxesCont();
        for (size_t j = 0; j < bsize; j++) {
            if (activeWindow->getActiveBox(j)) { // tu ma być chyba editable

                if (action == Keyboard_action::ARROW_DOWN && j < (bsize - 1)) {
                    j++;
                }

                if (action == Keyboard_action::ARROW_UP && j > 0) {
                    j--;
                }

                activeBox = activeWindow->getBoxPointer(j);
                break;
            }
        }

        if (activeBox == nullptr && bsize > 0) {
            activeBox = activeWindow->getBoxPointer(0);
        }
    }

public:
    window_manager() {
        index = 0;
    }

    /**
     * @brief This function initialize window_manager
     *
     * @return
     */
    void init() {
        initAll();
    }

    void add(Window *window) {
        if (actualWindowsCount < windowsMaxCount) {
            windows[actualWindowsCount] = window;
            actualWindowsCount++;
        }
    }

    /**
     * @brief This function can be can be used in rx callback from USART interrupt
     *
     * @param received character
     * @return
     */
    void in_RX_callback(uint8_t received_char) {
        uart_buffer.append(received_char);
    }

    /**
     * @brief This function can be ONLY IN LOOP, printing all data to VT100
     *
     * @return
     */
    void run() {
        readFromCyclicBuffer();
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

    /**
     * @brief This function refreshes all data in all windows
     *
     * @return
     */
    void refreshAll() {
        for (uint8_t i = 0; i < actualWindowsCount; i++) {
            windows[i]->refresh();
        }
    }

    void refresh_value(bool refreshHard = false) {
        for (uint8_t i = 0; i < actualWindowsCount; i++) {
            windows[i]->refreshValue(refreshHard);
        }
    }

}; // namespace window_manager

#endif