#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <virtuabotixRTC.h>
#include <LoRa.h>
#include <SPI.h>
#include "SIM800L.h"

// Inisialisasi pin LoRa
#define SCK     PB13    // GPIO5  -- SX1278's SCK
#define MISO    PB14   // GPIO19 -- SX1278's MISnO
#define MOSI    PB15   // GPIO27 -- SX1278's MOSI
#define SS      PB12   // GPIO18 -- SX1278's CS
#define RST     PA0   // GPIO14 -- SX1278's RESET
#define DI0     PA1   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND  915E6

// Inisialisasi pin RTC (CLK, DAT, RST)
virtuabotixRTC myRTC(PB10, PB11, PA3);

// Inisialisasi pin SIM800l
#define SIM800_RX_PIN PA9
#define SIM800_TX_PIN PA10
#define SIM800_RST_PIN PA2

const char APN[] = "M2MAUTOTRONIC";
const char URL[] = "https://postman-echo.com/post";
const char CONTENT_TYPE[] = "application/json";
const char PAYLOAD[] = "{\"name\": \"morpheus\", \"job\": \"leader\"}";

SIM800L* sim800l;

static const int RXPin = PC10, TXPin = PC11;
char databuffer[35];
double temp;

static const uint32_t dfBaud = 9600;
HardwareSerial Serial2(PD6, PD5);
SoftwareSerial df(RXPin, TXPin);
int counter = 0;
void setup() {
  Serial2.begin(115200);
  df.begin(dfBaud);
  while(!Serial2);
  getBuffer();
  set_sim800l(); 
  //set_lora();
  
} 
void loop() {
  
  loop_sim800l();
  // loop_lora();
  // loop_dfrobot();
 
}
