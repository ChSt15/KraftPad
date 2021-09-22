#include "kraftpad_mainmenu.h"




Menu_KraftpadMain::Menu_KraftpadMain(KraftKommunication& comms, MenuControl_Abstract& menuControl): Menu_Abstract("Kraftpad Mainmenu"), Task_Abstract("Kraftpad Mainmenu", 20, eTaskPriority_t::eTaskPriority_Middle), messageSendTopic_(comms.getSendMessageTopic()) {

    menuControlSubscriber_.subscribe(menuControl.getMenuInputTopic());

    //messageSendPublisher_.subscribe(comms.getBroadcastMessageTopic());
    //messageSendPublisher_.receiveEnable(false); //Only want to publish, no receiving.

    messageSubscriber_.addReceiverMessage(vehicleAttitude_);
    messageSubscriber_.addReceiverMessage(vehiclePosition_);
    messageSubscriber_.addReceiverMessage(vehicleVelocity_);
    messageSubscriber_.addReceiverMessage(vehicleModeIs_);

    messageSubscriber_.subscribe(comms.getReceivedMessageTopic());
    
}


void Menu_KraftpadMain::thread()  {

    MessageLinkData messageData;
    messageData.receiverID = eKraftMessageNodeID_t::eKraftMessageNodeID_vehicle;
    messageData.requiresAck = false;

    messageData.message = CommandMessageVehicleModeSet(vehicleModeSet_);
    messageSendTopic_.publish(messageData);

    messageData.message = CommandMessagePositionSet(vehiclePositionSet_);
    messageSendTopic_.publish(messageData);

}


Menu_Abstract* Menu_KraftpadMain::menuDisplayUpdate(Display_Abstract& display, Task_Abstract& gui) {

    Menu_Abstract* returnValue = this;

    //Handle inputs first
    while(menuControlSubscriber_.available()) {

        eMenuControlInput_t controlInput;
        menuControlSubscriber_.takeBack(controlInput);

        switch (controlInput) {
        /*case eMenuControlInput_t::eMenuControlInput_LeftLong: //Leave menu.
            returnValue = nullptr;
            break;*/

        case eMenuControlInput_t::eMenuControlInput_Button1Long:
            if (vehicleModeSet_ != eVehicleMode_t::eVehicleMode_Arm) vehicleModeSet_ = eVehicleMode_t::eVehicleMode_Disarm;
            else vehicleModeSet_ = eVehicleMode_t::eVehicleMode_Disarm;
            break;

        case eMenuControlInput_t::eMenuControlInput_Button1Short:
            vehicleModeSet_ = eVehicleMode_t::eVehicleMode_Disarm;
            break;

        case eMenuControlInput_t::eMenuControlInput_UpLong:
            vehiclePositionSet_.z += 0.5;
            break;

        case eMenuControlInput_t::eMenuControlInput_DownLong:
            vehiclePositionSet_.z -= 0.5;
            break;

        case eMenuControlInput_t::eMenuControlInput_LeftLong:
            vehiclePositionSet_.y += 0.5;
            break;

        case eMenuControlInput_t::eMenuControlInput_RightLong:
            vehiclePositionSet_.y -= 0.5;
            break;
        
        default:
            break;
        }

    }


    //Draw menu status

    Color color(255);

    display.clear();

    display.drawText(20, 10, color, 1, getMenuName());

    char modeName[20];
    getVehicleModeString(modeName, 20, vehicleModeSet_);
    display.drawText(0, 20, color, 1, (String("Mode Set: ") + modeName).c_str());
    getVehicleModeString(modeName, 20, vehicleModeIs_.getData());
    display.drawText(0, 30, color, 1, (String("Mode Is: ") + modeName).c_str());




    display.update();

    return returnValue;

}


void Menu_KraftpadMain::init() {



}


void Menu_KraftpadMain::exit() {

    menuControlSubscriber_.receiveEnable(false);

}

void Menu_KraftpadMain::enter() {

    menuControlSubscriber_.receiveEnable(true);

}
