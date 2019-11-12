#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include "Window.hpp"
#include "box.hpp"
#include "cyclicBuffer.hpp"

namespace window_manager {

/**
 * @brief This enum class contains special characters using in window_manager
 */
enum class Keyboard_action { TAB = 9, QUIT = 'q', ARROW_UP = 65, ARROW_DOWN = 66, ARROW_LEFT = 68, ARROW_RIGHT = 67 };

void init_all();
/**
 * @brief This function initialize window_manager
 * 
 * @return
 */
void init(std::vector<Window> *wins);

/**
 * @brief This function can be can be used in rx callback from USART interrupt
 * 
 * @param received character
 * @return
 */
void in_RX_callback(uint8_t received_char);

/**
 * @brief This function can be ONLY IN LOOP, printing all data to VT100
 * 
 * @return
 */
void run();

/**
 * @brief This function refreshes all data in all windows
 * 
 * @return
 */
void refresh_all();

void refresh_value(bool refresh_hard);
} // namespace window_manager

#endif