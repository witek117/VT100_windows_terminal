#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <cstring>

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

    void printFrame(VT::COLOUR frameColor);

    constexpr static uint8_t maxBoxesCount = 10;
    Box* boxes[maxBoxesCount] {nullptr};
    uint8_t actualBoxesCount = 0;
public:

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
     *
     */
     uint8_t getBoxesCont() const {
        return actualBoxesCount;
     }

     /**
      *
      */
      Box* getBoxPointer(uint8_t index) {
          if (index < actualBoxesCount) {
              return boxes[index];
          } else {
              return nullptr;
          }
      }

      /**
       *
       */
       bool getActiveBox(uint8_t index) {
          if (index < actualBoxesCount) {
              return boxes[index]->getActive();
          } else {
              return false;
          }
       }

    /**
     * @brief This function returns bool statement describes possibility to edit the box
     * 
     * @return isEditable
     */
    bool getEditable() const { return isEditable; }

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
    void setNonActive(bool delPrt_ = false);

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
    void refreshFrame();

    /**
     * @brief This function refreshes the value of of the boxes in window
     * 
     * @return
     */
    void refreshValue(bool refreshHard);

    /**
     * @brief This function is used ONLY IN LOOP to refreash frame and value of the boxes in window
     * 
     * @return bool statement delPtr
     */
    bool inLoop();


    /**
     * @brief This function adds new box do boxes vector
     * 
     * @return
     */
    void addBox(Box *box);
};


#endif