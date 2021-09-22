#ifndef KRAFTPAD_MAINMENU_H
#define KRAFTPAD_MAINMENU_H



#include "KraftKontrol/gui/menu_abstract.h"

#include "KraftKontrol/utils/topic_subscribers.h"
#include "KraftKontrol/modules/communication_modules/kraft_message_subscriber.h"
#include "KraftKontrol/modules/communication_modules/kraft_kommunication.h"

#include "KraftKontrol/KraftPacket_KontrolPackets/kraftkontrol_telemetry_messages.h"
#include "KraftKontrol/KraftPacket_KontrolPackets/kraftkontrol_command_messages.h"

#include "KraftKontrol/data_containers/navigation_data.h"



/**
 * This menu shows on the display the value of a given item. The name of the menu will also display the value.
 */
class Menu_KraftpadMain: public Menu_Abstract, public Task_Abstract {
private:

    //Control for menu
    Buffer_Subscriber<eMenuControlInput_t, 10> menuControlSubscriber_;

    //Used for sending commands to vehicle.
    Topic<MessageLinkData>& messageSendTopic_;
    
    TelemetryMessageAttitude vehicleAttitude_;
    TelemetryMessagePosition vehiclePosition_;
    TelemetryMessageVelocity vehicleVelocity_;
    TelemetryMessageVehicleModeIs vehicleModeIs_;

    //Receives messages and places them into correct message storage.
    KraftMessage_Subscriber messageSubscriber_;

    //Vehicle mode set.
    eVehicleMode_t vehicleModeSet_;

    //Vehicle Position Set
    Vector<> vehiclePositionSet_;


public:

    /**
     * @param comms Kraft kommunication module being used.
     * @param menuControl Control input for menu. 
     */
    Menu_KraftpadMain(KraftKommunication& comms, MenuControl_Abstract& menuControl);


    void init() override;


    void thread() override;


private:

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


    void exit() override;

    void enter() override;

};



#endif