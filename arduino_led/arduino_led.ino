//TODO, CHANGE BUZZER TO NOT INTERRUPTING

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

struct timer
{
  public:
    unsigned long high;
    unsigned long low;
};

timer Alarm = {
  0,0
};

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

//   if (String(incomingByte[1]) == "S"){
//    FlightState=2;
//   }

    String batterylevel = String(incomingByte);
    batterylevel.remove(0,2); //Remove flight mode to leave string with batterylevel
    batterylevelint = batterylevel.toInt();
    printto("Battery level is: "+String(batterylevelint));
  }

}

//Function to Store State of the previous flightmode so that the LED animations can be contionusly driven with delays
void flightmode(){
  extern String FlightState;
  extern String prevFlightState;
  
  if(FlightState=="H"){
    printto("Alarm On");
    // if (millis()>Alarm.low){ //Sequence has finished
    //   Alarm.high=rand()%200+10+millis(); // Regenerate new timing variables
    //   Alarm.low=rand()%200+10+Alarm.high;
    // }else if (millis()<Alarm.high){
    //   digitalWrite(L, LOW);
    //   digitalWrite(R, HIGH);
    //   digitalWrite(S, HIGH);
    //   for(int i=0; i<15; i++) leds[i]=CRGB::Red;
    // }else if (millis()<Alarm.low){
    //   digitalWrite(L, LOW);
    //   digitalWrite(R, HIGH);
    //   digitalWrite(S, LOW);
    // }
    lightup(CRGB::Yellow,0);
    FastLED.show();
    
    digitalWrite(LB, LOW);
    digitalWrite(L, LOW);
    digitalWrite(RB, HIGH);
    digitalWrite(R, HIGH);
    digitalWrite(S, HIGH);
    delay(rand()%200+10);
    digitalWrite(RB, LOW);
    digitalWrite(R, LOW);
    digitalWrite(LB, HIGH);
    digitalWrite(L, HIGH);
    digitalWrite(S, LOW);
    delay(rand()%200+10);
    
  
  }else if(FlightState != prevFlightState){

    if(FlightState=="R"){
        printto("Scout mode");
        digitalWrite(LB, HIGH);
        digitalWrite(L, HIGH);
        digitalWrite(RB, HIGH);
        digitalWrite(R, HIGH);
        lightup(CRGB::Orange, 0);
    }else if(FlightState=="O"){
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
  // This is only used when mavros is not turned on and the alarm needs to be off
  }else if(FlightState=="L"){
        printto("Alarm off");
        digitalWrite(LB, LOW);
        digitalWrite(L, LOW);
        digitalWrite(RB, LOW);
        digitalWrite(R, LOW);
        digitalWrite(S, LOW);
        lightup(CRGB::Black, 0);
  }
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


void plannermode(){
  if(PlannerState != prevPlannerState){

    if(PlannerState=="F"){
        lightup(CRGB::Yellow, 1);
    }else if(PlannerState=="A"){
        lightup(CRGB::MediumOrchid, 1);
    }else if(PlannerState=="M"){
        lightup(CRGB::Green, 1);
    }else if(PlannerState=="I"){
        lightup(CRGB::Black, 1);
  }
  prevPlannerState=PlannerState;
  }else{
      lightup(CRGB::Black, 1);
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
 plannermode();
 FastLED.show();
 delay(1000);

//batterydisplay(batterylevelint); //Send the number of LEDs(1-10) to light up

 //delay(500); // To prevent microcontroller from restarting due to fastled.show

//   if (Serial.available() > 0) {
//    // read the oldest byte in the serial buffer:
//    incomingByte = Serial.readStringUntil('\n');
////    previousByte = incomingByte;
//    // if it's a capital H (ASCII 72), turn on the LED:
//      if(incomingByte== "S5"){
//        Serial.println("HIGH");
//        digitalWrite(10,HIGH);
//        fill_solid(leds, NUM_LEDS, CRGB::Red);
//        FastLED.show();
//      }
//      if(incomingByte== "R10"){
//        Serial.println("LOW");
//        digitalWrite(10,LOW);
//        fill_solid(leds, NUM_LEDS, CRGB::White);
//        FastLED.show();
//      }
//      Serial.print("I received: ");
//      Serial.println(incomingByte);
//   }

//  uint16_t timerA = 500;  // How often to run Event A [milliseconds]
//  uint16_t timerB = 500;  // How long after A to run Event B [milliseconds]
//
//
//  // Setting the amount of time for "triggerTimer".
//  // You can name "triggerTimer" whatever you want.
//  static CEveryNMilliseconds triggerTimer(timerB);
//  
//  EVERY_N_MILLISECONDS(timerA){
//    // do Event A stuff
//    fill_solid(leds, NUM_LEDS, CHSV(random8(),255,128));
//    counterTriggered = 1;  // Set to True
//    triggerTimer.reset();  // Start trigger timer
//  }
//  
//  if (counterTriggered == 1) {  // Will only be True if Event A has started
//    if (triggerTimer) {  // Check if triggerTimer time reached
//      // do Event B stuff
//      for (uint8_t i=0; i<NUM_LEDS/2; i++){
//        leds[random8(NUM_LEDS)] = CRGB::Red;
//      }
//      counterTriggered = 0;  // Set back to False
//    }
//  }
//
//
//  EVERY_N_SECONDS_I( timingObj, 20) {
//    // This initally defaults to 20 seconds, but then will change the run
//    // period to a new random number of seconds from 10 and 30 seconds.
//    // You can name "timingObj" whatever you want.
//    timingObj.setPeriod( random8(10,31) );
//    FastLED.clear();
//    for (uint16_t i=0; i<NUM_LEDS*3; i++){
//      leds[random8(NUM_LEDS)] = CRGB::Black;
//      leds[random8(NUM_LEDS)] = CHSV(random8(), random8(140,255), random8(50,255));
//      FastLED.show();
//      delay(random8(20,80));
//    }
//    delay(500);
//  }

    
}

// void test(){

//   Serial.println("Testing every LED");
//   FastLED.clear();
//   for(int i=0;i<NUM_LEDS; i++){
//     leds[i]=CRGB::Red;
//     FastLED.show();
//     delay(50);
//   }
//   delay(2000);
//   FastLED.clear();
  
//   Serial.println("Testing Battery");
//   // Run through the 10 LEDS assigned to battery
//   for(int i=0; i<11; i++){
//     Serial.print("Displaying battery at ");
//     Serial.println(i*10);
//     batterydisplay(i);
//     FastLED.show();
//     delay(50);
//   }
//   delay(2000);
//   FastLED.clear();

//   extern String FlightState;
//   Serial.println("Testing Flight Modes");

//   FlightState="H";
//   Serial.print("test ");
//   Serial.println(FlightState);
//   flightmode();
//   batterydisplay(10);
//   FastLED.show();
//   delay(1000);
//   FastLED.clear();
  
//   FlightState="R";
//   Serial.print("test ");
//   Serial.println(FlightState);
//   flightmode();
//   batterydisplay(5);
//   FastLED.show();
//   delay(1000);
//   FastLED.clear();

//   FlightState="O";
//   Serial.print("test ");
//   Serial.println(FlightState);
//   flightmode();
//   batterydisplay(10);
//   FastLED.show();
//   delay(1000);
//   FastLED.clear();

//   FlightState="S";
//   Serial.print("test ");
//   Serial.println(FlightState);      
//   flightmode();
//   batterydisplay(5);
//   FastLED.show();
//   delay(1000);

// }
