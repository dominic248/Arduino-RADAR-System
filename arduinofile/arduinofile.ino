#include <SoftwareSerial.h>
#include <Wire.h> 
#include <Servo.h>
//#include <LiquidCrystal.h>

SoftwareSerial Serial1(10,11);//RX,TX
Servo myservo;
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2); 

int pos = 15; 
const int trigPin = 2;
const int echoPin = 3;
const int motor = 4;
const int buzzer = 5;
const int ledPin1 = 14;
const int ledPin2 = 15;
int distanceCm, DistanceSec,duration;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  myservo.attach(motor);
//  lcd.begin(16,2);
  pinMode(trigPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motor, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  DistanceSec=20;
  delay(1000); 
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos); 
//    delay(80);
    distanceCm = calculateDistance();
    Serial.println(String(pos) + "," + String(distanceCm)+".");
//    Serial1.println(distanceCm);
//    Serial1.println(String(pos) + "," + String(distanceCm));
delay(30);
    if (distanceCm < 25){
      digitalWrite(buzzer, HIGH);
    } else {
      digitalWrite(buzzer, LOW);
    } 
  }
  for (pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(pos);
//    delay(80);
    distanceCm = calculateDistance();
    Serial.println(String(pos) + "," + String(distanceCm)+".");
//    Serial1.println(distanceCm);
//    Serial1.println(String(pos) + "," + String(distanceCm));
delay(30);
    if (distanceCm < 25){
      digitalWrite(buzzer, HIGH);
    } else {
      digitalWrite(buzzer, LOW);
    } 
  }
}

int calculateDistance(){ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm= duration*0.034/2;
  return distanceCm;
}
