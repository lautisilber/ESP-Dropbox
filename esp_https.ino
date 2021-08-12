#include <WiFi.h>
#include <HTTPClient.h>
#include "dropboxSDK.hpp"

const char* ssid = "SSID";
const char* password = "PASSWORD";

Dropbox dbx;

void setup() {
  Serial.begin(115200);
  delay(1000);
 
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
}
 
void loop() {
  Serial.print("Testing dbx connection... ");
  Serial.println(dbx.test());
  delay(5000);
}
