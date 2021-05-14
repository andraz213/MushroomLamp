#ifndef HANDLE_LED_H
#define HANDLE_LED_H
#include <Arduino.h>
#include "global_defines.h"

typedef struct {
  bool done;
  int position;
  float distance;
  float initial_distance;
  float speed;
  float strength;
} star_data;



void reset_leds();
void efekt_rgb();
void show_leds();
bool init_neo();
void handle_leds();
void efekt_rgb();
void recalc_brightness();
void calculate_stars();
void warm_white();
void setNewColor(int pix);
void calculate_fire();
void fill_color(int r, int g, int b);
#endif
