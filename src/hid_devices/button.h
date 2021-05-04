#ifndef BUTTON_H
#define BUTTON_H



#include "Arduino.h"

#include "stdint.h"

#include "KraftKontrol.h"



enum BUTTON_STATE {
    NONE,
    RELEASED,
    PRESSED,
    HELD
};



class Button {
public:

    bool begin(int16_t pin, uint32_t threshold_milliseconds = 500, bool pullup = false, bool activeLow = false);

    BUTTON_STATE read();


private:

    virtual bool _pinState();

    IntervalControl _interval = IntervalControl(20);

    int16_t _pin;
    bool _invert = false;
    uint32_t _threshold;

    uint8_t _state = 0;
    uint32_t _pressTimestamp;

};





#endif