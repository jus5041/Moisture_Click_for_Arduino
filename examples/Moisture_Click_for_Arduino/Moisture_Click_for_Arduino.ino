// ### FDC2112

#include <Wire.h>
#include "Moisture_Click_for_Arduino.h"
FDC2112 capsense(MOISTURE_DEV_ADR_GND); // Use FDC2112_I2C_ADDR_1 

#define MOISTURE_SAMPLE_NUM 16
uint16_t calib_air_data;
bool flag = false;

void setup() {
  
  // ### Start I2C 
  Wire.begin(23, 22);
  
  // ### Start serial
  Serial.begin(115200);
  Serial.println("\FDC2112 test");
  
  // ### Start FDC
  bool capOk = capsense.begin();

  if (capOk) Serial.println("Sensor OK");  
  else Serial.println("Sensor Fail");  
  
  moisture_cal ();
  if(flag) {
    Serial.println("moisture_cal OK"); 
    flag = false;
  }
  else Serial.println("moisture_cal Fail");  
}


void moisture_cal ()
{
    uint8_t cnt;
    uint32_t rx_data;
    uint16_t tmp;

    rx_data = 0;
    for ( cnt = 0; cnt < MOISTURE_SAMPLE_NUM; cnt++ )
    {
         tmp = capsense.readConv();
         rx_data += tmp;
         delay(10);
    }
    calib_air_data  = ( uint16_t )( rx_data / MOISTURE_SAMPLE_NUM );
    flag = true;
}


void loop() {
  unsigned long capa; // variable to store data from FDC
  uint8_t n_cnt;
  uint32_t rx_data = 0;
  int16_t moisture_data;
  uint32_t clc_data;
  uint16_t tmp;
  uint8_t moisture;

  for ( n_cnt = 0; n_cnt < MOISTURE_SAMPLE_NUM; n_cnt++ ){
         tmp = capsense.readConv();
         rx_data += tmp;
         delay(1);
         }
  rx_data /= MOISTURE_SAMPLE_NUM;
  moisture_data = ( uint16_t )( rx_data );

  if ( moisture_data > calib_air_data )
  {
      moisture_data = calib_air_data;
  }
  moisture_data = calib_air_data  - moisture_data;

  clc_data = ( uint32_t )( moisture_data * 100 );
  clc_data /= calib_air_data ;
  delay(1);
  
  moisture = ( uint8_t )clc_data;

  Serial.print("calib_air_data : "); 
  Serial.print(calib_air_data); 
  Serial.print(" / rx_data : "); 
  Serial.print(rx_data); 
  Serial.print(" / moisture_data : "); 
  Serial.print(moisture_data); 
  Serial.print(" / last_moisture : "); 
  Serial.print(moisture); 
  Serial.println("%"); 
    
}
