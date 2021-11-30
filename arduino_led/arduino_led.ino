#include "FastLED.h"
#define NUM_LEDS 60
CRGB leds[NUM_LEDS];

const int L=0; //Left LED
const int R=1; //Right LED
const int S=2; //Speaker
const int LEDPIN=3; //WS2812B LED DIGITAL PIN
String incomingByte;
//int previousByte;
int tLOW=0; //timer variable
int tHIGH=0;
int tLED=0;
bool AlarmState=0;
String message="";
bool debug=1;
batteryarr =[1,2,3,4,5,6,7,8,9,10];

void setup() {
  pinMode(L, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(S, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  FastLED.addLeds<WS2812B, LEDPIN>(leds, NUM_LEDS);
  Serial.setTimeout(200); //Currently Serial.readString waits for 200ms, might rewrite in the future to allow terminator \n
  Serial.begin(115200);
  Serial.println("Initalising Program");
}

void lightall(CRGB color){
  fill_solid( leds, NUM_LEDS, color);
  FastLED.show();
}

void batteryled(int batterylevel){
  for(int i=0;i++;i<batterylevel){
    if(i<5){
      leds[batteryarr[i]]=CRGB:RED;
    }else{
      leds[batteryarr[i]]=CRGB:GREEN;
    }
    
  }
}

void printto(String text){
  if(debug==1){
    Serial.println(text);
  }
}

void loop() {
  //lightall(CRGB::Red);
  //Serial.println(Serial.read());
  if (Serial.available() > 0) {
    
    //incomingByte = Serial.read();
    //printto("Reading String");
    incomingByte = Serial.readString();
    printto(incomingByte); //DEBUG
    //previousByte = incomingByte;

  //move outside
    if (String(incomingByte[0]) == "H" or AlarmState == 1) {
      printto("Alarm On");
      digitalWrite(0, LOW);
      digitalWrite(1, HIGH);
      delay(rand()%200+10);
      digitalWrite(1, LOW);
      digitalWrite(0, HIGH);
      delay(rand()%200+10);
      AlarmState==1
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (String(incomingByte[0]) == "L" or AlarmState == 0) {
      printto("Alarm Off");
      digitalWrite(L, LOW);
      digitalWrite(R, LOW);
      digitalWrite(S, LOW);
      AlarmState=0
    }

    String batterylevel = String(incomingByte);
    batterylevel.remove(0,1); //Remove flight mode to leave with batterylevel
    void batteryLED(batterylevel/10); //Send the number of LEDs(1-10) to light up

//  }else{
//     if (previousByte == 'H') {
//        digitalWrite(0, LOW);
//        digitalWrite(1, HIGH);
//        delay(rand()%200+10);
//        digitalWrite(1, LOW);
//        digitalWrite(0, HIGH);
//        delay(rand()%200+10);
//    }
//    // if it's an L (ASCII 76) turn off the LED:
//    if (previousByte == 'L') {
//      digitalWrite(L, LOW);
//      digitalWrite(R, LOW);
//    }
  }
}
//
//  digitalWrite(0, LOW);
//  digitalWrite(1, HIGH);
//  delay(rand()%200+10);
//  digitalWrite(1, LOW);
//  digitalWrite(0, HIGH);
//  delay(rand()%200+10);

//delay(rand()%20);
//  digitalWrite(0, LOW);
//  delay(rand()%20);
//  digitalWrite(1, HIGH);
//  delay(rand()%200+10);
//  digitalWrite(1, LOW);
//  delay(rand()%20);
//  digitalWrite(0, HIGH);
//  delay(rand()%200+10);
