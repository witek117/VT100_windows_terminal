#ifndef NUMBER_HPP_
#define NUMBER_HPP_

#include "VT100.hpp"
#include "box.hpp"

/**
 * @brief This class describes Number class
 */
class Number {
protected:
    int prevValue = -1;         // to note the change between new value and old value, used in refreshValue function
    int value;                  // current value
    const char *unit;           // to const char unit, e.g. "mV", "A"  
    uint16_t divider;           // for value, to display 1.234mV, value should be 1234 and divider 1000
    int minValue;               // minimum value
    int maxValue;               // maximum value

public:

    /**
     * @brief This is constructor 
     * 
     * @param pointer to const char unit, e.g. "mV", "A" 
     * @param divider for value, to display 1.234mV, value should be 1234 and divider 1000
     * @param minimum value, default 0
     * @param maximum value, default 100
     */
    Number(const char *unit, uint16_t divider = 1, int minValue = 0, int maxValue = 100);

    /**
     * @brief This function sets maximum value that can be kept
     * 
     * @param new maximum value
     * @return
     */
    void setMaxValue(int maxValue);

    /**
     * @brief This function sets minimum value that can be kept
     * 
     * 
     * @param new minimum value
     * @return
     */
    void setMinValue(int minValue);

    /**
     * @brief This function returns current value
     * 
     * @return value
     */
    int get();

    /**
     * @brief This function prints value on console, depending on divider
     * 
     * @return
     */
    void printValue() const;

    /**
     * @brief This function increase value by one
     * 
     * @return value
     */
    void increase();

    /**
     * @brief This function decrease value by one
     * 
     * @return
     */
    void decrease();

    /**
     * @brief This function sets new value
     * 
     * @return
     */
    void set(int value);
};

#endif // VALUE_HPP_