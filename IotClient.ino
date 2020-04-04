  // This code stablish connectio with the ThingSpeak API, ennables to write data 
  // comming from the accelerometer of the Arduino Nana IoT.
   
#include "ThingSpeak.h"
#include <WiFiNINA.h>
#include "secrets.h"
#include <Arduino_LSM6DS3.h>

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long readChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
//int number1 = 0;
float x;
float y;
float z;


void setup() {
  Serial.begin(115200);  // Initialize serial

  //check if i receive data from my accelerometer

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
    
  ThingSpeak.begin(client);  //Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(50);     
    } 
    Serial.println("\nConnected.");
  }
 
//accelerometer read
 
 float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    Serial.print(x*100);
    Serial.print(',');
    Serial.print(y*100);
    Serial.print(',');
    Serial.println(z*100);
  }
  
// set the fields with the values

  ThingSpeak.setField(1, x*100);
  ThingSpeak.setField(2, y*100);
  ThingSpeak.setField(3, z*100);

 
 ThingSpeak.writeFields(readChannelNumber, myWriteAPIKey);
  
 delay(3000); // Wait 3 seconds to update the channel again
}
