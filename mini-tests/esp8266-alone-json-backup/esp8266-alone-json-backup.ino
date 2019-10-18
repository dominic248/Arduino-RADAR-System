#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
#include <ArduinoJson.h>


#include <Wire.h> 
#include <Servo.h>
Servo myservo;
int pos = 15; 
//const int trigPin = 16; //D0
//const int echoPin = 5; //D1
//const int buzzer = 0; //D3
//const int motor = 2; //D4
//const int buttonOn =14; //D5
//const int buttonSwitch =12; //D6

const int trigPin = 14; //D5
const int echoPin = 12; //D6
const int buzzer = 16; //D0
const int motor = 0; //D3
const int buttonOn =13; //D7
const int buttonSwitch =15; //D8
//Vin  3.3V
//GND GND
//SCL GPIO 5 (D1)
//SDA GPIO 4 (D2) 

int distanceCm, DistanceSec,duration;
int buttonStateOn = 0;
int buttonStateSwitch = 0;
int turnon=0,changed=0,switching=0;

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
  pinMode(buttonOn, INPUT);
  pinMode(buttonSwitch, INPUT);
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
  buttonCheck();  
  if(turnon==1 && switching==0){
     mode0();
  }else if(turnon==1 && switching==1){
     mode1();
  }
}

void buttonCheck(){
  buttonStateOn = digitalRead(buttonOn);
  buttonStateSwitch=digitalRead(buttonSwitch);
  if (buttonStateOn == HIGH) {
    changed=1;
    if(turnon==0){
      turnon=1;
    }else{
      turnon=0;
    }
//    Serial.println(turnon);
    delay(1000);
  } else {
    changed=0;
  }
  if (buttonStateSwitch == HIGH) {
    changed=1;
    if(switching==0){
      switching=1;
    }else{
      switching=0;
    }
//    Serial.println(switching);
    delay(1000);
  } else {
    changed=0;
  }
  if(changed==1){
    if(turnon==1 && switching==0){
      mode0();
    }else if(turnon==1 && switching==1){
      mode1();
    }
  }
}

void mode0(){
  for (pos = 0; pos <= 180; pos += 1) {
    task(pos);
    buttonCheck(); 
    if(turnon==1 && switching==1){
      break;
    } 
  }
  for (pos = 180; pos >= 0; pos -= 1) { 
    task(pos);
    buttonCheck(); 
    if(turnon==1 && switching==1){
      break;
    }  
  }
}

void mode1(){
  task(90);
  buttonCheck();  
}


void task(int posit){
  myservo.write(posit); 
    distanceCm = calculateDistance();
    Serial.println(String(posit) + "," + String(distanceCm)+".");
    delay(30);
    if (distanceCm < 25){
      triggerdetect(distanceCm,posit);
      digitalWrite(buzzer, HIGH);
    } else {
      detect=0;
      digitalWrite(buzzer, LOW);
    } 
//    Serial.println(detect);
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

void triggerdetect(int distance, int posit){
  if(detect==0){
    detect=1;
//    String urls="https://maker.ifttt.com/trigger/detect/with/key/f_IoiRr86d6nXqRUS603FDpc9vv993aG-bjLP9Yyxyu";
//        client.print(String("POST ") + urls + " HTTP/1.1\r\n" +
//             "Host: " + IFTTT_URL + "\r\n" + 
//             "Connection: close\r\n\r\n");
             
   String url="https://maker.ifttt.com/trigger/detected/with/key/f_IoiRr86d6nXqRUS603FDpc9vv993aG-bjLP9Yyxyu";
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();
    data["value1"] = distance;
    data["value2"] = posit;
//    Serial.println(distance + "," + posit);
//    data.printTo(Serial);
    client.print("POST ");
    client.print(url);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(IFTTT_URL);
    client.println("Connection: close\r\nContent-Type: application/json");
    client.print("Content-Length: ");
    client.print(data.measureLength());
    client.print("\r\n");
    client.println();
    data.printTo(client);
    ThingSpeak.setField(1, distanceCm);
    ThingSpeak.setField(2, pos);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  }
}
