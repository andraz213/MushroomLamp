#include "handle_input.h"
#include <ESP8266WiFi.h>
#include "handle_sleep.h"
#include "handle_led.h"
#include "handle_ota.h"


void setup() {
  WiFi.mode(WIFI_OFF);
  wifi_fpm_set_sleep_type (LIGHT_SLEEP_T);
  WiFi.forceSleepBegin();   // if you dont need wifi, ~22ma instead of ~75ma

  Serial.begin(115200);

  Serial.println("hejj");

  setup_pins();

  Serial.println("hejjd");




}
long prevl = millis();
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("hejjf");

  if(is_ota()){
    //handle_ota();
  }

  int del = millis() - prevl;
  prevl = millis();
  //Serial.print("FPS: ");
  //Serial.println(1000 / del);

  handle_leds();
  handle_sleep();

  yield();
  delay(16);

    //Serial.println("Exit light sleep mode");
}
