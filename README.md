# MushroomLamp
 
A birthday present for my girlfriend. 

It's a bedside lamp. It features RGB ws2812 led lights and possible IoT expansion because of the esp8266 that is used. 

The lamp turns on when the smallest mushroom is pressed. The light is dimmed when the lamp turns on because it's meant to be a bedside lamp that can be used in the middle of the night. The brightness can be adjusted by rotating the smallest mushroom. Pressing on the smallest mushroom cycles the different modes of operation:

1. Static warm light
2. Slow pastel changing colors
3. Shooting stars shower
4. Fire animation 

The lamp is turned off by turning the brightness down.



## Enclosure

Mushrooms and the base were modeled in Fusion 360. The model is available on: https://a360.co/3hpJmct


![Render]<img src="https://raw.githubusercontent.com/andraz213/MushroomLamp/main/assets/render1.png" width="50%">



All parts were printed on a Creality ender3V2 with 0.25mm layer height and 33% gyroid infill at 205C out of ecoPLA white filament. Then it was sanded down with 220 grit sandpaper and glued with Loctite glue.

![Final Product]<img src="https://raw.githubusercontent.com/andraz213/MushroomLamp/main/assets/photo1.jpg" width="50%">



## Electronics

The stars of the show are two Neopixel rings. The bigger one features 12 chips, the smaller one has 8 chips. They're controlled by a WEMOS D1 board with an esp8266 MCU. I used a rotary encoder for user input and a 18650 Li-ion battery with a TP4056 battery protection circuit.

I have implemented a few power-saving features. The MCU is never turned off and is only put to sleep. Neopixels also use each 1mA even when they're not used, so I used an IRFZ44N MOSFET to cut the power to them when the unit is off. Additionally, I have disabled all wifi functionally to further reduce consumption.


![Circuit]<img src="https://raw.githubusercontent.com/andraz213/MushroomLamp/main/assets/circuit.png" width="50%">


