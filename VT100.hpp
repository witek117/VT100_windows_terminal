#ifndef VT100_HPP_
#define VT100_HPP_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

namespace VT {

constexpr const char *esc = "\033";
constexpr const char *erase = "\033[2J";
constexpr const char *deleteLine = "\033[M";
constexpr const char *attrSet = "\033[0";

enum class COLOUR { BLACK = 30, RED = 31, GREEN = 32, YELLOW = 33, BLUE = 34, MAGENTA = 35, CYAN = 36, WHITE = 37 };
enum class ATTRIBUTE { RESET = 0, BRIGHT = 1, DIM = 2, UNDERSCORE = 4, BLINK = 5, REVERSE = 7, HIDDEN = 8 };
enum class UNICODE {
    RD_CORNER = 0x6A,
    RU_CORNER = 0x6B,
    LU_CORNER = 0x6C,
    LD_CORNER = 0x6D,
    CROSS = 0x6E,
    DASH = 0x71,
    DASH_UP = 0x76,
    DASH_DOWN = 0x77,
    PIPE = 0x78,
    PIPE_R = 0x74,
    PIPE_L = 0x75
};
/**
 * @brief This function set pointer to print char function and initialize VT100 terminal
 *
 * @return
 */	
void init(void (*print_char_function_)(char));

/**
 * @brief This function reset terminal to default
 *
 * @return
 */
void reset();

/**
 * @brief This function is basic print char function
 *
 * @param 
 * @return
 */	
void print(const char c);

/**
 * @brief This function print string to console
 *
 * @param pointer to const char string
 * @return
 */	
size_t print(const char *s);

/**
 * @brief This function print unicodes from enum class
 *
 * @param specyfic unicode
 * @return
 */	
void print(UNICODE uni);

/**
 * @brief This function print integer value to console
 *
 * @param integer value
 * @return
 */	
void print(int num);

/**
 * @brief This function must be used before print with unicode
 *
 * @return
 */	
void begin_unicode();

/**
 * @brief This function must be used after print with unicode
 *
 * @return
 */	
void end_unicode();

/**
 * @brief This function set attribute to printed text
 *
 * @param attribute form ATTRIBUTE enum class
 * @return
 */	
void set_attritute(ATTRIBUTE attr);

/**
 * @brief This function move coursor to specific location in console
 *
 * @param location x
 * @param location y
 * @return
 */	
void move_to(int16_t x, int16_t y);

/**
 * @brief This function hide coursor on console
 *
 * @return
 */
void hide_cursor();

/**
 * @brief This function show coursor on console
 *
 * @return
 */
void show_cursor();

/**
 * @brief This function save all values of coursor, position, colour, attributes
 *
 * @return
 */
void save_cursor();

/**
 * @brief This function restore all values of coursor, position, colour, attributes, if save_cursor function was used before
 *
 * @return
 */
void restore_cursor();

/**
 * @brief This function clean terminal
 *
 * @return
 */
void clean();

/**
 * @brief This function set text colour
 *
 * @param colour of text
 * @return
 */
void set_colour(COLOUR col);

/**
 * @brief This function set background colour
 *
 * @param colour of backgorund
 * @return
 */
void set_background(COLOUR back);

/**
 * @brief This function repeat character
 *
 * @param character to repeat
 * @param number of repeats
 * @return
 */
void repeat(char c, size_t num);

/**
 * @brief This function repeat unicode
 *
 * @param unicode to repeat
 * @param number of repeats
 * @return
 */
void repeat(UNICODE uni, size_t num);

/**
 * @brief This function disables VT monitor
 *
 * @return
 */
void disable();

// not tested yet
void del(int16_t len);
void del();
void del(int16_t x, int16_t y);
void del(int16_t x, int16_t y, int16_t len);
void del_line();



}; // namespace VT

#endif // VT100_HPP
