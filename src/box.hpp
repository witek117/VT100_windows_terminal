#ifndef BOX_HPP_
#define BOX_HPP_

#include <cinttypes>
#include "VT100.hpp"

/**
 * @brief This class describes virtual box class, boxes can be added to windows 
 */
class Box {
protected:

    long map(long x, long in_min, long in_max, long out_min, long out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    int positionX, positionY;
    const char *title;

    bool isEditable = true;                 // possibility to edit from keyboard
    bool isActive = false;                  // when is set, color of box e.g. can be changed, to show that it is active current box
    bool refreshFrameBool = false;          // set in interrupt, use in loop to refresh frame of box
    bool refreshValueBool = false;          // set in interrupt, use in loop to refresh value of box
    bool delPrt = false;                    // set in interrupt, use in loop to tell window_manger to delete pointer to this instance, e.g. after quit
    uint_fast16_t width = 0;                // size of the box
    uint_fast16_t height = 0;

    void (*eventFunctionPointer)() = nullptr;

    void eventFunction() {
        if (eventFunctionPointer) {
            eventFunctionPointer();
        }
    }

public:
    /**
     * @brief This enum class contains the type of box
     */
    enum class TYPE {
        BAR, SLIDER, LABEL, TEXT
    };
    TYPE type;                              // type of box must be set in constructor 

    /**
     * @brief This constructor is a basic constructor
     * 
     * @param pointer to const char title 
     * @param bool statement describes possibility to edit the box
     */
    Box(const char *title, bool isEditable);

    /**
     * @brief This constructor is a expanded constructor
     * 
     * @param pointer to const char title 
     * @param bool statement describes possibility to edit the box
     * @param pointer to function which is used when value of the box is changed
     * @param height of the box
     */
    Box(const char *title, bool isEditable, void (*eventFunction)(), uint8_t height);

    /**
     * @brief This is default destructor
     */
    ~Box();

    /**
     * @brief This function initializes the box
     *
     * @param position X on console
     * @param position Y on console
     * @param width of the box
     * @return
     */
    virtual void init(int positionX, int positionY, int width);

    /**
     * @brief This function initializes the box
     * 
     * @param position X on console 
     * @param position Y on console 
     * @return 
     */
    virtual void init(int positionX, int positionY);

    /**
     * @brief This function is default initialize function
     * 
     * @return 
     */
    virtual void init();

    /**
     * @brief This function calculates the width of the box, depended io the content
     * 
     * @return 
     */
    virtual void calculateWidth() = 0;

    /**
     * @brief This function returns width od the box
     * 
     * @return width
     */
    uint16_t getWidth() const { return width; }

    /**
     * @brief This function returns height od the box
     * 
     * @return height
     */
    uint8_t getHeight() const { return height; }

    /**
     * @brief This function returns bool statement describes possibility to edit the box
     * 
     * @return isEditable
     */
    bool getEditable() const { return isEditable; }

    /**
     * @brief This function returns bool statement describes if it is currently active box
     * 
     * @return isActive
     */
    bool getActive() const { return isActive; }

    /**
     * @brief This function refreshes the frame of the box on console
     * 
     * @return
     */
    virtual void refreshFrame() = 0;

    /**
     * @brief This function refreshes the value of the box on console
     * 
     * @return
     */
    virtual void refreshValue(bool refreshHard) = 0;

    /**
     * @brief This function is used when user do an event on the box, 
     * 
     * @param array of keys pushed by user
     * @param num of keys
     * @return
     */
    virtual void keyboardEvent(const uint8_t *buff, uint8_t numOfKeys) = 0;

    /**
     * @brief This function refreashes frame and value of the box on console
     * 
     * @return
     */
    void refresh();

    /**
     * @brief This function sets the bool statement isActive, can be used in interrupt
     * 
     * @return
     */
    void setActive();

    /**
     * @brief This function resets the bool statement isActive, can be used in interrupt
     * 
     * @param if delPrt is set, window manager should delete a pointer to this box
     * @return
     */
    void setNotActive(bool delPrt = false);

    /**
     * @brief This function is used ONLY IN LOOP to refreash frame and value of the box on console
     * 
     * @return bool statement delPtr
     */
    bool inLoop();
};

#endif // BOX_HPP