const int L=0; //Left LED
const int R=1; //Right LED
const int S=2; //Speaker
const int LED=3; //WS2812B LED DIGITAL PIN
int incomingByte;
int previousByte;
int tLOW=0; //timer variable
int tHIGH=0;
int tLED=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(L, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(S, OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
}

void loop() {
   if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    previousByte = incomingByte;
    previousByte[0];
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'H') {
      digitalWrite(0, LOW);
      digitalWrite(1, HIGH);
      delay(rand()%200+10);
      digitalWrite(1, LOW);
      digitalWrite(0, HIGH);
      delay(rand()%200+10);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'L') {
      digitalWrite(L, LOW);
      digitalWrite(R, LOW);
    }
  }else{
     if (previousByte == 'H') {
        digitalWrite(0, LOW);
        digitalWrite(1, HIGH);
        delay(rand()%200+10);
        digitalWrite(1, LOW);
        digitalWrite(0, HIGH);
        delay(rand()%200+10);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (previousByte == 'L') {
      digitalWrite(L, LOW);
      digitalWrite(R, LOW);
    }
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
