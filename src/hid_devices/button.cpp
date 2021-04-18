#include "button.h"



bool Button::begin(int16_t pin, uint32_t threshold_milliseconds, bool pullup, bool activeLow) {

    _pin = pin;
    _invert = activeLow;
    _threshold = threshold_milliseconds*1000;

    pinMode(_pin, INPUT);

    if (pullup) pinMode(_pin, INPUT_PULLUP);

    return true;

}


bool Button::_pinState() {

    if (_invert) return !digitalRead(_pin);

    return digitalRead(_pin);

}


BUTTON_STATE Button::read() {

    if (!_interval.isTimeToRun()) {
        return (_state == 2) ? BUTTON_STATE::HELD : BUTTON_STATE::RELEASED;
    }
    

    switch (_state) {

    case 0: 
        if (_pinState()) {
            _pressTimestamp = micros();
            _state = 1;
        }
        break;

    case 1: 
        if (!_pinState()) {
            _state = 0;
            return BUTTON_STATE::PRESSED;
        } else if (micros() - _pressTimestamp >= _threshold) {
            _state = 2;
            return BUTTON_STATE::HELD;
        }
        break;

    case 2: 
        if (!_pinState()) {
            _state = 0;
            return BUTTON_STATE::RELEASED;
        } else return BUTTON_STATE::HELD;
        break;
    
    default:
        break;
    }


    return BUTTON_STATE::RELEASED;

}