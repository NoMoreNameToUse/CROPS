#include "DHT.h"

#define DHTPIN 2     

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

/****************************** Serial communication ******************************/
const byte maxCLength = 32;
char serialRecieved[maxCLength];
char tempChars[maxCLength];        // temporary array for use when parsing

// variables to hold the parsed data
char serialStr[maxCLength] = {0};

boolean serialnewData = false;
