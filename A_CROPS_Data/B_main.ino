void setup() {
  //Initiate Serial communication
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  //Set Esp32 Pinmode
  declarePinMode();

  //Begin sensor connection

  INA219Begin();
  dht1.begin();
  dht2.begin();

  startWifi();

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  bootEssential();
  startSDCard();
  //Set directory name and create a new directory on SD as well as set directory for data logging.
  snprintf(dataDir, 40, "/boot_%d__%d.%d.%d_%dh%dmin", counter, timeinfo.tm_mday, 1 + timeinfo.tm_mon, 1900 + timeinfo.tm_year, timeinfo.tm_hour, timeinfo.tm_min);
  createDir(SD, dataDir);
  sprintf(logDir, "%s%s", dataDir, "/log.txt");
  sprintf(INALogDir, "%s%s", dataDir, "/INA219Data.csv");
  sprintf(DHTLogDir, "%s%s", dataDir, "/DHT22Data.csv");
  sprintf(sensorLogDir, "%s%s", dataDir, "/sensorData.csv");


  char bootMessage[200];
  char lastbBootMessage[150];
  char bootCountBuffer[50];
  strftime (bootMessage, 150, "///Boot time: %c, ", &timeinfo);
  snprintf(bootCountBuffer, 50, "boot count: %d, code version: %s ///\r\n", counter, codeVersion);
  strncat (bootMessage, bootCountBuffer, 50);

  strftime (lastbBootMessage, 150, "#last boot time: %c, #\r\n", &lastboot);
  strncat (logBuffer, lastbBootMessage, 80);


  writeFile(SD, logDir, bootMessage);
  writeFile(SD, INALogDir, bootMessage);
  writeFile(SD, DHTLogDir, bootMessage);
  writeFile(SD, sensorLogDir, bootMessage);

  appendFile(SD, logDir, logBuffer);
  memset(logBuffer, 0, sizeof(logBuffer));

  appendFile(SD, INALogDir, INATitle);
  appendFile(SD, DHTLogDir, DHTTitle);
  appendFile(SD, sensorLogDir, sensorTitle);
  sdConnected = true;
  Serial.print(bootMessage);
}

void loop() {
  serialCommHandler();
  serialReciever();
  buzzHandler();
  powerMonitor();

  unsigned long currentMillis = millis();

  //////////////////Sensor reading event//////////////////
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //////////////////Sensor reading event//////////////////

    //get a time stamp
    char timeStamp[10] = "Failed";
    if (getCurrentTime()) {
      snprintf(timeStamp, 10, "%d-%d-%d, ", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    }

    //Read from INA219 Sensor(voltage and current) and save it to file
    ina219_Read('C');
    ina219_Read('B');
    ina219_Read('M');
    strcat(sensorDataBuffer, timeStamp);
    INADataHandler(ina219_5VData);
    INADataHandler(ina219_12VBData);
    INADataHandler(ina219_12VMData);
    strcat(sensorDataBuffer, "\r\n");
    appendFile(SD, INALogDir, sensorDataBuffer);
    memset(sensorDataBuffer, 0, sizeof(sensorDataBuffer));

    //Read from DHT22 Sensor(temperature and humidity) and save it to file
    getDHTData('1');
    getDHTData('2');
    strcat(sensorDataBuffer, timeStamp);
    DHTDataHandler(dht1Data);
    DHTDataHandler(dht2Data);
    strcat(sensorDataBuffer, "\r\n");
    appendFile(SD, DHTLogDir, sensorDataBuffer);
    memset(sensorDataBuffer, 0, sizeof(sensorDataBuffer));


    getMoistureData(0);
    getMoistureData(1);
    getMoistureData(2);
    strcat(sensorDataBuffer, timeStamp);
    sensorDataHandler(true);
    getSensorData(LightSenPin, 0);
    getSensorData(TempSenPin, 1);
    getSensorData(WaterSenPin, 2);
    sensorDataHandler(false);
    getHandleRainSensor();
    strcat(sensorDataBuffer, "\r\n");
    appendFile(SD, sensorLogDir, sensorDataBuffer);
    memset(sensorDataBuffer, 0, sizeof(sensorDataBuffer));


    //get reading cycle time
    unsigned long cycleEndMillis = millis();
    unsigned long cycleMillis = cycleEndMillis - previousMillis;
    snprintf(logBuffer, 600, "Read cycle time: %ld ms \r\n", cycleMillis);
    appendFile(SD, logDir, logBuffer);
    Serial.print(logBuffer);
    memset(logBuffer, 0, sizeof(logBuffer));

    //////////////////Sensor reading event//////////////////
  }
  //////////////////Sensor reading event//////////////////

}
