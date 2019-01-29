/*
  code for RFID get request to register at server
  logic :
    get connection
      wait RFID read
        GET to server
*/
#define SS_PIN D4  //D2
#define RST_PIN D3 //D1
#define RELAY D8
#include <SPI.h>
#include <MFRC522.h>

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;

void setup() {
  // put your setup code here, to run once:
  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);
  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Hehe", "normalin");
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    content.toUpperCase();
    Serial.println();

    HTTPClient http;
    String apikey="aef88b00bcaff32001";
    String RFID_num = content.substring(1);
    String number_id = "1234544";
    String message = "https://member.patriot-id.org/api/write?apikey="+apikey+"&rfid="+RFID_num+"&number="+number_id;

    USE_SERIAL.print("[HTTP] begin...\n");
    Serial.println(message);
    // configure traged server and url
    http.begin(message, "19 A3 10 7D 5B 17 D7 A1 DD DE FF BB E8 08 A0 98 F0 7E 6D 86"); //HTTPS
//    http.begin("http://member.patriot-id.org/api/write?apikey=aef88b00baff32001&rfid=aabbccdd&number=123451"); //HTTP

    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        USE_SERIAL.println(payload);
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(10000);
}
