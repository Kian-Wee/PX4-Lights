#include "FastLED.h"
#define NUM_LEDS 25
CRGB leds[NUM_LEDS];

const int L=0; //Left LED
const int R=1; //Right LED
const int S=2; //Speaker
#define LED_PIN 8 //WS2812B LED DIGITAL PIN 8
String incomingByte;
bool AlarmState=0;
String message;
bool debug=1;
//int batteryarr[]={1,2,3,4,5,6,7,8,9,10};
int batterylevelint = 0;
String FlightState;
int brightness=5;

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
  pinMode(LED_PIN, OUTPUT);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  //Serial.setTimeout(200); //Currently Serial.readString waits for 200ms, might rewrite in the future to allow terminator \n
  Serial.begin(115200);
  Serial.println("Initalising Program");
}

void lightall(CRGB color){
  fill_solid( leds, NUM_LEDS, color);
  FastLED.show();
}

void batterydisplay(int batterylevel){
  for(int i=0; i < batterylevel; i++){
    if(i<5){
      leds[15+i]=CRGB::Red;
    }else{
      leds[15+i]=CRGB::Green;
    }
  }
  FastLED.show();
  
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

    FlightState=String(incomingByte[0]);

//    if (String(incomingByte[0]) == "H") {
//      AlarmState==1;
//    }else if (String(incomingByte[0]) == "L") {
//      AlarmState=0;
//    }

    String batterylevel = String(incomingByte);
    batterylevel.remove(0,1); //Remove flight mode to leave string with batterylevel
    batterylevelint = batterylevel.toInt()/10;
    batterydisplay(batterylevelint); //Send the number of LEDs(1-10) to light up
  }
}

//Function to Store State of the previous flightmode so that the LED animations can be contionusly driven with delays
void flightmode(){
  extern String FlightState;
  if(FlightState=="H"){
      printto("Alarm On");
      if (millis()>Alarm.low){ //Sequence has finished
        Alarm.high=rand()%200+10+millis(); // Regenerate new timing variables
        Alarm.low=rand()%200+10+Alarm.high;
      }else if (millis()<Alarm.high){
        digitalWrite(L, LOW);
        digitalWrite(R, HIGH);
        digitalWrite(S, HIGH);
        for(int i=0; i++; i<15) leds[i]=CRGB::Red;
        FastLED.show();
      }else if (millis()<Alarm.low){
        digitalWrite(L, LOW);
        digitalWrite(R, HIGH);
        digitalWrite(S, LOW);

//  }else if(FlightState="L"){
//      printto("Alarm Off");
//      digitalWrite(L, LOW);
//      digitalWrite(R, LOW);
//      digitalWrite(S, LOW);
  }else if(FlightState=="R"){
      printto("Scout mode");
      digitalWrite(L, HIGH);
      digitalWrite(R, HIGH);
      for(int i=0; i++; i<15) leds[i]=CRGB::Pink;
      FastLED.show();
  }else if(FlightState=="O"){
      printto("Offboard mode");
      digitalWrite(L, LOW);
      digitalWrite(R, LOW);
      digitalWrite(S, LOW);
      for(int i=0; i++; i<15) leds[i]=CRGB::Purple;
      FastLED.show();
  }else if(FlightState=="S"){
      printto("Stabalize mode");
      digitalWrite(L, LOW);
      digitalWrite(R, LOW);
      digitalWrite(S, LOW);
      for(int i=0; i++; i<15) leds[i]=CRGB::Blue;
      FastLED.show();
  }
}
}

void test(){

  Serial.println("Testing every LED");
  FastLED.clear();
  for(int i=0;i<NUM_LEDS; i++){
    leds[i]=CRGB::Red;
    FastLED.show();
    delay(50);
  }
  delay(2000);
  FastLED.clear();
  
  Serial.println("Testing Battery");
  // Run through the 10 LEDS assigned to battery
  for(int i=0; i<11; i++){
    Serial.print("Displaying battery at ");
    Serial.println(i*10);
    batterydisplay(i);
    delay(50);
  }
  delay(2000);
  FastLED.clear();

  extern String FlightState;
  
  FlightState="H";
  flightmode();
  delay(2000);
  FastLED.clear();
  
  FlightState="R";
  Serial.print("test ");
  Serial.println(FlightState);
  flightmode();
  delay(2000);
  FastLED.clear();

  FlightState="O";
  flightmode();
  delay(2000);
  FastLED.clear();

  FlightState="S";
  flightmode();
  delay(2000);
  FastLED.clear();
}

void loop() {
//  readSerial();
//  flightmode();
  test();
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
