#ifndef JOYSTICK_H
#define JOYSTICK_H



#include "button.h"

#include "ADS1X15.h"


enum DIRECTION_STATE {
    DIRECTION_NONE,
    DIRECTION_UP,
    DIRECTION_UP_RIGHT,
    DIRECTION_RIGHT,
    DIRECTION_RIGHT_DOWN,
    DIRECTION_DOWN,
    DIRECTION_DOWN_LEFT,
    DIRECTION_LEFT,
    DIRECTION_LEFT_UP
};



class Joystick {
public:

    bool begin();

    void read();

    DIRECTION_STATE getDirectionState();

    float getVerticalValue();
    float getHorizontalValue();

    BUTTON_STATE getButtonState();


private:

    ADS1115 _adc = ADS1115(0x48);

    Button _button;


};


extern Joystick joystick;


#endif