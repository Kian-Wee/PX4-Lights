#include "FastLED.h"
#define NUM_LEDS 60
CRGB leds[NUM_LEDS];

const int L=0; //Left LED
const int R=1; //Right LED
const int S=2; //Speaker
const int LEDPIN=3; //WS2812B LED DIGITAL PIN
String incomingByte;
bool AlarmState=0;
String message;
bool debug=1;
int batteryarr[] =[1,2,3,4,5,6,7,8,9,10];
int batterylevelint = 0;
String FlightState;

int tLOW=0; //timer variable
int tHIGH=0;
int tLED=0;
int tcur=0;

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

void batterydisplay(int batterylevel){
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

void readSerial(){
  if (Serial.available() > 0) {
    
    //incomingByte = Serial.read();
    //printto("Reading String");
    incomingByte = Serial.readString();
    printto(incomingByte); //DEBUG
    //previousByte = incomingByte;

    FlightState=String(incomingByte[0])

//    if (String(incomingByte[0]) == "H") {
//      AlarmState==1
//    }else if (String(incomingByte[0]) == "L") {
//      AlarmState=0
//    }

    batterylevel = String(incomingByte);
    batterylevel.remove(0,1); //Remove flight mode to leave string with batterylevel
    batterylevelint = int(batterylevel)/10
    batterydisplay(batterylevelint); //Send the number of LEDs(1-10) to light up
  }
}

//Function to Store State of the previous flightmode so that the LED animations can be contionusly driven with delays
void flightmode(){
  if(FlightState=='H'){
      printto("Alarm On");
      if (millis()>Alarm.low): //Sequence has finished
        Alarm.high=rand()%200+10+millis(); // Regenerate new timing variables
        Alarm.low=rand()%200+10+Alarm.high;
      else if (millis()<Alarm.high):
        digitalWrite(L, LOW);
        digitalWrite(R, HIGH);
        digitalWrite(S, HIGH);
      else if (millis()<Alarm.low):
        digitalWrite(L, LOW);
        digitalWrite(R, HIGH);
        digitalWrite(S, LOW);

//  }else if(FlightState='L'){
//      printto("Alarm Off");
//      digitalWrite(L, LOW);
//      digitalWrite(R, LOW);
//      digitalWrite(S, LOW);
  }else if(FlightState='R'){
      printto("Scout mode");
      digitalWrite(L, HIGH);
      digitalWrite(R, HIGH);
  }else if(FlightState='O'){
      printto("Offboard mode");
      digitalWrite(L, LOW);
      digitalWrite(R, LOW);
      digitalWrite(S, LOW);
  }else if(FlightState='S'){
      printto("Stabalize mode");
      digitalWrite(L, LOW);
      digitalWrite(R, LOW);
      digitalWrite(S, LOW);
  }
}

void loop() {
  //lightall(CRGB::Red);
  readSerial();
  flightmode();
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
