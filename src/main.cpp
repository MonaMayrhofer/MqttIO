#include <Arduino.h>
#include <HttpServer.h>
#include <MqttClient.h>
#include <Button.h>
#include <NodeTime.h>

Button buttonA(D0, false, true, 50);

void setup() {
    Serial.begin(9600);
    HttpServer.init();
    MqttClient.init("/test/main");
    NodeTime.setNtpTimeSubscriber();


    Serial.println("====== Home Interface =======");
}

void buttonEvent(String data){
    String pressTime = String(NodeTime.getDateTime());
    pressTime.concat(";");
    pressTime.concat(data);
    char* pressTimeArr = const_cast<char*>(pressTime.c_str());
    Serial.println(pressTimeArr);
    MqttClient.publish("/test/button", pressTimeArr);
}

void buttonEventListener(){
    static bool longPress = false;
    buttonA.read();
    if(buttonA.pressedFor(500) && !longPress){
        longPress = true;
    }
   if(buttonA.wasReleased()){
        if(!longPress){
            buttonEvent("simplePress");
        }
        else if(longPress){
            buttonEvent("longPress");
        }
        longPress = false;
    }



}

int wait = 0;
int abc = 0;
void loop() {
    HttpServer.handleClient();
    buttonEventListener();
    delay(1);
}