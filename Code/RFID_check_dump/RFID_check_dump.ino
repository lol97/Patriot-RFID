/*
 *  Created by TheCircuit
*/

#define SS_PIN D4  //D2
#define RST_PIN D3 //D1
#define RELAY D8
#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY,LOW);
}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if(content.substring(1).length()>0){
    Serial.println("there's text");
  }
  if (content.substring(1) == "42 71 19 10") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    Serial.println(" Welcome Mr.Circuit ");
    digitalWrite(RELAY,HIGH);
    Serial.println(" Have FUN ");
    Serial.println();
    delay(1000);
    digitalWrite(RELAY,LOW);
    statuss = 1;
    
  }
  
  else   {
    Serial.println(" Access Denied ");
    delay(3000);
  }
} 
