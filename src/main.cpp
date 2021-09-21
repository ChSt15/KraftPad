#include <Arduino.h>

#include "KraftKontrol.h"

#include "KraftKontrol/modules/communication_modules/kraft_kommunication.h"

#include "KraftKontrol/modules/hid_modules/display_modules/st7735_driver.h"
#include "KraftKontrol/modules/sensor_modules/imu_modules/bno080_driver.h"

#include "KraftKontrol/KraftPacket_KontrolPackets/kraftkontrol_telemetry_messages.h"
#include "KraftKontrol/modules/communication_modules/kraft_message_subscriber.h"

#include "KraftKontrol/gui/menus/menu_list.h"
#include "KraftKontrol/gui/menus/menu_test.h"
#include "KraftKontrol/gui/menus/menu_vector.h"

#include "kraftpad_specifics/menu_navigation_data.h"


#include "hardware_info.h"


SX1280Driver radio = SX1280Driver(SX1280_RFBUSY_PIN, SX1280_TXEN_PIN, SX1280_RXEN_PIN, SX1280_DIO1_PIN, SX1280_NRESET_PIN, SX1280_NSS_PIN);
KraftKommunication commsPort(radio, eKraftMessageNodeID_t::eKraftMessageNodeID_controller);
//KraftKonnectNetwork network(&commsPort);

//UbloxSerialGNSS gnss(Serial1, M8N_RX_PIN, M8N_TX_PIN);
//BME280Driver baro(&Wire, 0x76);
//BNO080Driver imu(IMU_INT_PIN, Wire, &baro/*, &gnss*/);


ST7735Driver display(SPI, LCD_LED_PIN, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN);

ADS1115Driver adc(Wire, 50);
Joystick joystick = Joystick(adc[JOYSTICK_X_ADC_PIN], adc[JOYSTICK_Y_ADC_PIN], JOYSTICK_BUTTON_PIN);


//Menu_List mainMenu("Main Menu", joystick);
//Menu_NavigationData navMenu("NavigationData", imu.getNavigationDataTopic(), joystick);



class TestVectorClass: public Topic<Vector<>>, public Task_Abstract {
public:

    TestVectorClass(): Task_Abstract("TestVectorClass", 100, eTaskPriority_t::eTaskPriority_Realtime) {}

    TelemetryMessageAngularVelocity vector_;

    KraftMessage_Subscriber subr_ = KraftMessage_Subscriber(vector_);

    void init() {
        subr_.subscribe(commsPort.getReceivedMessageTopic());
    }

    void thread() {

        publish(vector_.getVector());

    }

} vectorTest;



Menu_Vector menuVectorTest(vectorTest, "TestVector", joystick);

Gui gui = Gui(display, menuVectorTest);









class Observer: public Task_Abstract {
public:

    Observer() : Task_Abstract("Observer", 20, eTaskPriority_t::eTaskPriority_Realtime) {}

    Simple_Subscriber<NavigationData> sensorSubr;

    void init() {

        //sensorSubr.subscribe(imu.getGyroTopic());
        //sensorSubr.setTaskToResume(*this);

    }

    void thread() {

        //Serial.println(String("Time: ") + (double)NOW()/SECONDS);

        /*if (sensorSubr.isDataNew()) {

            const SensorTimestamp<Vector<>>& vector = sensorSubr.getItem();

            Serial.println(String("Sensor: ") + vector.sensorData.toString() + ", GyroRate: " + imu.gyroRate() + ", AccelRate: " + imu.accelRate() + ", MagRate: " + imu.magRate());


        }*/

        //Serial.println(String("Joystick: ") + joystick.getJoystickData().posX + ", " + joystick.getJoystickData().posY);

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

        //stopTaskThreading();

    }


private:

    bool buttonLock = false;

    bool posChangeLock = false;

    bool joystickButtonLock = false;

    bool controlPos = true;

};



class TaskMonitor: public Task_Abstract {
public:

    TaskMonitor() : Task_Abstract("Task Monitor", 1, eTaskPriority_t::eTaskPriority_Realtime) {}

    void thread() {

        Serial.println(String("Num tasks: ") + Task_Abstract::getTaskList().getNumItems());
        Serial.println("Task usages: ");
        Serial.println(String("CPU Usage: ") + String(Task_Abstract::getSchedulerSystemUsage()*100.0, 2));
        for (uint32_t i = 0; i < Task_Abstract::getTaskList().getNumItems(); i++) {
            Serial.println(String() + "    - Usage: " + String(Task_Abstract::getTaskList()[i]->getTaskSystemUsage()*100, 2) + ", rate: " + Task_Abstract::getTaskList()[i]->getLoopRate() + "\t, Name: " + Task_Abstract::getTaskList()[i]->getTaskName());
        }

    }

    void init() {

        Serial.println("IM ALIVE. IVE BEEN INITIALISED!");

    }

    void removal() {

        Serial.println("WHY HAVE YOU FORSAKEN MEEEE!");

    }


};



TaskMonitor taskMonitor;
Observer observer;



void setup() {

    Serial.begin(115200);

    pinMode(VEXT_SWITCH_PIN, OUTPUT);
    digitalWrite(VEXT_SWITCH_PIN, LOW);

    setCpuFrequencyMhz(240);

    Wire.begin(26, 27, 400000);

    //while(millis() < 10000) 

    //digitalWrite(LCD_LED_PIN, LOW);

    /*adc[JOYSTICK_X_ADC_PIN].setScaling(-1.0f/3.3f*2.0f);
    adc[JOYSTICK_X_ADC_PIN].setOffset(1.0f);
    adc[JOYSTICK_Y_ADC_PIN].setScaling(-1.0f/3.3f*2.0f);
    adc[JOYSTICK_Y_ADC_PIN].setOffset(1.0f);*/

    //mainMenu.addMenuToList(navMenu);

    Task_Abstract::schedulerInitTasks();

}


void loop() {

    Task_Abstract::schedulerTick();
    
}