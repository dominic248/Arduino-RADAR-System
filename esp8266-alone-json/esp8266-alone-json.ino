#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
#include <Wire.h> 
#include <Servo.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Servo myservo;
int pos = 15; 

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
  delay(1000); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  if(turnon==1){
    display.println("TURNED ON");
  }else if(turnon==0){
    display.println("TURNED OFF");
  }
  display.display(); 
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
  }else if(turnon==0){
      display.clearDisplay();
      display.setTextColor(WHITE);
      display.setCursor(0,10);
      display.println("TURNED OFF");
      display.display();
    
  }
}

void mode0(){
  for (pos = 0; pos <= 180; pos += 1) {
    task(pos);
    buttonCheck(); 
    if((turnon==1 && switching==1) || turnon==0){
      break;
    } 
  }
  for (pos = 180; pos >= 0; pos -= 1) { 
    task(pos);
    buttonCheck(); 
    if((turnon==1 && switching==1) || turnon==0){
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
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0,10);
    if(turnon==1){
      display.println("TURNED ON");
      if(switching==0){
        display.println("180 deg rotation mode:");
      }else if(switching==1){
        display.println("90 deg static mode:"); 
      }
      display.println();
      display.println("Distance: " + String(distanceCm) + " cm");
      display.println("Angle: " + String(posit) + " deg");
    }else{
      display.println("TURNED OFF");
    }
    display.display();
    delay(30);
    if (distanceCm < 25 && turnon==1){
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
