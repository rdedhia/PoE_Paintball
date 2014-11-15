#include <Servo.h>
int marker = 12;
Servo pan;
Servo tilt;
Servo loader;
String incoming = "";
String panString = "";
String tiltString = "";
int panCom = 0;
int tiltCom = 0;

void setup() {
  // initialze pins as outputs
  pinMode(marker, OUTPUT);
  // setup the servos
  pan.attach(5);
  tilt.attach(6);
  loader.attach(3);
  // setup Serial connection
  Serial.begin(9600);
  pan.write(0);
  tilt.write(0);
  loader.write(0);
}



// the loop routine runs over and over again forever:
void loop() {
  if (Serial.available() > 0) { //check for new data
    incoming = Serial.readString();
    panString = incoming.substring(0,3);
    tiltString = incoming.substring(4,7);
    panCom = panString.toInt();
    tiltCom = tiltString.toInt();
    load();
    aim(panCom, tiltCom);
    delay(500);
    fire();
  }
}

void fire() { //electrically pulls the trigger on the marker
  digitalWrite(marker, HIGH);
  delay(2);
  digitalWrite(marker, LOW);
}

void aim(int x,int y) { //moves the gimbal to the correct x and y positions
  pan.write(x);
  tilt.write(y);
}

void load() { //load a paintball in the barrel
  loader.write(80);
  delay(200);
  loader.write(200);
  delay(200);
}



