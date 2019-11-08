#ifndef SLIDER_HPP_
#define SLIDER_HPP_

#include "VT100.hpp"
#include "box.hpp"
#include "number.hpp"


class Slider : public Box, public Number {
    uint16_t oldBarStatus = std::numeric_limits<short>::max(); // to note the change between new value and old value, used in refresh_value function

  public:

    /**
     * @brief This is constructor 
     * 
     * @param pointer to const char title 
     * @param pointer to const char unit, e.g. "mV", "A" 
     * @param bool statement describes possibility to edit the box
     * @param pointer to function which is used when value of the box is changed
     * @param divider for value, to display 1.234mV, value should be 1234 and divider 1000
     * @param minimum value, default 0
     * @param maximum value, default 100
     */
    Slider(const char *title, const char *unit, bool isEditable, void (*event_function)(), uint16_t divider = 1, int min = 0, int max = 100);

    /**
     * @brief This is default destructor
     */
    ~Slider() = default;

    /**
     * @brief This function is required by Box class, but nothing to do here
     */
    void calculate_width() override {}

    /**
     * @brief This function prints frame of the box on console, IN LOOP USE ONLY
     * 
     * @return
     */
    void refresh_frame() override ;

    /**
     * @brief This function prints value of the box on console, IN LOOP USE ONLY
     * 
     * @return
     */
    void refresh_value() override ;

    /**
     * @brief This function is used when user do an event on the box 
     * 
     * @param array of keys pushed by user
     * @param num of keys
     * @return
     */
    void keyboard_event(const uint8_t *buff, uint8_t numOfKeys) override ;
};

#endif // SLIDER_HPP_