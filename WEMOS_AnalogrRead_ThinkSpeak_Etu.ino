#include <ESP8266WiFi.h>
//#include <SimpleDHT.h>
/*
  ReadAnalogVoltage and send Voltage to ThingSpeak Cloud
  Controller WEMOS D1 mini
  The ESP8266EX(Which IC D1 board used)  integrates a generic purpose 10-bit analog ADC. The ADC range is from 0V to 1.0V
  And on the  board, we use following circuit:

  -----A0
  |
 220K
  |--- ADC
 100K
  |
 GND

so,the input voltage(A0) can be 3.2V, and the A0=3.2*ADC

 */

// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "Votre API KEY";
const char* ssid = "Reine"; //ssid name
const char* password = "1234567890";//ssid password
const char* server = "api.thingspeak.com";

WiFiClient client;

void setup() {
    // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  pinMode(BUILTIN_LED, OUTPUT);  // initialize onboard LED as output
  delay(10);
  
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
}

void loop() {

   // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 3.2V):
  float voltage = sensorValue * (3.29 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);

  digitalWrite(BUILTIN_LED, LOW);  // turn on LED with voltage HIGH
  delay(100);                      // wait one second
  digitalWrite(BUILTIN_LED, HIGH);   // turn off LED with voltage LOW
  delay(100);                      // wait one second

  if (client.connect(server,80)) {
  String postStr = apiKey;
  postStr +="&field1=";
  postStr += String(voltage);
  postStr += "\r\n\r\n";
 
  client.print("POST /update HTTP/1.1\n");
  client.print("Host: api.thingspeak.com\n");
  client.print("Connection: close\n");
  client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
  client.print("Content-Type: application/x-www-form-urlencoded\n");
  client.print("Content-Length: ");
  client.print(postStr.length());
  client.print("\n\n");
  client.print(postStr);

  Serial.println("Sending data to Thingspeak");
  }
  delay(500);  //Important pour transmettre Avant fermeture
  client.stop();
 
  Serial.println("Waiting 15 secs");
  // thingspeak needs at least a 15 sec delay between updates
  // 15 seconds to be safe
  delay(15000);
}
