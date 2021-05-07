#include <Arduino.h>

#include "KraftKontrol.h"

#include "hardware_info.h"


SX1280Driver radio = SX1280Driver(SX1280_RFBUSY_PIN, SX1280_TXEN_PIN, SX1280_RXEN_PIN, SX1280_DIO1_PIN, SX1280_NRESET_PIN, SX1280_NSS_PIN);
KraftKommunication commsPort(&radio, eKraftPacketNodeID_t::eKraftPacketNodeID_basestation);

ST7735Driver display(LCD_LED_PIN);

ADS1115Driver adc = ADS1115Driver(&Wire);


KinematicData kinematics;

String commsMessage;


uint32_t packetsCounter = 0;
uint32_t packetsSent = 0;



class PacketsPerSecond: public Task_Abstract {
public:

    PacketsPerSecond() : Task_Abstract(1, eTaskPriority_t::eTaskPriority_Middle, true) {}

    void thread() {

        float dTime = float(micros() - lastRun)/1000000;
        lastRun = micros();

        packetRate = packetsCounter/dTime;
        packetsCounter = 0;

        sendRate = packetsSent/dTime;
        packetsSent = 0;

    }

    uint32_t packetRate = 0;
    uint32_t sendRate = 0;

    uint32_t lastRun = 0;

};


PacketsPerSecond packetRateCalc;


class Observer: public Task_Abstract {
public:

    Observer() : Task_Abstract(10, eTaskPriority_t::eTaskPriority_Middle, true) {}

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

        display.clearDisplay();
        display.drawString(String("Attitude w: ") + kinematics.attitude.w, 0, 0);
        display.drawString(String("         x: ") + kinematics.attitude.x);
        display.drawString(String("         y: ") + kinematics.attitude.y);
        display.drawString(String("         z: ") + kinematics.attitude.z);
        display.drawString(String("Altitude:   ") + kinematics.position.z);
        display.drawString(String("Vertical Rate:") + kinematics.velocity.z);
        display.drawString(commsMessage);
        display.drawString(String("Packets per sec: ") + packetRateCalc.packetRate);

        float voltage;
        uint32_t timestamp;
        adc.getVoltage(&voltage, &timestamp, JOYSTICK_X_ADC_PIN);
        display.drawString(String("Joystick X: ") + voltage);
        adc.getVoltage(&voltage, &timestamp, JOYSTICK_Y_ADC_PIN);
        display.drawString(String("Joystick Y: ") + voltage);
        adc.getVoltage(&voltage, &timestamp, VBAT_DIV_ADC_PIN);
        display.drawString(String("VBat Y: ") + voltage/VBAT_DIV_FACTOR);

        adc.flushVoltage(0);
        adc.flushVoltage(1);
        adc.flushVoltage(2);
        adc.flushVoltage(3);

    }


};





class NetworkingReceiver: public Task_Abstract {
public:

    NetworkingReceiver() : Task_Abstract(1000, eTaskPriority_t::eTaskPriority_Middle, true) {}

    void thread() {

        commsPort.loop();

        if (commsPort.messageAvailable()) {

            packetsCounter++;

            MessageData messageInfo = commsPort.getMessageInformation();

            if (messageInfo.payloadID == eKraftMessageType_t::eKraftMessageType_String_ID) {

                KraftMessageStringPacket stringPacket;

                commsPort.getMessage(&stringPacket);

                char string[stringPacket.getStringLength()];

                stringPacket.getString(string, sizeof(string));

                commsMessage = string;

                //Serial.println(string + String(", Rate: ") + packetRateCalc.packetRate);

            } else if (messageInfo.payloadID == eKraftMessageType_KraftKontrol_t::eKraftMessageType_KraftKontrol_Attitude) {

                KraftMessageAttitude attitudeMessage;

                commsPort.getMessage(&attitudeMessage);

                kinematics.attitude = attitudeMessage.getAttitude();

            } else if (messageInfo.payloadID == eKraftMessageType_KraftKontrol_t::eKraftMessageType_KraftKontrol_Position) {

                

                KraftMessagePosition positionMessage;

                commsPort.getMessage(&positionMessage);

                kinematics.position = positionMessage.getPosition();
                //positionTimestamp = positionMessage.getTimestamp();

                //Serial.println(String("position: ") + position.x + ", " + position.y + ", " + position.z);

            } else if (messageInfo.payloadID == eKraftMessageType_KraftKontrol_t::eKraftMessageType_KraftKontrol_FullKinematics) {

                KraftMessageFullKinematics kinematicsMessage;

                commsPort.getMessage(&kinematicsMessage);

                kinematics = kinematicsMessage.getKinematics();

                //Serial.println(String("position: ") + position.x + ", " + position.y + ", " + position.z);

            }

        }

    }

};


class NetworkingTransmitter: public Task_Abstract {
public:

    NetworkingTransmitter() : Task_Abstract(1, eTaskPriority_t::eTaskPriority_Middle, true) {}

    void thread() {

        if (!enableSending) return;
        if (commsPort.networkBusy() || commsPort.networkAckBusy()) return;

        packetsSent++;

        KraftMessageStringPacket stringPacket(String(String("Hello im ") + commsPort.getSelfID() + "! Time is: " + millis() + String("Im sending ") + packetRateCalc.sendRate + " packets per second").c_str());

        commsPort.sendMessage(&stringPacket, eKraftPacketNodeID_t::eKraftPacketNodeID_broadcast);

        Serial.println("Sending: " + String("Hello im ") + commsPort.getSelfID() + "! Time is: " + millis());

        //stopTaskThreading();

    }

    bool enableSending = false;


};


NetworkingTransmitter transmitter;
NetworkingReceiver receiver;

Observer observer;



void setup() {

    Serial.begin(115200);

    pinMode(VEXT_SWITCH_PIN, OUTPUT);
    digitalWrite(VEXT_SWITCH_PIN, LOW);

    pinMode(LCD_LED_PIN, OUTPUT);
    digitalWrite(LCD_LED_PIN, LOW);

    pinMode(JOYSTICK_BUTTON_PIN, INPUT);

    pinMode(LCD_CS_PIN, OUTPUT);
    digitalWrite(LCD_CS_PIN, HIGH);
    pinMode(LCD_DC_PIN, OUTPUT);
    digitalWrite(LCD_DC_PIN, HIGH);

    pinMode(BUZZER_PIN, OUTPUT);

    Wire.begin(26, 27, 400000);

    //while(millis() < 10000) 

    //digitalWrite(LCD_LED_PIN, LOW);

}


void loop() {

    Task_Abstract::schedulerTick();

    if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) {

        transmitter.enableSending = true;

    } else {
        transmitter.enableSending = false;
    }

    
    
}