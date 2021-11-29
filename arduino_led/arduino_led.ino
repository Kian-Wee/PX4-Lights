const int L=0; //Left LED and speaker
const int R=1; //Right LED
int incomingByte;
int previousByte;

void setup() {
  // put your setup code here, to run once:
  pinMode(L, OUTPUT);
  pinMode(R, OUTPUT);
  Serial.begin(9600);
}

void loop() {
   if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    previousByte = incomingByte;
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
