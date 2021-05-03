#include <Arduino.h>

#include "KraftKontrol.h"

#include "hardware_info.h"


SX1280Driver radio = SX1280Driver(SX1280_RFBUSY_PIN, SX1280_TXEN_PIN, SX1280_RXEN_PIN, SX1280_DIO1_PIN, SX1280_NRESET_PIN, SX1280_NSS_PIN);
KraftKommunication commsPort(&radio, eKraftPacketNodeID_t::eKraftPacketNodeID_basestation);

//Radio radioModule; //This modules takes care of communication with the goundstation and radiocontrol. It receives waypoints or also manual control commands from controllers.


void vehicleProgram(); //Has all functions needed for vehicle testing, waypoints and stuff
void idleLoop(); //Will be ran whenever free time is available. DO NOT BLOCK e.g with delay(). Things that NEED to be ran should be placed in normal loop().
//Runner runner(&vehicle, &vehicleProgram, &radioModule, &idleLoop)



class Observer: public Task_Abstract {
public:

    Observer() : Task_Abstract(20, eTaskPriority_t::eTaskPriority_Middle, true) {}

    void thread() {

        //NavigationData navData = navigationmodule.getNavigationData();

        //Quaternion att = navData.attitude;
        //Vector vec = navData.attitude.copy().conjugate().rotateVector(navData.angularAcceleration);
        //float altitude = navData.position.z;

        //Serial.println(String("Attitude: w: ") + att.w + ", x: " + att.x + ", y: " + att.y + ", z: " + att.z + ". Altitude: " + altitude);
        //Serial.println(String("Vec: x: ") + vec.x + ", y: " + vec.y + ", z: " + vec.z);

        //Serial.println(String("Hello World! Speed: ") + F_CPU_ACTUAL + ", Rate: " + getSchedulerTickRate());

    }


};


class NetworkingReceiver: public Task_Abstract {
public:

    NetworkingReceiver() : Task_Abstract(100, eTaskPriority_t::eTaskPriority_Middle, true) {}

    void thread() {

        commsPort.loop();

        if (commsPort.messageAvailable()) {

            MessageData messageInfo = commsPort.getMessageInformation();

            if (messageInfo.payloadID == eKraftMessageType_t::eKraftMessageType_String_ID) {

                KraftMessageStringPacket stringPacket;

                commsPort.getMessage(&stringPacket);

                char string[stringPacket.getStringLength()];

                stringPacket.getString(string, sizeof(string));

                Serial.println(string);

            }

        }

    }


};



class NetworkingTransmitter: public Task_Abstract {
public:

    NetworkingTransmitter() : Task_Abstract(1, eTaskPriority_t::eTaskPriority_Middle, true) {}

    void thread() {

        KraftMessageStringPacket stringPacket((String("Hello im ") + commsPort.getSelfID() + "! Time is: " + millis()).c_str());

        commsPort.sendMessage(&stringPacket, eKraftPacketNodeID_t::eKraftPacketNodeID_broadcast);

        Serial.println("Sending: " + String("Hello im ") + commsPort.getSelfID() + "! Time is: " + millis());

        //stopTaskThreading();

    }


};


NetworkingTransmitter transmitter;
NetworkingReceiver receiver;

Observer observer;



void setup() {

    Serial.begin(115200);

}


void loop() {

    //runner.tick();
    Task_Abstract::schedulerTick();
    
}