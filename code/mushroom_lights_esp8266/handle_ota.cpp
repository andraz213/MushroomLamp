#include "handle_ota.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFiMulti.h>
#include "handle_led.h"
#include "handle_input.h"

ESP8266WiFiMulti wifiMulti;


void handle_ota(){

  fill_color(200, 155, 5);

  if(connect_wifi()){
    fill_color(0, 0, 200);
    ArduinoOTA.setHostname("MUSHROOM");

    ArduinoOTA
    .onStart([]() {
      fill_color(0, 0, 200);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
      fill_color(0, 255, 100);
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {

      fill_color(0, (int)(progress / (total / 200)), 200);

      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));


    });

    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.printf("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.printf("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.printf("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.printf("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.printf("End Failed");

      fill_color(255, 0, 0);
      delay(1500);
      ESP.restart();

    });

  ArduinoOTA.begin();

  long start = millis();
  while(millis() - start < (3 * 60 * 1000) && get_brightness() > 0){
      ArduinoOTA.handle();
      yield();
  }

  ESP.restart();

  }

  ESP.restart();

}




bool connect_wifi(){

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP("PSP256", "siol2004");
  wifiMulti.addAP("srecko3", "SA05NA99");
  long start = millis();
  while (wifiMulti.run() != WL_CONNECTED && millis() - start < 15000) {
    yield();
  }
  if (wifiMulti.run() == WL_CONNECTED){
    return true;
  }
  return false;


}
