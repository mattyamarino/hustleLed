#include <FastLED.h>
#include "IRremote.h"

#define LED_PIN     13
int buttonApin = 9;//one button  to pin 9 on the Arduino

// Information about the LED strip itself
#define NUM_LEDS    86
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

  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  while (!irrecv.isIdle()) { // pauses loop while ir input is being recieved
  }

  if (irrecv.decode(&results)){ // have we received an IR signal?
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  

  if (digitalRead(buttonApin) == LOW){
    turnOnOff();
    delay(500);
  }
}

//*********** REMOTE CONTROL FUNCTIONS ***********

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
  fill_rainbow(dummyLedsHorizontal, 25, 0, 255/NUM_LEDS);
  ON = true;
  convertPatternHorizontal(); 
  FastLED.show(); 
}

void setCylon() {
  static uint8_t hue = 0;
	// First slide the led in one direction
	for(int i = 0; i < 25; i++) {
		// Set the i'th led to red 
		dummyLedsHorizontal[i] = CHSV(hue++, 255, 255);

		// Show the leds
		FastLED.show(); 
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
	}
	Serial.print("x");

	// Now go in the other direction.  
	for(int i = (25)-1; i >= 0; i--) {
		// Set the i'th led to red 
		dummyLedsHorizontal[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
	}
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
  switch(results.value) {
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
  case 0xFF6897: Serial.println("0");    break;
  case 0xFF30CF: Serial.println("1");   setSolidBlue(); break;
  case 0xFF18E7: Serial.println("2");   setSolidOrange(); break;
  case 0xFF7A85: Serial.println("3");   setSolidTeal(); break;
  case 0xFF10EF: Serial.println("4");    break;
  case 0xFF38C7: Serial.println("5");   setRainbow(); break;
  case 0xFF5AA5: Serial.println("6");   setCylon(); break;
  case 0xFF42BD: Serial.println("7");    break;
  case 0xFF4AB5: Serial.println("8");    break;
  case 0xFF52AD: Serial.println("9");    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;  

  default:
    Serial.println(" other button   ");

  }// End Case

  delay(350); // Do not get immediate repeat
}


//*********** MAPPING FUNCTIONS ***********

void convertPatternHorizontal() {

  updateLeds(h0, 8, 0);
  updateLeds(h1, 1, 1);
  updateLeds(h2, 8, 2);
  updateLeds(h3, 7, 3);
  updateLeds(h4, 1, 4);
  updateLeds(h5, 1, 5);
  updateLeds(h6, 7, 6);
  updateLeds(h7, 2, 7);
  updateLeds(h8, 2, 8);
  updateLeds(h9, 3, 9);
  updateLeds(h10, 2, 10);
  updateLeds(h11, 3, 11);
  updateLeds(h12, 1, 12);
  updateLeds(h13, 1, 13);
  updateLeds(h14, 8, 14);
  updateLeds(h15, 1, 15);
  updateLeds(h16, 1, 16);
  updateLeds(h17, 8, 17);
  updateLeds(h18, 1, 18);
  updateLeds(h19, 1, 19);
  updateLeds(h20, 1, 20);
  updateLeds(h21, 9, 21);
  updateLeds(h22, 3, 22);
  updateLeds(h23, 3, 23);
  updateLeds(h24, 3, 24);
}

void convertIndividualColumn(int index) {
  switch(index) {
  case 0:  updateLeds(h0, 8, 0);   break;
  case 1:  updateLeds(h1, 1, 1);   break;
  case 2:  updateLeds(h2, 8, 2);   break;
  case 3:  updateLeds(h3, 7, 3);   break;
  case 4:  updateLeds(h4, 1, 4);   break;
  case 5:  updateLeds(h5, 1, 5);   break;
  case 6:  updateLeds(h6, 7, 6);   break;
  case 7:  updateLeds(h7, 2, 7);   break;
  case 8:  updateLeds(h8, 2, 8);   break;
  case 9:  updateLeds(h9, 3, 9);   break;
  case 10: updateLeds(h10, 2, 10); break;
  case 11: updateLeds(h11, 3, 11); break;
  case 12: updateLeds(h12, 1, 12); break;
  case 13: updateLeds(h13, 1, 13); break;
  case 14: updateLeds(h14, 8, 14); break;
  case 15: updateLeds(h15, 1, 15); break;
  case 16: updateLeds(h16, 1, 16); break;
  case 17: updateLeds(h17, 8, 17); break;
  case 18: updateLeds(h18, 1, 18); break;
  case 19: updateLeds(h19, 1, 19); break;
  case 20: updateLeds(h20, 1, 20); break;
  case 21: updateLeds(h21, 9, 21); break;
  case 22: updateLeds(h22, 3, 22); break;
  case 23: updateLeds(h23, 3, 23); break;
  case 24: updateLeds(h24, 3, 24); break;

  default:
    Serial.println(" ERROR: convertColumnHorizontal failed ");
}

void updateLeds(int pixelGroup[], int pixelGroupSize, int dummyLedIndex) {
  CRGB colorToSet = CRGB(dummyLedsHorizontal[dummyLedIndex].r,dummyLedsHorizontal[dummyLedIndex].g,dummyLedsHorizontal[dummyLedIndex].b);

  for(int i =0; i < pixelGroupSize; i++) {
      
      leds[pixelGroup[i]] = colorToSet;

  }
}

// *********** ANIMATION FUNCTIONS ***********
// CYLON
void fadeall() { for(int i = 0; i < 25; i++) { dummyLedsHorizontal[i].nscale8(250); } }

