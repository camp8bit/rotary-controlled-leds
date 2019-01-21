#include "FastLED.h"

#define NUM_LEDS 50
#define DATA_PIN 9

int val;
int encoder0Click = 4;
int encoder0PinA = 2;
int encoder0PinB = 3;
int encoder0Pos = 16;
int encoder0PinALast = LOW;
int n = LOW;

// Long click to power on / off
bool poweredOff = false;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  pinMode (encoder0Click, INPUT);
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  digitalWrite(encoder0Click, HIGH);  

  Serial.begin (9600);
  delay(500);

  // ductedFan.attach(7);
  FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

byte brightness = 255;
long offset = 0;
float counter = 0;

void rainbow () {
  fill_rainbow(leds, NUM_LEDS, (offset * 3) % 255, 4);
}

void chase () {
  int x; 
  
  for (x = 0; x < NUM_LEDS; x+= 20) {
    leds[(x + offset) % NUM_LEDS] = CHSV(offset, 255, 255);
  }
}

void sweep () {
  int x; 
  
  for (x = 0; x < 16; x++) {
    leds[(x + offset) % NUM_LEDS] = CHSV(offset, 255, 255);
  }
}

void sweepUpDown () {
  int m = NUM_LEDS / 2;
  int x = m - abs(offset % (2*m) - m);
  
  for (int i = i; i < NUM_LEDS; i++) {
    if (i < m && i < x) {
      leds[i] = CHSV(offset, 255, 255);
    }

    if (i > m && NUM_LEDS - i < x) {
      leds[i] = CHSV(offset, 255, 255);
    }
  }
}

void noise () {
  int x; 

  FastLED.setBrightness(brightness);
  
  for (x = 0; x < 4; x++) {
    leds[random(NUM_LEDS)] = CRGB(255, 255, 255);
  }
}

void fade () {
  fill_solid(leds, NUM_LEDS, CHSV(offset, 255, 255));
}

void whiteStrobe () {
  int x;
  
  for (x = 0; x < NUM_LEDS; x+= 2) {
    leds[x] = offset % 4 < 1  ? CRGB(255, 255, 255) : CRGB::Black;
  }
}

void colorStrobe () {
  if (offset % 4 < 1) {
    fill_solid(leds, NUM_LEDS, CHSV(offset, 255, 255));
  }
}

void primaryColors () {
  fill_solid(leds, NUM_LEDS, CHSV((long) floor(offset / 16) % 3 * 85, 255, 255));
}


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { noise, rainbow, fade, sweep, sweepUpDown, chase, whiteStrobe, colorStrobe, primaryColors };
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current


void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void loop() {
//  EVERY_N_SECONDS( 2 ) { 
//    nextPattern(); 
//   }

  EVERY_N_MILLISECONDS( 15 ) { 
    float speed = (float) max(0, min(512, encoder0Pos)) / 64;
    brightness = max(0, min(127, encoder0Pos)) * 2;
    
    // speed = s
    
    counter += speed;
    offset = floor(counter);
    
    fill_solid(leds, NUM_LEDS, CRGB::Black);

    if (!poweredOff) {
      gPatterns[gCurrentPatternNumber]();
    }
    
    FastLED.show();
  }
  if (!digitalRead(encoder0Click)) {
    // Serial.print ("Click\n");

     if (poweredOff) {
       poweredOff = false;

       while (!digitalRead(encoder0Click)) {
         // wait
         true;
       }
     } else {
       long t = millis();
       bool longPress = false;

       while (!digitalRead(encoder0Click)) {
         if (millis() - t > 500) {

           if (!longPress) {
             longPress = true;

             fill_solid(leds, NUM_LEDS, CRGB::Black);
             FastLED.show();
           }
         }
       }

       if (longPress) {
         poweredOff = true;
       } else {
         nextPattern(); 
       }
     }
  }
  
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos++;
    } else {
      encoder0Pos--;
    }
    Serial.print (encoder0Pos);
    Serial.print ("/");
  }
  encoder0PinALast = n;
  
  // delay(1);
  // delay(50);
}

