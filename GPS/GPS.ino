#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.   
It requires the use of SoftwareSerial, and assumes that you have a 9600-baud serial 
GPS device hooked up on pins 8(rx) and 9(tx).*/
static const int RXPin = 10, TXPin = 11;
static const int GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop()
{ 
  ss.listen();
  Serial.println(ss.available());
  ss.write(10);
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

 //Serial.println(ss.isListening());

  Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
  Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
  Serial.print("ALT=");  Serial.println(gps.altitude.meters());
  
  if (millis() > 5000 && gps.charsProcessed() < 10)
                {
    Serial.println("No GPS detected: check wiring.");
    Serial.print("Sentences that failed checksum=");
    Serial.println(gps.failedChecksum());
 
// Testing overflow in SoftwareSerial is sometimes useful too.
    Serial.print("Soft Serial device overflowed? ");
    Serial.println(ss.overflow() ? "YES!" : "No");
    while(true);
  }
}

void displayInfo()
{
  Serial.print("Location: "); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(",");
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print("INVALID");
  }  
  Serial.println();
}


