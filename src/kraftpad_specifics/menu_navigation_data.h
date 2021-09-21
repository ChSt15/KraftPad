#ifndef MENU_NAVIGATION_DATA_H
#define MENU_NAVIGATION_DATA_H



#include"KraftKontrol/gui/menu_abstract.h"

#include "KraftKontrol/utils/topic_subscribers.h"

#include "KraftKontrol/data_containers/navigation_data.h"



/**
 * This menu shows on the display the value of a given item. The name of the menu will also display the value.
 */
class Menu_NavigationData: public Menu_Abstract {
private:

    //List of menus to show
    const char* (*valueFunc)(void) = nullptr;

    //Control for menu
    Simple_Subscriber<eMenuControlInput_t> menuControlSubscriber_;

    MenuControl_Abstract& menuControl_;
    
    //Which menu in list the cursor points to.
    enum eNavDataSelect_t : uint32_t {
        eNavDataSelect_Accel,
        eNavDataSelect_LinearAccel,
        eNavDataSelect_Velocity,
        eNavDataSelect_Position,
        eNavDataSelect_AngAccel,
        eNavDataSelect_AngVelocity,
        eNavDataSelect_Attitude
    } menuCursor_ = eNavDataSelect_t::eNavDataSelect_Accel;

    //Subscriber for navigation data.
    Simple_Subscriber<NavigationData> navDataSubr_;


    Buffer<Vector<>, 160> dataBuffer_;


public:

    /**
     * @param menuValueName Title or identifier of value to be shown.
     * @param valueFunc Function to call the gets the value and returns a char pointer to a formatted value.
     * @param menuControl Control input for menu. 
     */
    Menu_NavigationData(const char* menuName, Topic<NavigationData>& navDataTopic, MenuControl_Abstract& menuControl);


protected:

    /**
     * Virtual method to display current menu status for the GUI.
     * Subclass must implement this function and write to the given display.
     * 
     * Menu return function works as following:
     * - returning a pointer to this menu means to stay on this menu.
     * - returning a nullptr means to return to last menu.
     * - returning a pointer to another menu will switch to that menu. 
     * 
     * @param display Display to be updated with menu interface.
     * @param gui Gui task that called this menu. Can be used to change display fps, etc. 
     * @returns pointer to Menu_Abstract.
     */
    Menu_Abstract* menuDisplayUpdate(Display_Abstract& display, Task_Abstract& gui) override;


    void resetMenu() override;


private:

    void drawGraph(Display_Abstract& display, Buffer<Vector<>, 160>& dataBuffer);


};



#endif