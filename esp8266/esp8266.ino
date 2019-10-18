#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;

const char* ssid = "Skynet"; //Your Network SSID
const char* password = "24081999"; //Your Network Password

//
const char* IFTTT_URL="maker.ifttt.com";
const char* IFTTT_KEY="c_QxEnZGw7ieOc28-jWDIR";
const char* IFTTT_EVENT="Alert!, Danger!";
//

int detect=0;

WiFiClient client;
unsigned long myChannelNumber = 858265; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "23NJGFJ3EP1MPUWB"; //Your Write API Key

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
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
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() { // run over and over
  if(!client.connect(IFTTT_URL,80)){
    Serial.println("Connection Failed!");
    return;
  }
  if (Serial.available()>0) {
    String content=Serial.readStringUntil('\n');
//    String Spos = getValue(content, ',', 0);
//    String Sdistance = getValue(content, ',', 1);
//    int pos=Spos.toInt();
//    int distance=Sdistance.toInt();
    int distance=content.toInt();
    Serial.println(content);
//    Serial.println(pos);
//    Serial.println(distance);
    if(distance<25){
      if(detect==0){
        detect=1;
        String url="https://maker.ifttt.com/trigger/detected/with/key/f_IoiRr86d6nXqRUS603FDpc9vv993aG-bjLP9Yyxyu";
        client.print(String("POST ") + url + " HTTP/1.1\r\n" +
             "Host: " + IFTTT_URL + "\r\n" + 
             "Connection: close\r\n\r\n");
//        String url="https://maker.ifttt.com/trigger/detected/with/key/f_IoiRr86d6nXqRUS603FDpc9vv993aG-bjLP9Yyxyu";
//        client.print(String("POST ") + url + " HTTP/1.1\r\n" +
//             "Host: " + IFTTT_URL + "\r\n" + 
//             "Content-Type: application/json \r\n" +
//             "Content-Length: " + object1.measureLength() + "\r\n");
//        object1.printTo(client);
//        client.println("Connection: close\r\n\r\n");
      }
    }else{
      detect=0;
    }
    //ThingSpeak.writeField(myChannelNumber, 1,content, myWriteAPIKey); 
  }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
