#ifndef BAR_HPP_
#define BAR_HPP_

#include "VT100.hpp"
#include "box.hpp"
#include "number.hpp"

/**
 * @brief This class describes Bar class
 */
class Bar : public Box, public Number {
    size_t oldBarStatus = 0;          // to note the change between new value and old value, used in refreshValue function
    size_t yellowStart;               // when yellow colour should be set, used in init function
    size_t redStart;                  // when red colour should be set, used in init function

    /**
     * @brief This function is default initialize function, calculates yellowStart and redStart
     * 
     * @return 
     */
    void init();

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
    Bar(const char *title, const char *unit, bool isEditable, void (*event_function)(), uint16_t divider = 1,
        int min = 0, int max = 100);

    /**
     * @brief This is default destructor
     */
    ~Bar() = default;

    /**
     * @brief This function is required by Box class, but nothing to do here
     */
    void calculateWidth() override {}

    /**
     * @brief This function prints frame of the box on console, IN LOOP USE ONLY
     * 
     * @return
     */
    void refreshFrame() override;

    /**
     * @brief This function prints value of the box on console, IN LOOP USE ONLY
     * 
     * @return
     */
    void refreshValue() override;

    /**
     * @brief This function is used when user do an event on the box 
     * 
     * @param array of keys pushed by user
     * @param num of keys
     * @return
     */
    void keyboardEvent(const uint8_t *buff, uint8_t numOfKeys) override;
};

#endif // BOX_HPP