/**
 * I2CScanner.ino -- I2C bus scanner for Arduino
 * 2009,2014, Tod E. Kurt, http://todbot.com/blog/
 * Modified by Ashish Adhikari: https://www.youtube.com/user/tarantula3
**/

#include "Wire.h"
extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}

// Scan the I2C bus between addresses from_addr and to_addr.
// On each address, call the callback function with the address and result.
// If result==0, address was found, otherwise, address wasn't found
// (can use result to potentially get other status on the I2C bus, see twi.c)
// Assumes Wire.begin() has already been called
void scanI2CBus(byte from_addr, byte to_addr, void(*callback)(byte address, byte result) ) 
{
  byte rc;
  byte data = 0; // not used, just an address to feed to twi_writeTo()
  for( byte addr = from_addr; addr <= to_addr; addr++ ) {
    rc = twi_writeTo(addr, &data, 0, 1, 0);
    callback( addr, rc );
  }
}

// Called when address is found in scanI2CBus()
// (like adding I2C comm code to figure out what kind of I2C device is there)
void scanFunc( byte addr, byte result ) {
  Serial.print("ADD: ");
  Serial.print(addr, HEX);
  Serial.print( (result==0) ? " found!":"       ");
  Serial.print( (addr%4) ? "\t":"\n");
}


byte start_address = 8;       // lower addresses are reserved to prevent conflicts with other protocols
byte end_address = 200;       // higher addresses unlock other modes, like 10-bit addressing

void setup(){
    Wire.begin();

    Serial.begin(9600);                   // Changed from 19200 to 9600 which seems to be default for Arduino serial monitor
    Serial.println("\nI2CScanner ready!");

    Serial.print("starting scanning of I2C bus from ");
    Serial.print(start_address, DEC);
    Serial.print(" to ");
    Serial.print(end_address, DEC);
    Serial.println("...");

    // start the scan, will call "scanFunc()" on result from each address
    scanI2CBus( start_address, end_address, scanFunc );

    Serial.println("\ndone");
}

void loop(){}
