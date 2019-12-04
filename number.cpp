#include "number.hpp"

Number::Number(const char *unit, uint16_t divider, int minValue, int maxValue)
    :  value(minValue), unit(unit), divider(divider), minValue(minValue), maxValue(maxValue) {}

void Number::set_max_Value(int maxValue) {
    this->maxValue = maxValue;
}

void Number::set_min_Value(int minValue) {
    this->minValue = minValue;
}

int Number::get_value() {
    return value;
}

void Number::printValue() {
    if (divider == 1) {
        VT::print(prevValue);
    } else if (divider == 10) {
        VT::print(prevValue / 10);
        VT::print('.');
        VT::print(prevValue % 10);
    } else if (divider == 100) {
        int tempVal = prevValue / 100;
        VT::print(tempVal);
        VT::print('.');
        tempVal = prevValue % 100;
        VT::print(tempVal / 10);
        VT::print(tempVal % 10);
    } else if (divider == 1000) {
        int tempVal = prevValue / 1000;
        VT::print(tempVal);
        VT::print('.');
        tempVal = prevValue % 1000;
        tempVal = tempVal / 100;
        VT::print(tempVal);
        tempVal = prevValue % 100;
        VT::print(tempVal / 10);
        VT::print(tempVal % 10);
    }
}

void Number::increase() {
    if (value < maxValue) {
        value++;
    }
}

void Number::decrease() {
    if (value > minValue) {
        value--;
    }
}

void Number::set(int value) {
    if (value > maxValue) {
        this->value = maxValue;
    } else if (value < minValue) {
        this->value = minValue;
    } else {
        this->value = value;
    }
}







