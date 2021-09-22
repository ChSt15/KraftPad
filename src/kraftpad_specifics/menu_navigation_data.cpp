#include "menu_navigation_data.h"



Menu_NavigationData::Menu_NavigationData(const char* menuName, Topic<NavigationData>& navDataTopic, MenuControl_Abstract& menuControl): Menu_Abstract(menuName), menuControl_(menuControl) {

    navDataSubr_.subscribe(navDataTopic);

    menuControlSubscriber_.subscribe(menuControl.getMenuInputTopic());

}


Menu_Abstract* Menu_NavigationData::menuDisplayUpdate(Display_Abstract& display, Task_Abstract& gui) {

    Menu_Abstract* returnValue = this;

    //Handle inputs first
    if(menuControlSubscriber_.isDataNew()) {

        eMenuControlInput_t controlInput = menuControlSubscriber_.getItem();

        switch (controlInput) {
        case eMenuControlInput_t::eMenuControlInput_LeftShort:
            if (menuCursor_ == 0) menuCursor_ = eNavDataSelect_t(7);
            menuCursor_ = eNavDataSelect_t(menuCursor_ - 1);
            break;

        case eMenuControlInput_t::eMenuControlInput_RightShort:
            menuCursor_= eNavDataSelect_t(menuCursor_ + 1);
            if (menuCursor_ == 7) menuCursor_ = eNavDataSelect_t(0);
            break;

        case eMenuControlInput_t::eMenuControlInput_LeftLong: //Leave menu.
            returnValue = nullptr;
            menuControlSubscriber_.unsubcribe();
            break;
        
        default:
            break;
        }

    }


    //Retrieve navigation data
    const NavigationData& navData = navDataSubr_.getItem();

    dataBuffer_.placeFront(navData.angularRate, true);


    //Draw menu status

    Color color(255);

    display.clear();

    display.drawText(40, 0, color, 1, getMenuName());

    /*display.drawText(0, 20, color, 0, (String("Positional Data: ")).c_str());
    display.drawText(0, 30, color, 0, (String("Acc: ") + navData.acceleration.toString()).c_str());
    display.drawText(0, 40, color, 0, (String("Lin: ") + navData.linearAcceleration.toString()).c_str());
    display.drawText(0, 50, color, 0, (String("Vel: ") + navData.velocity.toString()).c_str());
    display.drawText(0, 60, color, 0, (String("Pos: ") + navData.position.toString()).c_str());

    display.drawText(0, 80, color, 0,  (String("Angular Data: ")).c_str());
    display.drawText(0, 90, color, 0,  (String("Acc: ") + navData.angularAcceleration.toString()).c_str());
    display.drawText(0, 100, color, 0, (String("Vel: ") + navData.velocity.toString()).c_str());
    display.drawText(0, 110, color, 0, (String("Att: ") + navData.attitude.toString()).c_str());*/

    switch (menuCursor_) {
    case eNavDataSelect_t::eNavDataSelect_Accel :
        display.drawText(0, 20, color, 0, " > Acceleration");
        display.drawText(0, 30, color, 0, (navData.acceleration.toString()).c_str());
        break;

    case eNavDataSelect_t::eNavDataSelect_LinearAccel :
        display.drawText(0, 20, color, 0, " > Linear Acceleration");
        display.drawText(0, 30, color, 0, (navData.linearAcceleration.toString()).c_str());
        break;

    case eNavDataSelect_t::eNavDataSelect_Velocity :
        display.drawText(0, 20, color, 0, " > Velocity");
        display.drawText(0, 30, color, 0, (navData.velocity.toString()).c_str());
        break;

    case eNavDataSelect_t::eNavDataSelect_Position :
        display.drawText(0, 20, color, 0, " > Position");
        display.drawText(0, 30, color, 0, (navData.position.toString()).c_str());
        break;

    case eNavDataSelect_t::eNavDataSelect_AngAccel :
        display.drawText(0, 20, color, 0, " > Angular Acceleration");
        display.drawText(0, 30, color, 0, (navData.angularAcceleration.toString()).c_str());
        break;

    case eNavDataSelect_t::eNavDataSelect_AngVelocity :
        display.drawText(0, 20, color, 0, " > Angular Velocity");
        display.drawText(0, 30, color, 0, (navData.angularRate.toString()).c_str());
        break;

    case eNavDataSelect_t::eNavDataSelect_Attitude :
        display.drawText(0, 20, color, 0, " > Attitude");
        display.drawText(0, 30, color, 0, (String("w:") + navData.attitude.w).c_str());
        display.drawText(0, 40, color, 0, (navData.attitude.toVector().toString()).c_str());
        break;
    
    default:
        menuCursor_ = eNavDataSelect_t(0);
        break;
    }

    drawGraph(display, dataBuffer_);

    display.update();

    return returnValue;

}


void Menu_NavigationData::drawGraph(Display_Abstract& display, Buffer<Vector<>, 160>& dataBuffer) {

    Color color(255);

    float maxValue = 0.00001;

    //Find max
    for (uint32_t i = 0; i < dataBuffer.available(); i++) {
        if (abs(dataBuffer[i].x) > maxValue) maxValue = abs(dataBuffer[i].x);
    }

    for (uint32_t i = 0; i < dataBuffer.available(); i++) {

        display.drawPixel(i, (float) 100 + 30*dataBuffer[i].x/maxValue, color);

    }

}


void Menu_NavigationData::exit() {}

