#include "handle_led.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "handle_input.h"


#define NUMPIXELS 20

int to_show [NUMPIXELS][3];
bool inited_neo = false;
float pixs[NUMPIXELS][2][3];
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void reset_leds(){
  inited_neo = false;
}

void show_leds(){
  if(init_neo()){
    pixels.clear();


    for(int i = 0; i < NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(to_show[i][0], to_show[i][1], to_show[i][2]));

    }

    pixels.show();
  }
}


bool init_neo(){
  if(!inited_neo){
      pixels.begin();
      pixels.setBrightness(255);
      for(int i = 0; i < NUMPIXELS; i++){
        to_show[i][0] = 0;
        to_show[i][1] = 0;
        to_show[i][2] = 0;
        pixels.setPixelColor(i, pixels.Color(to_show[i][0], to_show[i][1], to_show[i][2]));

      }

      pixels.show();

      for(int i = 0; i<NUMPIXELS; i++){
        pixs[i][0][0] = 0;
        pixs[i][0][1] = 0;
        pixs[i][0][2] = 0;
        pixs[i][1][0] = 0;
        pixs[i][1][1] = 0;
        pixs[i][1][2] = 0;
      }

      inited_neo = true;
  }

digitalWrite(MOSFET_PIN, HIGH);
return inited_neo;

}


void handle_leds(){
  int mode = get_mode(); // get mode

  int brightness = 100;


  switch(mode){
    case 0 : warm_white(); break;
    case 1 : efekt_rgb(); break;
    case 2 : calculate_stars(); break;
    case 3: calculate_fire(); break;
  }


  // Serial.println("HUH");

  recalc_brightness();
  // Serial.println("HHHH");
  show_leds();
  // Serial.println("HHHHsadfaf");


}



void recalc_brightness(){
  int brightness = get_brightness();
  int fak = 0;
  if(brightness < 15){
    fak = brightness;
  } else {
  fak = map(brightness * brightness, 15*15, 100*100, 15, 100);
  }


  for(int i = 0; i < NUMPIXELS; i++){
    to_show[i][0] = to_show[i][0] * fak / 100;
    to_show[i][1] = to_show[i][1] * fak / 100;
    to_show[i][2] = to_show[i][2] * fak / 100;

    if(to_show[i][0] < 0){
      to_show[i][0] = 0;
    }
    if(to_show[i][1] < 0){
      to_show[i][1] = 0;
    }
    if(to_show[i][2] < 0){
      to_show[i][2] = 0;
    }
    if(to_show[i][0] > 255){
      to_show[i][0] = 255;
    }
    if(to_show[i][1] > 255){
      to_show[i][1] = 255;
    }
    if(to_show[i][2] > 255){
      to_show[i][2] = 255;
    }
  }

}





void efekt_rgb(){


  for(int i = 0; i<NUMPIXELS; i++){
    float d = abs(pixs[i][0][0] - pixs[i][1][0]);
    d += abs(pixs[i][0][1] - pixs[i][1][1]);
    d += abs(pixs[i][0][2] - pixs[i][1][2]);


    if(abs(d) < 4.0){
      int r = random(25);
      int g = random(25);
      int b = random(25);

      int avg = (r + g + b)/3;
      int delta = abs(avg - r) + abs(avg - g) + abs(avg - b);
      while(delta < 30 || avg > 20){

        r = random(25);
        g = random(25);
        b = random(25);

        avg = (r + g + b)/3;
        delta = abs(avg - r) + abs(avg - r) + abs(avg - r);
      }

      pixs[i][1][0] = r;
      pixs[i][1][1] = map(g, 0, 25, 0, 20);
      pixs[i][1][2] = map(b, 0, 25, 0, 22);

    }

    float fak = (float)(random(10) + 10)/1000.0;
    if(fak < 0.01){
      fak = 0.01;
    }

    pixs[i][0][0] += fak * (pixs[i][1][0] - pixs[i][0][0]);
    pixs[i][0][1] += fak * (pixs[i][1][1] - pixs[i][0][1]);
    pixs[i][0][2] += fak * (pixs[i][1][2] - pixs[i][0][2]);

    to_show[i][0] = (int)pixs[i][0][0] * 10.0;
    to_show[i][1] = (int)pixs[i][0][1] * 10.0;
    to_show[i][2] = (int)pixs[i][0][2] * 10.0;

  }

}






const int num_stars = 4;
// kje, kok bliz(močno), kok hitr, kera faza
star_data stars[num_stars];

long prev_stars = 0;

bool first_stars = true;

void calculate_stars(){
 Serial.println("stars");

  for(int i = 0; i<NUMPIXELS; i++){
    to_show[i][0] = 0;
    to_show[i][1] = 0;
    to_show[i][2] = 0;
  }

  int calc_num = num_stars; // * stars_data[0] / 100;

  if(first_stars){
    calc_num = num_stars;
  }

// Serial.println("hejadsfjg");
  for(int i = 0; i<calc_num; i++){
    if(stars[i].done == true || first_stars){
      stars[i].done = false;
      stars[i].position = random(NUMPIXELS);
      stars[i].distance = 400.0;
      stars[i].initial_distance = stars[i].distance;
      stars[i].speed = random(70) + 10;
      stars[i].strength = random(255);
    }

// Serial.println("hejjgaaa");
      for(int j = 0; j<3; j++){

        int rp = j*10;

        float dist_to_star = abs(stars[i].distance)*abs(stars[i].distance);


        float brightness = stars[i].strength - (stars[i].strength * dist_to_star / pow(stars[i].initial_distance, 2)); //  map(dist_to_star, 0, 130, stars[i].strength, 0);// / dist_to_star;
        brightness /= ((j*2)+1);
        // Serial.println("hejjgbbb");
        if(j == 0){
          to_show[stars[i].position][0] += (int)brightness;
          to_show[stars[i].position][1] += (int)brightness;
          to_show[stars[i].position][2] += (int)brightness;
        } else {

          int pos = stars[i].position + j;
          pos %= NUMPIXELS;

          to_show[pos][0] += (int)brightness;
          to_show[pos][1] += (int)brightness;
          to_show[pos][2] += (int)brightness;

          pos = stars[i].position - j + NUMPIXELS;
          pos %= NUMPIXELS;

          to_show[pos][0] += (int)brightness;
          to_show[pos][1] += (int)brightness;
          to_show[pos][2] += (int)brightness;

        }
        // Serial.println("hejjgccc");

        stars[i].distance -= stars[i].speed * (millis() - prev_stars) / 400.0;

        if(stars[i].distance < -400){
          stars[i].done = true;
        }
  }


    }
    // Serial.println("hejjgddd");
    prev_stars = millis();
        first_stars = false;

        // Serial.println("hejjgsss");
        for(int i = 0; i<NUMPIXELS; i++){
          int val = to_show[i][0];
          if(val> 255){
            val = 255.0;
          }

          // Serial.println("hejjgsdfs");
          int calc = map(val*val, 0, 255*255, 0, 255);
            to_show[i][0] = (int)calc;
            to_show[i][1] = (int)((float)calc * 0.6);
            to_show[i][2] = (int)((float)calc * 0.3);

            // Serial.println(i);
          }
          // Serial.println("hsdf");

}



void warm_white(){
  int brightness = get_brightness();

  int r = 255;
  int g = 119;
  int b = 49;

  if(brightness < 50){
    g = map(brightness, 0, 50, 90, 119);
    b = map(brightness, 0, 50, 30, 49);

  }
  Serial.println("r");
  Serial.println(r);
  Serial.println("g");
  Serial.println(g);
  Serial.println("b");
  Serial.println(b);



  for(int i = 0; i < NUMPIXELS; i++){


    to_show[i][0] = r;
    to_show[i][1] = g;
    to_show[i][2] = b;
  }

}








int prev [NUMPIXELS][3];
int current [NUMPIXELS][3];
int next[NUMPIXELS][3];
long prevupd = 0;
int update_interval = 150;


void calculate_fire(){


  if(millis() - prevupd > update_interval){
    prevupd = millis();
    update_interval = random((30)*10) + 50;
    for(int i = 0; i<NUMPIXELS; i++){
      setNewColor(i);
      }
    }

    int offset = (millis() - prevupd);

    int perc = map(offset, 0, update_interval, 0, 100);


    for(int i = 0; i<NUMPIXELS; i++){
      current[i][0] = (next[i][0]-prev[i][0])*perc/100 + prev[i][0];
      current[i][1] = (next[i][1]-prev[i][1])*perc/100 + prev[i][1];
      current[i][2] = (next[i][2]-prev[i][2])*perc/100 + prev[i][2];

      to_show[i][0] = current[i][0];
      to_show[i][1] = current[i][1];
      to_show[i][2] = current[i][2];
      }



}

    void setNewColor(int pix) {
      int n = random(1023);

      int g = 0;
      int b = 0;

      int plus_g = map(50, 0, 100, 35, 0);
      int plus_b = map(50, 0, 100, 10, 0);

      int r = map(n, 0, 1023, 0, 130);
      if (n > 200)
        g = map(n, 200, 1023, 0, plus_g);
      // g = map(n * n, 100 * 100, 1023 * 1023, 0, 255);
      if (n > 755)
        b = map(n, 755, 1023, 0, plus_b);

        if(pix%2 == 0){
        r = 50;
        g = 10 + plus_g*0.3;
        b = 2 + plus_b*0.1;
        }
      prev[pix][0] = current[pix][0];
      prev[pix][1] = current[pix][1];
      prev[pix][2] = current[pix][2];
      next[pix][0] = map(r, 0, 130, 0, 255);
      next[pix][1] = map(g, 0, 130, 0, 255);
      next[pix][2] = map(b, 0, 130, 0, 255);


    }



void fill_color(int r, int g, int b){
  for(int i = 0; i < NUMPIXELS; i++){

    to_show[i][0] = r;
    to_show[i][1] = g;
    to_show[i][2] = b;
  }
  recalc_brightness();
  show_leds();

}
