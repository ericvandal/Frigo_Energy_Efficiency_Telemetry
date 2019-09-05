#include <ESP8266WiFi.h>
#include <SimpleDHT.h>

// for DHT22, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: D2  = GPIO4
int pinDHT11 = 4;  //D2 = GPIO4
SimpleDHT11 dht11(pinDHT11);

// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "APIKEY";
const char* ssid = "SSID"; //ssid name
const char* password = "MOTDEPASSE";//ssid password
const char* server = "api.thingspeak.com";

WiFiClient client;

void setup() {
  Serial.begin(115200);

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
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT22...");
  
  // read without samples.
  // @remark We use read2 to get a float data, such as 10.1*C
  //    if user doesn't care about the accurate data, use read to get a byte data, such as 10*C.
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(2000);
    delay(2500);
    return;
  }
  
  Serial.print("Sample OK: ");
  Serial.print((float)temperature); Serial.print(" *C, ");
  Serial.print((float)humidity); Serial.println(" RH%");
  
  // DHT11 sampling rate is 0.5HZ.
  //delay(1500);

  if (client.connect(server,80)) {
  String postStr = apiKey;
  postStr +="&field1=";
  postStr += String(temperature);
  postStr +="&field2=";
  postStr += String(humidity);
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
