#define DEBUG

#include <WiFi.h>
#include <HTTPClient.h>
#include "dropboxSDK.hpp"
#include <SPIFFS.h>

#define TOKEN "7LH8leMAXXEAAAAAAAAAAXHA7o-z13ag3B0UrOMLZ7BkpZQbMDR2RkkZcDGC2S3s"

const char* ssid = "Wifi-Casa";
const char* password = "canotaje";

Dropbox dbx;

void setup() {
  Serial.begin(115200);
  
  if(!SPIFFS.begin()){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
 
  WiFi.begin(ssid, password); 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");

  char *msg = "Hola. Como te va, capo? sos muy crack";
  size_t msgLen = strlen(msg);


  dbx.begin(TOKEN);
  delay(2000);
  Serial.println("uploading file");
  //dbx.uploadString(msg, msgLen, "/otherFolder/moko.txt");
  //bool success = dbx.uploadFile(SPIFFS, "/beemovie.txt", "/mitulo/ratules.txt");
  //bool success = dbx.uploadFile(SPIFFS, "/hola.txt", "/mitulo/pitiflauta.txt");
  //bool success = dbx.uploadString(msg, msgLen, "/makatraka/pelele.txt");
  char *fileContent = dbx.downloadString("/Homework/math/Matrices.txt");
  //Serial.println((success ? "successful" : "error"));
  Serial.println(fileContent);
  Serial.println("done");

/*
  Serial.println("sending...");
    if ((WiFi.status() == WL_CONNECTED)) {

        HTTPClient http;
        http.begin("https://content.dropboxapi.com/2/files/upload", Dropbox::root_ca);
        http.addHeader("Authorization", String("Bearer ") + String(TOKEN));
        http.addHeader("Dropbox-API-Arg", "{\"path\": \"/makalaka/molo.txt\",\"mode\": \"add\",\"autorename\": true,\"mute\": false,\"strict_conflict\": false}");
        http.addHeader("Content-Type", "text/plain; charset=dropbox-cors-hack");
        int statusCode = http.POST((uint8_t *)msg, msgLen);

        Serial.println(statusCode);
        Serial.println(http.getString());

        http.end();
    }
   Serial.println("done");
  */ 
}
 
void loop() {
  delay(5000);
}
