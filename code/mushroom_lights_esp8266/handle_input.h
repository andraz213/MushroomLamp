#ifndef HANDLE_INPUT_H
#define HANDLE_INPUt_H

#include "global_defines.h"


void setup_pins();

int get_mode();
int get_brightness();
int get_last_change();
void reset_inputs();
bool is_ota();
#endif
