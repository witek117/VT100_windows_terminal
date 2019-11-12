#ifndef TEXT_HPP_
#define TEXT_HPP_

#include "VT100.hpp"
#include "box.hpp"
#include <cstring>

class Text : public Box {
    size_t current_tab_index = 0;
    size_t previous_tab_index = 255;
    size_t max_text_length = 0;
    std::vector<const char *> texts;

  public:

    /**
     * @brief This is constructor 
     * 
     * @param pointer to const char title 
     * @param bool statement describes possibility to edit the box
     * @param pointer to function which is used when value of the box is changed
     */
    Text(const char *title, bool isEditable, void (*event_function)());

    /**
     * @brief This is default destructor
     */
    ~Text() = default;

    /**
     * @brief This function calculates max width of box, depending on text content in texts vector
     */
    void calculate_width() override ;

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
    void refresh_value(bool refresh_hard) override ;

    /**
     * @brief This function is used when user do an event on the box 
     * 
     * @param array of keys pushed by user
     * @param num of keys
     * @return
     */
    void keyboard_event(const uint8_t *buff, uint8_t numOfKeys) override ;

    /**
     * @brief This function initializes the box
     * 
     * @param position X on console 
     * @param position Y on console 
     * @return 
     */
    void init(int positionX, int positionY) override ;

    /**
     * @brief This function increase current_tab_index by one
     * 
     * @return
     */
    void increase();

    /**
     * @brief This function decrease current_tab_index by one
     * 
     * @return
     */
    void decrease();
    
    /**
     * @brief This function returns current current_tab_index
     * 
     * @return current_tab_index
     */
    size_t get_value_index();

    /**
     * @brief This function adds new text do texts vector
     * 
     * @return
     */
    void add_text(const char *text) ;
};

#endif // TEXT_HPP_