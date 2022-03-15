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
int BRIGHTNESS = 100;
CRGB leds[NUM_LEDS];



void setup() {
  pinMode(buttonApin, INPUT_PULLUP);  
  
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

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
      fill_solid( leds, NUM_LEDS, CRGB::Blue);
      ON = true;
      FastLED.show();
    } else if (ON == true) {
      ON = false;
      BRIGHTNESS = 100;
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.clear();
      FastLED.show();  
    }
}

void increaseBrightness() {
  if(BRIGHTNESS < 226) {
    BRIGHTNESS += 30;
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.show();
  }
}

void decreaseBrightness() {
 if(BRIGHTNESS  > 29) {
    BRIGHTNESS -= 30;
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.show();
 }
}

void translateIR(){
  Serial.println("VALUE = ");
  Serial.println(results.value);
  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER"); turnOnOff(); break;
  case 0xFFE21D: Serial.println("VOL STOP"); break;
  case 0xFF629D: Serial.println("MODE"); break;
  case 0xFF22DD: Serial.println("PAUSE");    break;
  case 0xFF02FD: Serial.println("FAST BACK");    break;
  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
  case 0xFFE01F: Serial.println("EQ");    break;
  case 0xFFA857: Serial.println("VOL-"); decreaseBrightness();   break;
  case 0xFF906F: Serial.println("VOL+"); increaseBrightness();    break;
  case 0xFF9867: Serial.println("RETURN");    break;
  case 0xFFB04F: Serial.println("USB SCAN");    break;
  case 0xFF6897: Serial.println("0");    break;
  case 0xFF30CF: Serial.println("1");    break;
  case 0xFF18E7: Serial.println("2");    break;
  case 0xFF7A85: Serial.println("3");    break;
  case 0xFF10EF: Serial.println("4");    break;
  case 0xFF38C7: Serial.println("5");    break;
  case 0xFF5AA5: Serial.println("6");    break;
  case 0xFF42BD: Serial.println("7");    break;
  case 0xFF4AB5: Serial.println("8");    break;
  case 0xFF52AD: Serial.println("9");    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;  

  default:
    Serial.println(" other button   ");

  }// End Case

  delay(500); // Do not get immediate repeat
}
