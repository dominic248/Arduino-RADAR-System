#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;


#include <Wire.h> 
#include <Servo.h>
Servo myservo;
int pos = 15; 
const int trigPin = 16; //D0
const int echoPin = 5; //D1
const int motor = 2; //D4
const int buzzer = 0; //D3

int distanceCm, DistanceSec,duration;


const char* ssid = "Skynet"; //Your Network SSID
const char* password = "24081999"; //Your Network Password

const char* IFTTT_URL="maker.ifttt.com";
const char* IFTTT_KEY="c_QxEnZGw7ieOc28-jWDIR";
const char* IFTTT_EVENT="Alert!, Danger!";

int detect=0;

WiFiClient client;
unsigned long myChannelNumber = 873261; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "ME5W5ZZ5Y5USLMYO"; //Your Write API Key

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  myservo.attach(motor);
  pinMode(trigPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motor, OUTPUT);
  DistanceSec=20;
  delay(1000); 
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected...");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
  

}

void loop() { // run over and over
  if(!client.connect(IFTTT_URL,80)){
    Serial.println("Connection Failed!");
    return;
  }
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos); 
    distanceCm = calculateDistance();
    Serial.println(String(pos) + "," + String(distanceCm)+".");
    delay(30);
    if (distanceCm < 25){
      triggerdetect();
      digitalWrite(buzzer, HIGH);
    } else {
      detect=0;
      digitalWrite(buzzer, LOW);
    } 
//    Serial.println(detect);
  }
  for (pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(pos);
    distanceCm = calculateDistance();
    Serial.println(String(pos) + "," + String(distanceCm)+".");
    delay(30);
    if (distanceCm < 25){
      triggerdetect();
      digitalWrite(buzzer, HIGH);
    } else {
      detect=0;
      digitalWrite(buzzer, LOW);
    } 
//     Serial.println(detect);
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
void triggerdetect(){
  if(detect==0){
    detect=1;
    String url="https://maker.ifttt.com/trigger/detected/with/key/f_IoiRr86d6nXqRUS603FDpc9vv993aG-bjLP9Yyxyu";
    client.print(String("POST ") + url + " HTTP/1.1\r\n" +
             "Host: " + IFTTT_URL + "\r\n" + 
             "Connection: close\r\n\r\n");
    ThingSpeak.setField(1, distanceCm);
    ThingSpeak.setField(2, pos);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    }
}
