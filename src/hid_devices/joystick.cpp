#include "joystick.h"


Joystick joystick;


bool Joystick::begin() {

    _button.begin(0, 1000, true, true);

    _adc.begin();
    _adc.setGain(1);
    _adc.setDataRate(7);
    _adc.setMode(0);
    _adc.readADC(0);

}