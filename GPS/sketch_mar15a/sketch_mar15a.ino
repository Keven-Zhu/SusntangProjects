#include <TinyGPS++.h>

#include <CANHelper.h>
#include <GPSHelper.h>

#include <TinyGPS.h>


#include <SoftwareSerial.h>
#include <mcp_can.h>
#include <SPI.h>


#define RX_PIN 3
#define TX_PIN 4
#define GPS_BAUD 9600
#define DEBUG_BAUD 115200


#define DEBUG 1

// The TinyGPS++ object
TinyGPSPlus gps;

//CAN bus object
MCP_CAN CAN(10);

// The serial connection to the GPS device
SoftwareSerial ss(RX_PIN, TX_PIN);

void setup() {
  ss.begin(GPS_BAUD);
  #if DEBUG == 1
  Serial.begin(DEBUG_BAUD);
  Serial.println("Software Serial (GPS) initialized");
  #endif
  
  while(CAN.begin(CAN_SPEED) != CAN_OK)
  {
    #if DEBUG == 1
    Serial.println("CAN BUS Shield init fail");
    Serial.println("Init CAN BUS Shield again");
    #endif
    delay(100);
  }
  #if DEBUG == 1
  Serial.println("CAN BUS Shield initialized");
  #endif
}

void loop() {
  while (ss.available() > 0)    //Wait for GPS to send data
  {
    if (gps.encode(ss.read())) //Read the data and give it to the parser
    {
      //Location
      if(gps.location.isUpdated() && gps.location.isValid())
      {
        uint8_t locationBuffer[8];
        int32_t latitude = gps.location.lat() * 1e6;
        int32_t longitude = gps.location.lng() * 1e6;
        
        #if DEBUG == 1
        Serial.print("latitude: ");
        Serial.println(latitude);
        Serial.print("longitude: ");
        Serial.println(longitude);
        #endif
        
        insert32Bit(0, locationBuffer, latitude);
        insert32Bit(4, locationBuffer, longitude);
         
        CAN.sendMsgBuf(GPS_LOCATION_CAN_ID, 0, 8, locationBuffer);
      }
    }
  }
}
