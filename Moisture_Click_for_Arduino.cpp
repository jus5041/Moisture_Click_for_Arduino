#include "Arduino.h"
#include <Wire.h>
#include "Moisture_Click_for_Arduino.h"

FDC2112::FDC2112(uint8_t i2caddr) {
	_i2caddr = i2caddr;
}

// Checking for chip ID, if OK, calls chip init
boolean FDC2112::begin() {

    int devId = read16FDC(MOISTURE_REG_DEVICE_ID);
    
    if (devId != 0x3054) { //two valid device ids for FDC2112 0x3054 and 0x3055
        if (devId != 0x3055) {
            return false; 
        }
    }

    loadSettings();

    return true;
}


//Internal routine to do actual chip init
void FDC2112::loadSettings() {
  write16FDC( MOISTURE_REG_RESET_DEV , MOISTURE_OUTPUT_GAIN_x16 );

  write16FDC( MOISTURE_REG_SETTLE_CNT , MOISTURE_SETTLE_CNT_CFG_DEFAULT );

  write16FDC( MOISTURE_REG_RCNT , MOISTURE_RCNT_CFG_DEFAULT );
  write16FDC( MOISTURE_REG_OFFSET , MOISTURE_DEFAULT_OFFSET );
  write16FDC( MOISTURE_REG_CLOCK_DIVIDERS , MOISTURE_COMBINED_VAL_CH0_DIV | MOISTURE_CLOCK_DIVIDERS_x10 );

  write16FDC( MOISTURE_REG_DRIVE_CURR,MOISTURE_DRIVE_CURR_CH0 );

  write16FDC( MOISTURE_REG_ERR_CFG , MOISTURE_ALL_ERR_ENABLED );
  write16FDC( MOISTURE_REG_CFG , MOISTURE_REG_CFG_DEFAULT_SETTINGS );

  write16FDC( MOISTURE_REG_MUX_CFG , MOISTURE_INPUT_DEGLITCH_FILT_BWDTH_33MHZ );
                       
}

// Gets 16bit reading for FDC2112 and FDC2114
unsigned long FDC2112::readConv() {
    int timeout = 100;
    unsigned long reading = 0;
    long long fsensor = 0;
    int status = read16FDC(MOISTURE_REG_STATUS);
    uint8_t addressMSB;
	  uint8_t bitUnreadConv;

		addressMSB = MOISTURE_REG_DATA;
		bitUnreadConv = MOISTURE_STATUS_CH0_UNREADCONV;
	
	while (timeout && !(status & bitUnreadConv)) {
        status = read16FDC(MOISTURE_REG_STATUS);
        timeout--;
    }
    if (timeout == 100) {

      reading = (uint16_t)(read16FDC(addressMSB) & MOISTURE_12_BIT_DATA);
      while (timeout && !(status & bitUnreadConv)) {
          status = read16FDC(MOISTURE_REG_STATUS);
          timeout--;
      }
    } 
    if (timeout) {
        //read the 16 bit result
        reading = (uint16_t)(read16FDC(addressMSB) & MOISTURE_12_BIT_DATA);
        return reading;
    } else return 0;   // Could not get data, chip readynes flag timeout

}
// Read 1 byte from the FDC at 'address'
uint8_t FDC2112::read8FDC(uint16_t address) {
    uint8_t data;
    Wire.beginTransmission(_i2caddr);
    Wire.write(address >> 8);
    Wire.write(address);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2caddr, (uint8_t) 1);
    uint8_t r = Wire.read();
    return r;
}

// Read 2 byte from the FDC at 'address'
uint16_t FDC2112::read16FDC(uint16_t address) {
    uint16_t data;

    Wire.beginTransmission(_i2caddr);
    Wire.write(address);
    Wire.endTransmission(false); //restart

    Wire.requestFrom(_i2caddr, (uint8_t) 2);
    while (!Wire.available());
    data = Wire.read();
    data <<= 8;
    while (!Wire.available());
    data |= Wire.read();
    Wire.endTransmission(true); //end
    return data;
}

// write 1 byte to FDC
void FDC2112::write8FDC(uint16_t address, uint8_t data) {
    Wire.beginTransmission(_i2caddr);
    Wire.write(address >> 8);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();
}

// write 2 bytes to FDC  
void FDC2112::write16FDC(uint16_t address, uint16_t data) {
    Wire.beginTransmission(_i2caddr);
    Wire.write(address & 0xFF);
    Wire.write(data >> 8);
    Wire.write(data);
    Wire.endTransmission();
}
