#include <FastLED.h>
#include "IRremote.h"

#define LED_PIN     13
int buttonApin = 9;//one button  to pin 9 on the Arduino

// Information about the LED strip itself
#define NUM_LEDS    50
#define CHIPSET     WS2811
#define COLOR_ORDER RGB

int receiver = 11; // Signal Pin of IR receiver to Arduino Digital Pin 11
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

bool ON = false;
int BRIGHTNESS_INDEX = 2;
int BRIGHTNESS_ARRAY[] = {10, 45, 80, 150, 250};
CRGB leds[NUM_LEDS];
CRGB dummyLedsHorizontal[25];

//LEFT TO RIGHT LETTER MAP
int h0[]  = {0,1,2,3,4,5,6,7} ;
int h1[]  = {8};
int h2[]  = {9,10,11,12,13,14,15,16};
int h3[]  = {17,18,19,20,21,22,23};
int h4[]  = {24};
int h5[]  = {25};
int h6[]  = {26,27,28,29,30,31,32};
int h7[]  = {36,44};
int h8[]  = {35,37};
int h9[]  = {34,38,43};
int h10[] = {39,42};
int h11[] = {33,40,41};
int h12[] = {52};
int h13[] = {53};
int h14[] = {45,46,47,48,49,50,51,54};
int h15[] = {55};
int h16[] = {56};
int h17[] = {57,58,59,60,61,62,63,64};
int h18[] = {65};
int h19[] = {66};
int h20[] = {67};
int h21[] = {68,69,70,71,72,73,74,75,76};
int h22[] = {77,82,83};
int h23[] = {78,81,84};
int h24[] = {79,80,85};

void setup() {
  pinMode(buttonApin, INPUT_PULLUP);  
  
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS_ARRAY[BRIGHTNESS_INDEX]);

// Set up dummy leds, data pin not actually used
  FastLED.addLeds<WS2812B, 1, RGB>(dummyLedsHorizontal, 25);



  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
//  while (!irrecv.isIdle()) {
//   
//  }


  if (irrecv.decode(&results)){ // have we received an IR signal?
    translateIR(); 
    irrecv.resume(); // receive the next value
    
  }  

  if (digitalRead(buttonApin) == LOW){
    turnOnOff();
    delay(500);
  }

}

void turnOnOff() {
  if (ON == false)
    {
      fill_solid( leds, NUM_LEDS, CRGB::Teal );
      ON = true;
      FastLED.show();
    } else if (ON == true) {
      ON = false;
      BRIGHTNESS_INDEX = 2;
      FastLED.setBrightness(BRIGHTNESS_ARRAY[BRIGHTNESS_INDEX]);
      FastLED.clear();
      FastLED.show();  
    }
}

void setSolidBlue() {
  fill_solid( leds, NUM_LEDS, CRGB::Blue);
  ON = true;
  FastLED.show();  
}

void setSolidOrange() {
  fill_solid( leds, NUM_LEDS, CRGB::Orange);
  ON = true;      
  FastLED.show();  
}

void setSolidTeal() {
  fill_solid( leds, NUM_LEDS, CRGB::Teal);
  ON = true;      
  FastLED.show();  
}

void setRainbow() {
  fill_rainbow(leds, NUM_LEDS, 0, 255/NUM_LEDS);
  ON = true;      
  FastLED.show(); 
}

void increaseBrightness() {
  if(BRIGHTNESS_INDEX < 4) {
    BRIGHTNESS_INDEX++;
    FastLED.setBrightness(BRIGHTNESS_ARRAY[BRIGHTNESS_INDEX]);
    FastLED.show();
  }
}

void decreaseBrightness() {
 if(BRIGHTNESS_INDEX > 0) {
    BRIGHTNESS_INDEX--;
    FastLED.setBrightness(BRIGHTNESS_ARRAY[BRIGHTNESS_INDEX]);
    FastLED.show();
 }
}

void translateIR(){
  Serial.println("VALUE = ");
  Serial.println(results.value);
  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER"); turnOnOff(); break;
  case 0xFFE21D: Serial.println("VOL STOP");  break;
  case 0xFF629D: Serial.println("MODE");  break;
  case 0xFF22DD: Serial.println("PAUSE");    break;
  case 0xFF02FD: Serial.println("FAST BACK");    break;
  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
  case 0xFFE01F: Serial.println("EQ");    break;
  case 0xFFA857: Serial.println("VOL-"); decreaseBrightness();   break;
  case 0xFF906F: Serial.println("VOL+"); increaseBrightness();    break;
  case 0xFF9867: Serial.println("RETURN");    break;
  case 0xFFB04F: Serial.println("USB SCAN");    break;
  case 0xFF6897: Serial.println("0");   convertPatternHorizontal(); break;
  case 0xFF30CF: Serial.println("1");   setSolidBlue(); break;
  case 0xFF18E7: Serial.println("2");   setSolidOrange(); break;
  case 0xFF7A85: Serial.println("3");   setSolidTeal(); break;
  case 0xFF10EF: Serial.println("4");    break;
  case 0xFF38C7: Serial.println("5");   setRainbow(); break;
  case 0xFF5AA5: Serial.println("6");    break;
  case 0xFF42BD: Serial.println("7");    break;
  case 0xFF4AB5: Serial.println("8");    break;
  case 0xFF52AD: Serial.println("9");    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;  

  default:
    Serial.println(" other button   ");

  }// End Case

  delay(350); // Do not get immediate repeat
}

void convertPatternHorizontal() {
  fill_rainbow(dummyLedsHorizontal, 25, 0, 255/NUM_LEDS);
  ON = true;



  updateLeds(h0, 8, 0);
  updateLeds(h1, 1, 10);
  updateLeds(h2, 8, 20);
  FastLED.show(); 
}

void updateLeds(int pixelGroup[], int pixelGroupSize, int dummyLedIndex) {
  // CRGB colorToSet = CRGB(leds[dummyLedIndex].r,leds[dummyLedIndex].g,leds[dummyLedIndex].b);
  int redToSet = dummyLedsHorizontal[dummyLedIndex].r;
  int blueToSet = dummyLedsHorizontal[dummyLedIndex].b;
  int greenToSet = dummyLedsHorizontal[dummyLedIndex].g;

    Serial.print("DUMMY LEDS R VALUE ")   ;   
                Serial.print(redToSet);
        Serial.print(",  ");
                        Serial.print(blueToSet);
        Serial.print(",  ");
                        Serial.print(greenToSet);
        Serial.print(",  ");
                        Serial.println("   ");
                                                Serial.println(" ****************  ");

                                                Serial.println("   ");


  Serial.println(dummyLedsHorizontal[dummyLedIndex].r)   ;   

  for(int i =0; i < pixelGroupSize; i++) {
         Serial.print("MAP VALUE ");
         Serial.print(i);
        Serial.print(",  ");

        Serial.println(pixelGroup[i]);



      leds[pixelGroup[i]].red = redToSet;
      leds[pixelGroup[i]].blue = blueToSet;
      leds[pixelGroup[i]].green = greenToSet;

      Serial.print("LED RED VALUE ");
      Serial.println(leds[pixelGroup[i]].r);                 

        

  }
}
