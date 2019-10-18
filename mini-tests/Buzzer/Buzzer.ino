//+ve on pin no.8 and -ve on any GND
int Buzzer = 8; 
void setup() {
  Serial.begin (9600);  //define baud rate
  pinMode(Buzzer, OUTPUT); //define output
}

void loop() {

  digitalWrite(Buzzer,HIGH); //activate buzzer
  delay(100); 
  digitalWrite(Buzzer,LOW); //deactivate buzzer
  delay(100); 
}
