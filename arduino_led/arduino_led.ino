#include <LiquidCrystal.h>

//TODO, CHANGE BUZZER TO NOT INTERRUPTING

//#include "FastLED.h"
//#define NUM_LEDS 25
//CRGB leds[NUM_LEDS];

const int L=3; //Left LED
const int R=A0; //Right LED
const int LB=0; //Left LED Bottom (UNUSED)
const int RB=A3; //Right LED Bottom (UNUSED)
const int S=A1; //Speaker
#define LED_PIN 8 //WS2812B LED DIGITAL PIN 8
#define ONBOARD_LED 10 //13 for arduino mega

bool debug=0;

bool AlarmState=0;
int batterylevelint = 0;
String FlightState;
String prevFlightState;
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
  pinMode(L, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(S, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ONBOARD_LED, OUTPUT);

//  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
//  FastLED.setBrightness(brightness);
  //Serial.setTimeout(200); //Currently Serial.readString waits for 200ms, might rewrite in the future to allow terminator \n

  Serial.begin(115200);
  printto("Initalising Program");
  FlightState="H";
}

// void lightall(CRGB color){
//   fill_solid(leds, NUM_LEDS, color);
//   FastLED.show();
// }

// Use LEDs 15 to 25 where 15 to 20 is 50% to 100%, 21-25 is 0% to 50%
// void batterydisplay(int batterylevel){
//   for(int i=0 ; i < batterylevel; i++){
//     if(i<5){
//       leds[20+i]=CRGB::Red; //0->5,21->25
//     }else{
//       leds[10+i]=CRGB::Green; //5->10,15->20
//     }
//   }
// }

void printto(String text){
  if(debug==1){
    Serial.println(text);
  }
}

void readSerial(){
  if (Serial.available() > 0) {
    
    incomingByte = Serial.readStringUntil('\n');
    printto("Reading String: " + incomingByte); //DEBUG

    FlightState=String(incomingByte[0]);
    printto("Flight mode is:" + FlightState);
//
////    if (String(incomingByte[0]) == "H") {
////      AlarmState==1;
////    }else if (String(incomingByte[0]) == "L") {
////      AlarmState=0;
////    }
//
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
    digitalWrite(L, LOW);
    digitalWrite(R, HIGH);
    digitalWrite(S, HIGH);
    delay(rand()%200+10);
    digitalWrite(R, LOW);
    digitalWrite(S, LOW);
    digitalWrite(L, HIGH);
    delay(rand()%200+10);
  
  }else if(FlightState != prevFlightState){

    if(FlightState=="R"){
        printto("Scout mode");
        digitalWrite(L, HIGH);
        digitalWrite(R, HIGH);
        // for(int i=0; i<15; i++) leds[i]=CRGB::Pink;
    }else if(FlightState=="O"){
        printto("Offboard mode");
        digitalWrite(L, LOW);
        digitalWrite(R, LOW);
        digitalWrite(S, LOW);
        // for(int i=0; i<15; i++) leds[i]=CRGB::Purple;
    }else if(FlightState=="S"){
        printto("Stabalize mode");
        digitalWrite(L, LOW);
        digitalWrite(R, LOW);
        digitalWrite(S, LOW);
        // for(int i=0; i<15; i++) leds[i]=CRGB::Blue;
  }else if(FlightState=="D"){
        printto("Disarmed");
        digitalWrite(L, LOW);
        digitalWrite(R, LOW);
        digitalWrite(S, LOW);
  }
  prevFlightState=FlightState;
  }else{
      digitalWrite(L, LOW);
      digitalWrite(R, LOW);
      digitalWrite(S, LOW);
  }
  
  
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



void loop() {
// printto("looping");/
 readSerial();
 flightmode();
//  batterydisplay(batterylevelint); //Send the number of LEDs(1-10) to light up
  
  // FastLED.show();
//  delay(500); // To prevent microcontroller from restarting due to fastled.show
 
  // test();

  


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

    
    
}



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
