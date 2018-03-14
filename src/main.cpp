#include <Arduino.h>
#include <HttpServer.h>
#include <MqttClient.h>
#include <Button.h>
#include <NodeTime.h>
#include "ButtonListener.h"

ButtonListener buttonA(D0, false, true, 50);

void buttonACallback(int mode){
    Serial.print("Mode: ");
    Serial.println(mode);

    String data;
    switch(mode){
        case BUTTON_LISTENER_LONG:
        data = "longPress";break;
        case BUTTON_LISTENER_MULTI:
        data = "multiPress";break;
        case BUTTON_LISTENER_SIMPLE:
        data = "simplePress";break;
    }
    String pressTime = String(NodeTime.getDateTime());
    pressTime.concat(";");
    pressTime.concat(data);
    char* pressTimeArr = const_cast<char*>(pressTime.c_str());
    Serial.println(pressTimeArr);
    MqttClient.publish("/test/button", pressTimeArr);
}

void setup() {
    Serial.begin(9600);
    HttpServer.init();
    MqttClient.init("/test/main");
    NodeTime.setNtpTimeSubscriber();

    buttonA.setCallback(buttonACallback);

    Serial.println("====== Home Interface =======");
}

void loop() {
    HttpServer.handleClient();
    buttonA.listen();
    delay(1);
}