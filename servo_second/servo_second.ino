#include "Servo.h"

Servo loadServo;
int pos;
String incoming = "";

void setup() {
  Serial.begin(9600);
  loadServo.attach(9);
  loadServo.write(0);
}

void loop() {
  if (Serial.available() > 0) { //check for new data
    incoming = Serial.readString();
    load();
  }
}


void load() 
{ 
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    loadServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(10);                       // waits 15ms for the servo to reach the position 
  }
  for(pos = 180; pos>= 0; pos -= 1)     // goes from 180 degrees to 0 degrees 
  {                                
    loadServo.write(pos);               // tell servo to go to position in variable 'pos' 
    delay(10);                       // waits 15ms for the servo to reach the position 
  }
  delay(200);
}

