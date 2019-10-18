#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int incomingByte = 0;   // for incoming serial data

void setup() {
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}



void loop() {
for (pos = 0; pos <= 180; pos += 1) { // aller de 0 a 180 degée
// in steps of 1 degree
myservo.write(pos); 
delay(10);
}
for (pos = 180; pos >= 0; pos -= 1) { // aller de 0 a 180 degée
// in steps of 1 degree
myservo.write(pos); 
delay(10);
}
  
} 
