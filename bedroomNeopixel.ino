#include "config.h"
#include <Adafruit_NeoPixel.h>

#define PIN        D1 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 8 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_BRG + NEO_KHZ800);
AdafruitIO_Feed *color = io.feed("color");
byte red=255, green=255, blue=255;

void setup() {
  pinMode(A0, INPUT);
  pinMode(D4, OUTPUT);
  
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(115200);
  
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  
  color->onMessage(getColor);
  while(io.mqttStatus() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);      
  }
  color->get();
  Serial.println();
  Serial.println(io.statusText());
  digitalWrite(D4,HIGH);
}

void loop() {
  int bright = map(analogRead(A0), 50, 1024, 0,255);
  bright = constrain(bright, 50,255);
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.fill(pixels.Color(red*bright/255, green*bright/255, blue*bright/255));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }
  io.run();
}

void getColor(AdafruitIO_Data *data) {
  byte rgb[6];
  Serial.print("received <- ");
  Serial.println(data->value());
  for(int i=0; i<7; i++){
    if(i != 0){
      char c = data->value()[i];
       if ((c >= '0') && (c <= '9'))
          rgb[i-1] = c - '0';
       if ((c >= 'a') && (c <= 'f')) 
        rgb[i-1] = c - 'a' + 10;
    }
  }
  
  red = rgb[0]*16+rgb[1];
  green = rgb[2]*16+rgb[3];;
  blue = rgb[4]*16+rgb[5];;
  Serial.print("red : ");
  Serial.println(red);
  Serial.print("green : ");
  Serial.println(green);
  Serial.print("blue : ");
  Serial.println(blue);
}
