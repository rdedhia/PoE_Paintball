#include "Servo.h"

Servo loadServo;
int pos;
int pos_min;
int pos_max;
String incoming = "";
int color; // 0 is blue, 1 is orange, 2 is yellow

void setup() {
  Serial.begin(9600);
  loadServo.attach(9);
  loadServo.write(0);
}

void loop() {
  if (Serial.available() > 0) { //check for new data
    incoming = Serial.readString();
    color = incoming.toInt();
    load(color);
  }
}

void load(int color)
{ 
  if (color == 0) {
    pos_min = 0;
    pos_max = 90;  
  } else if (color == 1) {
    pos_min = 90;
    pos_max = 180;
  } else {
    pos_min = 180;
    pos_max = 270;
  }  
  for(pos = pos_min; pos <= pos_max; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    loadServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(10);                       // waits 15ms for the servo to reach the position 
  }
  for(pos = pos_max; pos >= pos_min; pos -= 1)     // goes from 180 degrees to 0 degrees 
  {                                
    loadServo.write(pos);               // tell servo to go to position in variable 'pos' 
    delay(10);                       // waits 15ms for the servo to reach the position 
  }
}

