#include "src/FastLED/src/FastLED.h"
//#include "FastLED.h"
#define NUM_LEDS 4
CRGB leds[NUM_LEDS];

const int L=3; //Left LED
const int S=A0; //Right LED
const int LB=0; //Left LED Bottom (UNUSED)
const int RB=A3; //Right LED Bottom (UNUSED)
const int R=A1; //Speaker
#define LED_PIN 7 //WS2812B LED DIGITAL PIN
#define ONBOARD_LED 10 //13 for arduino mega

bool debug=0;

bool AlarmState=0;
int batterylevelint = 0;
String FlightState;
String prevFlightState;
String PlannerState;
String prevPlannerState;
int brightness=5;

int tLOW=0; //timer variable
int tHIGH=0;
int tLED=0;
int tcur=0;

String incomingByte; //Using .readstring currently, use int(ascii) for .read

void setup() {
  pinMode(LB, OUTPUT);
  pinMode(RB, OUTPUT);
  pinMode(L, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(S, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ONBOARD_LED, OUTPUT);

  FastLED.addLeds<WS2812B, LED_PIN, BRG>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  //Serial.setTimeout(200); //Currently Serial.readString waits for 200ms, might rewrite in the future to allow terminator \n

  Serial.begin(115200);
  printto("Initalising Program");

  lightall(CRGB::Black);
  FastLED.show();
}

void lightall(CRGB color){
   fill_solid(leds, NUM_LEDS, color);
   FastLED.show();
}

// Use LEDs 15 to 25 where 15 to 20 is 50% to 100%, 21-25 is 0% to 50%
void batterydisplay(int batterylevel){
  for(int i=0 ; i < batterylevel; i++){
    if(i<5){
      leds[20+i]=CRGB::Red; //0->5,21->25
    }else{
      leds[10+i]=CRGB::Green; //5->10,15->20
    }
  }
}

void printto(String text){
  if(debug==1){
    Serial.println(text);
  }
}

void readSerial(){
  if (Serial.available() > 0) {
    
    incomingByte = Serial.readStringUntil('\n');
    printto("Reading String: " + incomingByte);

    FlightState=String(incomingByte[0]);
    printto("Flight mode is:" + FlightState);

    PlannerState=String(incomingByte[1]);
    printto("Planner mode is:" + PlannerState);

   if (String(incomingByte[0]) == "H") {
     AlarmState=1;
   }else if (String(incomingByte[0]) == "L") {
     AlarmState=0;
   }

    String batterylevel = String(incomingByte);
    batterylevel.remove(0,1); //Remove flight mode to leave string with batterylevel
    batterylevelint = batterylevel.toInt();
    printto("Battery level is: "+String(batterylevelint));
  }

}

//Function to Store State of the previous flightmode so that the LED animations can be contionusly driven with delays
void flightmode(){
  extern String FlightState;
  extern String prevFlightState;
  
  if(FlightState != prevFlightState){

    if(FlightState=="O"){
        printto("Offboard mode");
        digitalWrite(LB, LOW);
        digitalWrite(L, LOW);
        digitalWrite(RB, LOW);
        digitalWrite(R, LOW);
        digitalWrite(S, LOW);
        lightup(CRGB::Purple, 0);
    }else if(FlightState=="S"){
        printto("Stabalize mode");
        digitalWrite(LB, LOW);
        digitalWrite(L, LOW);
        digitalWrite(RB, LOW);
        digitalWrite(R, LOW);
        digitalWrite(S, LOW);
        lightup(CRGB::Blue, 0);
  }else if(FlightState=="D"){
        printto("Disarmed");
        digitalWrite(LB, LOW);
        digitalWrite(L, LOW);
        digitalWrite(RB, LOW);
        digitalWrite(R, LOW);
        digitalWrite(S, LOW);
        lightup(CRGB::Black, 0);
  prevFlightState=FlightState;
  }else{
      digitalWrite(LB, LOW);
      digitalWrite(L, LOW);
      digitalWrite(RB, LOW);
      digitalWrite(R, LOW);
      digitalWrite(S, LOW);
      lightup(CRGB::Black, 0);
  }
  
}
}

void lightup(CRGB color, bool mode){
  for(int i=0; i<NUM_LEDS; i++){
    if (i % 2 == 1 && mode == 1) {
      leds[i]=color;
    }else if (i % 2 == 0 && mode == 0) {
      leds[i]=color;
    
  }
  }
}

void loop() {

 printto("looping");
 readSerial();
 flightmode();
 FastLED.show();
 delay(100); // To prevent microcontroller from restarting due to fastled.show

  //batterydisplay(batterylevelint); //Send the number of LEDs(1-10) to light up
}