#include "handle_input.h"
#include <Arduino.h>
#include "global_defines.h"

bool ota_mode = false;

long prev_rotary = 0;
long rotary_history[100][2];
int rotary_history_ptr = 0;
int rotary_turned = 30;

int mode = 0;

long prev_press = 0;
void ICACHE_RAM_ATTR handle_press(){
  if(millis() - prev_press > 200){
    Serial.println("PRESSED");
    mode += 1;
    mode %= NUM_STATES;

    prev_press = millis();

    while(digitalRead(ROTARY_PRESS) == LOW && false){
      if(millis() - prev_press > 3000){
        ota_mode = true;
        return;
      }
    }


  }
}



void ICACHE_RAM_ATTR handle_turn(){

  int dir = 0;
  if(millis() - prev_rotary > 40){
    Serial.println("TURNED");
    prev_rotary = millis();

      if(digitalRead(ROTARY_B) == LOW){
        dir--;
      } else {
        dir++;
      }

      rotary_history[rotary_history_ptr][0] = millis();
      rotary_history[rotary_history_ptr][1] = dir;

      rotary_history_ptr += 1;
      rotary_history_ptr %= 100;

      int n = 0;
      int summ = 0;
      for(int i = 0; i< 100; i++){
        if(millis() - rotary_history[i][0] < 1000){
          n += 1;
          summ += rotary_history[i][1];
        }
      }

      dir = -1;

      if(summ > 0){
        dir = 1;
      }

      int mult = map(n*n, 1, 1000, 1,10);

      dir*= mult;
      rotary_turned += dir;

      if(rotary_turned < 0){
        rotary_turned = 0;
      }

      if(rotary_turned > 100){
        rotary_turned = 100;
      }

      Serial.println(mult);
      Serial.println(dir);

      //update_last_input();

    Serial.println(rotary_turned);

  }





}

void setup_pins(){
  pinMode(ROTARY_PRESS, INPUT_PULLUP);
  pinMode(ROTARY_A, INPUT_PULLUP);
  pinMode(ROTARY_B, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(ROTARY_PRESS), handle_press, FALLING);
  attachInterrupt(digitalPinToInterrupt(ROTARY_A), handle_turn, FALLING);

}




int get_mode(){
  return mode % NUM_STATES;
};

int get_brightness(){
  if(rotary_turned < 0){
    return 0;
  }
  if(rotary_turned > 100){
    return 100;
  }
  return rotary_turned;
}


int get_last_change(){
  return (int)(millis() - prev_rotary);
}


void reset_inputs(){

  prev_rotary = millis();
  rotary_turned = 10;

  mode = 0;


}


bool is_ota(){
  return ota_mode;
}
