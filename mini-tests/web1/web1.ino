#include <Servo.h>
#include <LiquidCrystal.h>

Servo myservo;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

int pos = 0; // la position initiale du servo moteur
const int trigPin = 2;
const int echoPin = 3;
const int motor = 4;
const int buzzer = 5;
const int ledPin1 = 14;
const int ledPin2 = 15;
float distanceCm, DistanceSec,duration;

void setup() {
  myservo.attach(motor); // attache le Servo moteur a la pin numéro 11
  lcd.begin(16,2); // Initialiser l'interface de Lcd avec leurs Dimensions
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  DistanceSec=20;
  Serial.begin(9600);

}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // aller de 0 a 180 degée
    // in steps of 1 degree
    myservo.write(pos); // Programmer le Servo pour aller a la position (pos)
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); //envoyer une impulsion de 10 micro seconds
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    distanceCm= duration*0.034/2;
    if (distanceCm <= DistanceSec){
      if(distanceCm <= DistanceSec/2){
        Serial.println("Distance: " + String(distanceCm) + " cm");
        Serial.println("Angle: " + String(pos) + " deg");
        delay(10);
      }else{
        Serial.println("Distance: " + String(distanceCm) + " cm");
        Serial.println("Angle: " + String(pos) + " deg");
        delay(10);
      }
    }
 
    Serial.println("Distance: " + String(distanceCm) + " cm");
    Serial.println("Angle: " + String(pos) + " deg");
    delay(10); //attendre 100ms pour que le servo cherche sa position
  
  }
  for (pos = 180; pos >= 0; pos -= 1) { //
    myservo.write(pos); //
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    distanceCm= duration*0.034/2;
    if (distanceCm <= DistanceSec){
      if(distanceCm <= DistanceSec/2){
       
        
        Serial.println("Distance: " + String(distanceCm) + " cm");
        Serial.println("Angle: " + String(pos) + " deg");
        delay(10);
      }else{
        
        Serial.println("Distance: " + String(distanceCm) + " cm");
        Serial.println("Angle: " + String(pos) + " deg");
        delay(10);
      }
    }
 
    Serial.println("Distance: " + String(distanceCm) + " cm");
    Serial.println("Angle: " + String(pos) + " deg");
    delay(10);
  }
}
