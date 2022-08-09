// ### FDC2112

#include <Wire.h>
#include "Moisture_Click_for_Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HardwareSerial.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SW 21
#define RX 16
#define TX 17

#define MOISTURE_SAMPLE_NUM 16



FDC2112 capsense(MOISTURE_DEV_ADR_GND); // Use FDC2112_I2C_ADDR_1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
HardwareSerial BTSerial(2); // 소프트웨어 시리얼 (TX,RX) 

uint16_t calib_air_data;
bool flag = false;

void setup() {
  
  // ### Start I2C 
  Wire.begin(23, 22);
  
  // ### Start serial
  Serial.begin(115200);
  Serial.println("Moisture Click Test");
  
  pinMode(SW,INPUT);
  pinMode(RX, INPUT);
  BTSerial.begin(115200,SERIAL_8N1,RX,TX); //Rx,Tx
  
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

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(5);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.display(); 

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

void moisture_get_data()
{
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

  log_print(rx_data, moisture_data, moisture);
  oled_print(moisture);
  BTSerial.print(moisture); 
}

void log_print(uint32_t rx_data, int16_t moisture_data, uint8_t moisture)
{
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

void oled_print(uint8_t moisture){
  
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print(moisture);
  display.print("%");
  display.display(); 
}

void drawProgressbar(int x,int y, int width,int height, int progress){
   progress = progress > 100 ? 100 : progress; // set the progress value to 100
   progress = progress < 0 ? 0 :progress; // start the counting to 0-100
   float bar = ((float)(width-1) / 100) * progress;
   display.drawRect(x, y, width, height, WHITE);
   display.fillRect(x+2, y+2, bar , height-4, WHITE); // initailize the graphics fillRect(int x, int y, int width, int height)
}


void loop() {

  int cnt = 0;
  bool flag = false;

  while(digitalRead(SW) == HIGH){
    delay(10);
    cnt++;
    if(cnt == 150){
      display.clearDisplay();
      for(int i=0 ; i<10 ; i++){
        drawProgressbar(0,20,120,10, i * 10 );
        display.display();
        delay(100);
      }
      flag = true;
      break;
    }
  }
  
  if(flag){
    moisture_get_data();
  }

}