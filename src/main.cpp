#include <Arduino.h>

#include "TaskManager.h"

#include "hid_devices/button.h"
#include "hid_devices/joystick.h"




void setup() {
    Serial.begin(9600);
    
}

void testLoop() {

    joystick.read();
    
}


void loop() {

    

}