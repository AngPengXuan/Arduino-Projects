echo "# projects" >> README.md
git init
git add README.md
git commit -m "first commit"
git remote add origin https://github.com/AngPengXuan/projects.git
git push -u origin master


int analogPin = 1; // noise sensor connected to analog pin
int noiseValue = 0;
int fan1Pin = 6; // fan connected to digital pin
int fan2Pin = 5; // fan connected to digital pin

//this is dust sensor
int PM01Value=0;          //define PM1.0 value of the air detector module
int PM2_5Value=0;         //define PM2.5 value of the air detector module
int PM10Value=0;         //define PM10 value of the air detector module
//this is dust sensor


#include <Arduino.h>
#include "DHT.h"
#include <SPI.h>
#include <WiFi101.h>
#include <ThingSpeak.h>

//Wifi portion
char ssid[] = "AndroidAP";        // your network SSID (name)
char pass[] = "123456789";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status
#define LENG 31   //0x42 + 31 bytes equal to 32 bytes
unsigned char buf[LENG];
//Wifi portion

//This is Temperature and Humidity DHT sensor
#define DHTPIN 2    // what digital pin we're connected to
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
//This is Temperature and Humidity DHT sensor

//This is ThingSpeak
String GET = "GET /update?api_key=N8PV03HUMNXD5KCZ";
#define IP "52.1.229.129"
WiFiClient client;
unsigned long myChannelNumber = 464163;
const char * myWriteAPIKey = "NH1BQOZGK5HSMQEZ";
//This is ThingSpeak

void setup() {

  //This is fan, requires both pins to work
  pinMode(fan1Pin, HIGH);
  pinMode(fan2Pin, HIGH);
  //This is fan, requires both pins to work
  
  Serial1.begin(9600);
  Serial1.setTimeout(1500);  
  Serial.begin(9600);
  ThingSpeak.begin(client);
  dht.begin(); 

  //This is loudness sensor
  noiseValue = analogRead(analogPin);
  Serial.println("this is noise");
  Serial.println(noiseValue);
  //This is loudness sensor

  //This wifi
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  //This is wifi
  Serial.print("You're connected to the network");
}

void loop() {

//This is fan
digitalWrite(fan1Pin, HIGH);
digitalWrite(fan2Pin, HIGH);
//This is fan

  //This is dust sensor
 if(Serial1.find(0x42)){    //start to read when detect 0x42
    Serial1.readBytes(buf,LENG);

    if(buf[0] == 0x4d){
      if(checkValue(buf,LENG)){
        PM01Value=transmitPM01(buf); //count PM1.0 value of the air detector module
        PM2_5Value=transmitPM2_5(buf);//count PM2.5 value of the air detector module
        PM10Value=transmitPM10(buf); //count PM10 value of the air detector module 
      }           
    } 
  }
   //This is dust sensor
  
  //This is Temperature and Humidity DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h))
  {
    Serial.println("Failed to read from DHT");
  }
  else
  {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");
  }
   //This is Temperature and Humidity DHT sensor
   
  //ThingSpeak processing
  String humidityStr = String(h, DEC);
  Serial.println("humidity convert to string");
  Serial.println(humidityStr);
  String tempStr = String(t, DEC);
  Serial.println("temperature convert to string");
  Serial.println(tempStr);
  String noiseStr = String(noiseValue, DEC);
  Serial.print("noise: ");
  Serial.println(noiseStr);
  String PM01Str=String(PM01Value, DEC);
    Serial.println("PM01 convert to string");
    Serial.println(PM01Str);
    String PM2_5Str=String(PM2_5Value,DEC);
    Serial.println("PM2_5 convert to string");
    Serial.println(PM2_5Str);
    String PM10Str=String(PM10Value, DEC);
    Serial.println("PM10 convert to string");
    Serial.println(PM10Str);


  ThingSpeak.setField(1, tempStr);
  ThingSpeak.setField(2, humidityStr);
  ThingSpeak.setField(3, noiseStr);
  ThingSpeak.setField(4, PM01Str);
  ThingSpeak.setField(5, PM2_5Str);
  ThingSpeak.setField(6, PM10Str);
  

  // Write the fields that you've set all at once.
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  delay(16000); // ThingSpeak will only accept updates every 15 seconds.
  printCurrentNet();
    //ThingSpeak processing

}

//This is for dust sensor
char checkValue(unsigned char *thebuf, char leng)
{  
  char receiveflag=0;
  int receiveSum=0;

  for(int i=0; i<(leng-2); i++){
  receiveSum=receiveSum+thebuf[i];
  }
  receiveSum=receiveSum + 0x42;
 
  if(receiveSum == ((thebuf[leng-2]<<8)+thebuf[leng-1]))  //check the serial data 
  {
    receiveSum = 0;
    receiveflag = 1;
  }
  return receiveflag;
}

int transmitPM01(unsigned char *thebuf)
{
  int PM01Val;
  PM01Val=((thebuf[3]<<8) + thebuf[4]); //count PM1.0 value of the air detector module
  return PM01Val;
}

//transmit PM Value to PC
int transmitPM2_5(unsigned char *thebuf)
{
  int PM2_5Val;
  PM2_5Val=((thebuf[5]<<8) + thebuf[6]);//count PM2.5 value of the air detector module
  return PM2_5Val;
  }

//transmit PM Value to PC
int transmitPM10(unsigned char *thebuf)
{
  int PM10Val;
  PM10Val=((thebuf[7]<<8) + thebuf[8]); //count PM10 value of the air detector module  
  return PM10Val;
}
//This is for dust sensor

//This is for wifi debugging
void printCurrentNet() {

 char ssid[] = "AndroidAP";        // your network SSID (name)
char pass[] = "123456789";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the WiFi radio's status
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

//if disconnected will attempt to reconnect
  if(rssi == 0){  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(1000);
    break;
  } }
 

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}
//This is for wifi debugging
