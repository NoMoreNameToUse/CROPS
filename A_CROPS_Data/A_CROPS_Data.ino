#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219_5VHARDWARE;
Adafruit_INA219 ina219_12VMAIN(0x44);
Adafruit_INA219 ina219_12VBRIDGE(0x41);

#define IndexEM 9
#define IndexED 16

#define RXD2 16
#define TXD2 17

const char codeVersion[] = "v1.2.1";
#define serialFlag true
#define stopOrWarningFlag true
/****************************** Pin definition ******************************/
#define TempSenPin 36
#define WaterSenPin 39
#define LightSenPin 33
#define RainSenPin 25
#define BuzzerPin 12
#define DHT1PIN 14
#define DHT2PIN 13

const unsigned short int RelayPin[2] = {26, 27};
const unsigned short int CSMSPin[3] = {34, 35, 32};
/****************************** Sensor ******************************/
//DHT Temperature & Moisture sensor
#include "DHT.h"
#define DHTTYPE DHT22
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht1(DHT1PIN, DHTTYPE);
DHT dht2(DHT2PIN, DHTTYPE);

//Capacitive Soil Moisture Sensor (Ref. CSMS)


const short int MMax[3] = {4095, 4095, 4095};
const short int MMin[3] = {1089, 1089, 1089};

const short int SMax[4] = {4095, 4095, 4095};
const short int SMin[4] = {0, 0, 0};

/****************************** Serial communication ******************************/
#define mainCon_VMax 5.5f
#define mainCon_VMin 3.6f
#define main12V_VMax 14.5f
#define main12V_VMin 9.5f

#define mainCon_CMax 500.0f
#define main12V_CMax 3000.0f
/****************************** Serial communication ******************************/
const byte maxCLength = 32;
char serialRecieved[maxCLength];
char tempChars[maxCLength];        // temporary array for use when parsing

// variables to hold the parsed data
char serialStr[maxCLength] = {0};

boolean serialnewData = false;
boolean sdConnected = false;
/****************************** SD Card file system ******************************/
#include "FS.h"
#include "SD.h"
#include "SPI.h"

char dataDir[40];
char logDir[50];
char INALogDir[50];
char DHTLogDir[50];
char sensorLogDir[50];

char destinationDir[50];

char logBuffer[600];
char sensorDataBuffer[300];
const char INATitle[] = "timestamp, INA5v_busvoltage, INA5v_shuntvoltage_mV, INA5v_loadvoltage, INA5v_current_mA, INA5v_power_mW, "
                        "INA12Bv_busvoltage, INA12Bv_shuntvoltage_mV, INA12Bv_loadvoltage, INA12Bv_current_mA, INA12Bv_power_mW, "
                        "INA12Mv_busvoltage, INA12Mv_shuntvoltage_mV, INA12Mv_loadvoltage, INA12Mv_current_mA, INA12Mv_power_mW, \r\n";
const char DHTTitle[] = "timestamp, DHT1_humidity, DHT1_tempC, DHT1_tempF, DHT1_hif, DHT1_hic, "
                        "DHT2_humidity, DHT2_tempC, DHT2_tempF, DHT2_hif, DHT2_hic, \r\n";
const char sensorTitle[] = "timestamp, CSMS1_humidity, CSMS2_humidity, CSMS3_humidity, Light_intensity, Temp_Celsius, "
                           "Water_percentage, Rain_percentage, \r\n";
/****************************** Flash preference ******************************/
#include <Preferences.h>
Preferences preference;

struct tm lastboot;
struct tm timeinfo;
unsigned int counter;

unsigned long previousMillis = 0;
unsigned long monitorMillis = 0;
unsigned long buzzMillis = 0;
const long interval = 10000;
bool buzzerState = false;
bool buzzFlag[3] = {false, false, false};
/****************************** Wifi ******************************/
#include <WiFi.h>
#include "time.h"

const char* ssid     = "WLAN-339828";
const char* password = "36938296529523707325";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

short int moistureValue;
short int sensorValue;

struct ina219_Data {
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;
  bool ifConnected = true;
};
struct dht_Data {
  float humidity = 0;
  float tempC = 0;
  float tempF = 0;
  float hif = 0;
  float hic = 0;
  bool validReading = true;
};

struct ina219_Data ina219_5VData, ina219_12VMData, ina219_12VBData;
struct dht_Data dht1Data, dht2Data;

float CSMPercentValue [3] = {0.0, 0.0, 0.0};
float sensorPercentValue [3] = {0.0, 0.0, 0.0};
bool rainValue = 0;

void createDir(fs::FS &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}
void appendFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (!file.print(message)) {
    Serial.print("failed to append data to");
    Serial.println(path);
  }
  file.close();
}
