#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <cstring>
#include <vector>

#include "VT100.hpp"
#include "bar.hpp"
#include "box.hpp"
#include "label.hpp"
#include "slider.hpp"
#include "text.hpp"

class Window {
    const char *title;              // window title
    uint8_t titleLen = 0;           //

    int positionX;            
    int positionY;
    int width;
    int height;

    bool isEditable = false;
    bool refreshFrameBool = false;  // set in interrupt, use in loop to refresh frame of box

    bool delPrt = false;            // set in interrupt, use in loop to tell window_manger to delete pointer to this instance, e.g. after quit
    bool isActive = false;          // when is set, color of box e.g. can be changed, to show that it is active current box

    void print_frame(VT::COLOUR frameColor);
    
 
  public:
    std::vector<Box *> boxes;       // vectors of pointers to boxes 

    /**
     * @brief This coppy constructor
     * 
     * @param reference to window
     */
    Window(const Window &win);

    /**
     * @brief This constructor is a expanded constructor
     * 
     * @param pointer to const char title 
     * @param position X on console 
     * @param position Y on console 
     * @param width of the window
     * @param height of the window
     * @param bool statement describes possibility to edit the window
     */
    Window(const char *title, int positionX, int positionY, int width, int height, bool isEditlable);

    /**
     * @brief This function is default initialize function, initializes all positions ob boxes
     * 
     * @return 
     */
    void init();
    
    /**
     * @brief This function returns bool statement describes possibility to edit the box
     * 
     * @return isEditable
     */
    bool get_editable() { return isEditable; }

    /**
     * @brief This function sets the bool statement isActive, can be used in interrupt
     * 
     * @return
     */  
    void set_active();

    /**
     * @brief This function resets the bool statement isActive, can be used in interrupt
     * 
     * @param if delPrt is set, window manager should delete a pointer to this box
     * @return
     */  
    void set_unactive(bool delPrt_ = false);   

    /**
     * @brief This function refreashes frame and value of the box on console
     * 
     * @return
     */ 
    void refresh();

    /**
     * @brief This function refreshes the frame of of the boxes in window
     * 
     * @return
     */
    void refresh_frame();

    /**
     * @brief This function refreshes the value of of the boxes in window
     * 
     * @return
     */
    void refresh_value();

    /**
     * @brief This function is used ONLY IN LOOP to refreash frame and value of the boxes in window
     * 
     * @return bool statement delPtr
     */  
    bool in_loop();


    /**
     * @brief This function adds new box do boxes vector
     * 
     * @return
     */
    void add_box(Box *box);

    
};



#endif