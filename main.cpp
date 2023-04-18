#include <Arduino.h>
#include <LiquidCrystal.h>
#include "SPI.h"
#include "MFRC522.h"
#include <Wire.h>

//other pins are connected to the general SPI pins of the ESP32, these 2 are configurable
#define SS_PIN 5
#define RST_PIN 34

//initialise the library with the numbers of the interface pins, should be in order of the pins of the LCD display, RS, RW, E etc
LiquidCrystal lcd(32, 33, 25, 26, 27, 14);
//Create the rfid instance (in other words initialise the library just like LiquidCrystal)
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(921600); //initialise serial comms with computer
  SPI.begin(); //initialise the SPI bus 
  rfid.PCD_Init(); //initialise the sensor

  Serial.println("Tap card");
  
  // set up LCD's number of colunns and rows
  lcd.begin(16, 2);

  // print message

  lcd.print("Tap Card");
}

void loop() {
//set the cusor to column 0 line 1, line 1 is the second row, counting begins at 0

lcd.setCursor(0, 1);

//print number of seconds since reset

//lcd.print(millis() / 1000);

if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) { //Checks to see if a card is present & if the card has been read
  String uidString = ""; //creates a string which the UID will be converted to so that it can be compared in an if statement

  for (byte i = 0; i < rfid.uid.size; i++){ //for loop which goes through every byte of the UID from the first to the UID size
    uidString += String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""); //makes sure each byte has a 0 if it is a single digit 
    uidString += String(rfid.uid.uidByte[i], HEX); //concatinates the HEX format into a string
  }

//checks if the string is equal to out desired UID
  if (uidString == "a99102e5") { 
    lcd.clear();
    lcd.print("Access Granted");
  }

  else {
    lcd.clear();
    lcd.print("Access Denied");
  }

  Serial.print(uidString);
  Serial.println();
  // lcd.println();

  rfid.PICC_HaltA(); //halts comms so that the reader is not constantly reading the same card, saves power
}

}
