#include <Arduino.h>

#include "KraftKontrol.h"

#include "KraftKontrol/modules/hid_modules/display_modules/st7735_driver.h"

#include "hardware_info.h"


//SX1280Driver radio = SX1280Driver(SX1280_RFBUSY_PIN, SX1280_TXEN_PIN, SX1280_RXEN_PIN, SX1280_DIO1_PIN, SX1280_NRESET_PIN, SX1280_NSS_PIN);
//KraftKommunication commsPort(&radio, eKraftPacketNodeID_t::eKraftPacketNodeID_controller);
//KraftKonnectNetwork network(&commsPort);

BME280Driver baro(&Wire, 0x76);

ST7735Driver display(SPI, LCD_LED_PIN, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN);

ADS1115Driver adc(Wire, 50);


//KinematicData kinematics;

//Vector<> vehiclePosSet = Vector<>(0,0,2);

//String commsMessage;

//WorldPosition vehicleWorldPosition;
//uint8_t vehicleNumSats = 0;




class Observer: public Task_Abstract {
public:

    Observer() : Task_Abstract(20, eTaskPriority_t::eTaskPriority_Middle) {}

    void thread() {

        //NavigationData navData = navigationmodule.getNavigationData();

        //Quaternion att = navData.attitude;
        //Vector vec = navData.attitude.copy().conjugate().rotateVector(navData.angularAcceleration);
        //float altitude = navData.position.z;

        //Serial.println(String("Attitude: w: ") + att.w + ", x: " + att.x + ", y: " + att.y + ", z: " + att.z + ". Altitude: " + altitude);
        //Serial.println(String("Vec: x: ") + vec.x + ", y: " + vec.y + ", z: " + vec.z);

        //Serial.println(String("Hello World! Speed: ") + F_CPU_ACTUAL + ", Rate: " + getSchedulerTickRate());

        //digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));


        /*float voltage;
        uint32_t timestamp;

        for (int i = 0; i < 4; i++) {
            if (adc.voltageAvailable(i)) {
                adc.getVoltage(&voltage, &timestamp, i);
                Serial.println(String("Channel: ") + i + String(", Voltage: ") + voltage + ", at time: " + timestamp + ", Rate: " + adc.measurementRate());
                adc.flushVoltage(i);
            }
        }*/

        /*if (!digitalRead(JOYSTICK_BUTTON_PIN) && !buttonModeLock) {
            buttonModeLock = true;

            currentPos++;
            if (currentPos >= sizeof(posSetpoints)/sizeof(posSetpoints[0])) currentPos = 0;
            vehiclePosSet = posSetpoints[currentPos];

        } else if (digitalRead(JOYSTICK_BUTTON_PIN) && buttonModeLock) {
            buttonModeLock = false;
        }*/

        /*if (digitalRead(JOYSTICK_BUTTON_PIN) && !joystickButtonLock) {
            joystickButtonLock = true;
            controlPos = !controlPos;
        } else if (!digitalRead(JOYSTICK_BUTTON_PIN) && joystickButtonLock) joystickButtonLock = false;

        const float thres = 0.3;

        if ((abs(joystickValue[0]) > thres || abs(joystickValue[1]) > thres) && !posChangeLock) {

            posChangeLock = true;

            float angle = atan2(joystickValue[1], joystickValue[0])/DEGREES;

            if (controlPos) {
                if (abs(angle) < 22.5) vehiclePosSet += Vector<>(0,-1,0); 
                else if (angle > 22.5 && angle < 67.5) vehiclePosSet += Vector<>(1,-1,0); 
                else if (angle > 67.5 && angle < 112.5) vehiclePosSet += Vector<>(1,0,0); 
                else if (angle > 112.5 && angle < 157.5) vehiclePosSet += Vector<>(1,1,0); 
                else if (angle < -22.5 && angle > -67.5) vehiclePosSet += Vector<>(-1,-1,0); 
                else if (angle < -67.5 && angle > -112.5) vehiclePosSet += Vector<>(-1,0,0); 
                else if (angle < -112.5 && angle > -157.5) vehiclePosSet += Vector<>(-1,1,0); 
                else vehiclePosSet += Vector<>(0,1,0); 
            } else {
                if (abs(angle) < 22.5) ;//vehiclePosSet += Vector<>(0,-1,0); 
                else if (angle > 22.5 && angle < 67.5) vehiclePosSet += Vector<>(0,0,1); 
                else if (angle > 67.5 && angle < 112.5) vehiclePosSet += Vector<>(0,0,1); 
                else if (angle > 112.5 && angle < 157.5) vehiclePosSet += Vector<>(0,0,1); 
                else if (angle < -22.5 && angle > -67.5) vehiclePosSet += Vector<>(0,0,-1); 
                else if (angle < -67.5 && angle > -112.5) vehiclePosSet += Vector<>(0,0,-1); 
                else if (angle < -112.5 && angle > -157.5) vehiclePosSet += Vector<>(0,0,-1); 
                //else vehiclePosSet += Vector<>(0,1,0);
            }

        } else if ((abs(joystickValue[0]) < thres && abs(joystickValue[1]) < thres) && posChangeLock) {

            posChangeLock = false;

        }

        if (!digitalRead(PROG_KILL_BUTTON_PIN) && !buttonLock) {
            buttonLock = true;
            if (vehicleModeSet == eVehicleMode_t::eVehicleMode_Disarm) vehicleModeSet = eVehicleMode_t::eVehicleMode_Arm;
            else vehicleModeSet = eVehicleMode_t::eVehicleMode_Disarm;
        } else if (digitalRead(PROG_KILL_BUTTON_PIN) && buttonLock) {
            buttonLock = false;
        }

        if (adc.voltageAvailable() > 0) {
            uint32_t timestamp;
            adc.getVoltage(&joystickValue[0], &timestamp, JOYSTICK_X_ADC_PIN);
            adc.getVoltage(&joystickValue[1], &timestamp, JOYSTICK_Y_ADC_PIN);

            joystickValue[0] -= 3.3/2;
            joystickValue[0] /= -3.3/2;
            joystickValue[1] -= 3.3/2;
            joystickValue[1] /= -3.3/2;

            adc.getVoltage(&vBat, &timestamp, VBAT_DIV_ADC_PIN);

            vBat *= VBAT_DIV_FACTOR;
            vBat *= VBAT_CORRECTION_SCALER;
            vBat += VBAT_CORRECTION_OFFSET;
        }*/

        /*display.clearDisplay();
        display.drawString(String("Att w: ") + kinematics.attitude.w, 0, 0);
        display.drawString(String("    x: ") + kinematics.attitude.x);
        display.drawString(String("    y: ") + kinematics.attitude.y);
        display.drawString(String("    z: ") + kinematics.attitude.z);
        display.drawString(String("Pos x: ") + vehiclePosSet.x + " is x: " + kinematics.position.x);
        display.drawString(String("    y: ") + vehiclePosSet.y + "    y: " + kinematics.position.y);
        display.drawString(String("    z: ") + vehiclePosSet.z + "    z: " + kinematics.position.z);
        display.drawString(String("Control mode: ") + (controlPos ? "position":"height"));
        display.drawString(commsMessage);

        display.drawString(String("Vehicle mode set: ") + (vehicleModeSet == eVehicleMode_Arm ? "Armed." : "Disarmed."));
        display.drawString(String("Vehicle mode is: ") + (vehicleModeIs == eVehicleMode_Arm ? "Armed." : "Disarmed."));
        display.drawString(String("Packets per sec: ") + packetRateCalc.packetRate);

        display.drawString(String("Vehicle lat: ") + String(vehicleWorldPosition.latitude/DEGREES,10));
        display.drawString(String("Vehicle lon: ") + String(vehicleWorldPosition.longitude/DEGREES,10));
        display.drawString(String("Vehicle num sats: ") + vehicleNumSats);
        display.drawString(String("VBat transmitter: ") + vBat);

        display.updateDisplay();

        adc.flushVoltage(0);
        adc.flushVoltage(1);
        adc.flushVoltage(2);
        adc.flushVoltage(3);*/


    }


private:

    bool buttonLock = false;

    bool posChangeLock = false;

    bool joystickButtonLock = false;

    bool controlPos = true;

};



class ImAlive: public Task_Abstract {
public:

    ImAlive() : Task_Abstract(1, eTaskPriority_t::eTaskPriority_Realtime) {}

    void thread() {

        Serial.println(String("Num tasks: ") + Task_Abstract::getTaskList().getNumItems());
        Serial.println("Task usages: ");
        Serial.println(String("Scheduler: ") + String(Task_Abstract::getSchedulerSystemUsage()*100, 2));
        for (uint32_t i = 0; i < Task_Abstract::getTaskList().getNumItems(); i++) {
            Serial.println(String("- Task ") + (i+1) + ": Usage: " + String(Task_Abstract::getTaskList()[i]->getTaskSystemUsage()*100, 2) + ", rate: " + Task_Abstract::getTaskList()[i]->getLoopRate());
        }

    }

    void init() {

        Serial.println("IM ALIVE. IVE BEEN INITIALISED!");

    }

    void removal() {

        Serial.println("WHY HAVE YOU FORSAKEN MEEEE!");

    }


};



ImAlive imAlive;
Observer observer;



void setup() {

    Serial.begin(115200);

    pinMode(VEXT_SWITCH_PIN, OUTPUT);
    digitalWrite(VEXT_SWITCH_PIN, LOW);

    Wire.begin(26, 27, 400000);

    //while(millis() < 10000) 

    //digitalWrite(LCD_LED_PIN, LOW);

    Task_Abstract::schedulerInitTasks();

}


void loop() {

    Task_Abstract::schedulerTick();
    
}