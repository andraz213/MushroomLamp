#include "handle_sleep.h"
#include "handle_input.h"
#include <ESP8266WiFi.h>
#include "handle_led.h"
void callback() {

}


void handle_sleep(){
  if(get_brightness() == 0){
    if(get_last_change() > 3000){
      do_sleep();
      reset_inputs();
      reset_leds();
    }
  }
}

void do_sleep(){
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(MOSFET_PIN, LOW);
  gpio_pin_wakeup_enable(GPIO_ID_PIN(ROTARY_PRESS), GPIO_PIN_INTR_LOLEVEL);
  wifi_set_opmode(NULL_MODE);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_open();
  wifi_fpm_set_wakeup_cb(callback);
  wifi_fpm_do_sleep(0xFFFFFFF);
  delay(1000);
  digitalWrite(MOSFET_PIN, HIGH);
  WiFi.mode(WIFI_OFF);
  wifi_fpm_set_sleep_type (LIGHT_SLEEP_T);
  WiFi.forceSleepBegin();

}
