#include <Servo.h>
int marker = 12;
Servo pan;
Servo tilt;
Servo loader;


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
    int panCom = Serial.parseInt();
    int tiltCom = Serial.parseInt();
    Serial.println(panCom);
    load();
    aim(panCom, tiltCom);
    delay(500);
    fire();
    delay(500);
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
  loader.write(20);
  delay(1000);
  loader.write(0);
}



