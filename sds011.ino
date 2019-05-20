#include "SdsDustSensor.h"
#include <RTClib.h>
#include <SD.h>
#include <Wire.h>
#include <SPI.h>


// tested on Arduino Leonardo with Serial1
SdsDustSensor sds(Serial1); // passing HardwareSerial& as parameter
DS1307 rtc;
/* SD-Karte
Die Library SD von Adafruit wird benötigt.
*/
File dataFile; // Datei für Distanzen

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  rtc.begin();
  
  sds.begin(); // this line will begin Serial1 with given baud rate (9600 by default)

  Serial.println("Initializing SD card...");
  if (!SD.begin(10, 11, 12, 13)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }
  Serial.println("SUCCESS - SD card initialized.");
}
  
  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setQueryReportingMode().toString()); // ensures sensor is in 'query' reporting mode
}

/*Schreibt die Ergebnisse auf die SD-Karte*/

void printErgebnisse2SD(DateTime now, float pm25, float pm10) {
  dataFile.print(now.year(), DEC);
  dataFile.print('/');
  dataFile.print(now.month(), DEC);
  dataFile.print('/');
  dataFile.print(now.day(), DEC);
  dataFile.print(' ');
  dataFile.print(now.hour(), DEC);
  dataFile.print(':');
  dataFile.print(now.minute(), DEC);
  dataFile.print(':');
  dataFile.print(now.second(), DEC);
  dataFile.print(' ');
  dataFile.print(pm25);
  dataFile.print(' ');
  dataFile.print(pm10);
  dataFile.println();
}

void loop() {
  DateTime now = rtc.now(); // Zeit
  PmResult pm = sds.queryPm();
  if (pm.isOk()) {
    Serial.print("PM2.5 = ");
    Serial.print(pm.pm25);
    Serial.print(", PM10 = ");
    Serial.println(pm.pm10);
    printErgebnisse2SD(now, pm25, pm10);
   }
}
