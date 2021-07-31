void INA219Begin() {
  // Initialize INA219.
  uint32_t currentFrequency;

  if (! ina219_5VHARDWARE.begin()) {
    Serial.println("Failed to find INA219 main 5v bus chip");
    error(10500);
    ina219_5VData.ifConnected = false;
  }
  if (! ina219_12VMAIN.begin()) {
    Serial.println("Failed to find INA219 main 12v bus chip");
    error(11500);
    ina219_12VMData.ifConnected = false;
  }
  if (! ina219_12VBRIDGE.begin()) {
    Serial.println("Failed to find INA219 Bridge power bus chip");
    error(11500);
    ina219_12VBData.ifConnected = false;
  }
}


void declarePinMode() {
  //Output
  for (int i = 0; i < 2; i++) {
    pinMode(RelayPin[i], OUTPUT);
    digitalWrite(RelayPin[i], HIGH);
  }
  pinMode(BuzzerPin, OUTPUT);
  //Input
  for (int i = 0; i < 3; i++) {
    pinMode(CSMSPin[i], INPUT);
  }
  pinMode(RainSenPin, INPUT);
}

void bootEssential() {
  preference.begin("boot", false);
  counter = preference.getUInt("counter", 0);
  if (getCurrentTime()) {
    if (counter != 0) {
      lastboot.tm_sec = preference.getInt("tm_sec", 0);
      lastboot.tm_min = preference.getInt("tm_min", 0);
      lastboot.tm_hour = preference.getInt("tm_hour", 0);
      lastboot.tm_mday = preference.getInt("tm_mday", 1);
      lastboot.tm_mon = preference.getInt("tm_mon", 0);
      lastboot.tm_year = preference.getInt("tm_year", 0);
      lastboot.tm_wday = preference.getInt("tm_wday", 0);
      lastboot.tm_yday = preference.getInt("tm_yday", 0);
      lastboot.tm_isdst = preference.getInt("tm_isdst", 0);
      Serial.println(lastboot.tm_sec);
      Serial.println(lastboot.tm_min);
      Serial.println(lastboot.tm_hour);
      Serial.println(mktime(&lastboot));
    }
    preference.putInt("tm_sec", timeinfo.tm_sec);
    preference.putInt("tm_min", timeinfo.tm_min);
    preference.putInt("tm_hour", timeinfo.tm_hour);
    preference.putInt("tm_mday", timeinfo.tm_mday);
    preference.putInt("tm_mon", timeinfo.tm_mon);
    preference.putInt("tm_year", timeinfo.tm_year);
    preference.putInt("tm_wday", timeinfo.tm_wday);
    preference.putInt("tm_yday", timeinfo.tm_yday);
    preference.putInt("tm_isdst", timeinfo.tm_isdst);
    Serial.println(timeinfo.tm_sec);
    Serial.println(timeinfo.tm_min);
    Serial.println(timeinfo.tm_hour);
    Serial.println(mktime(&timeinfo));
    
    double seconds = difftime(mktime(&timeinfo), mktime(&lastboot));
    Serial.print ("---- Second from last boot:----");
    Serial.println(seconds);

    //Buffer for logging it to SD card later
    char buff[50];
    snprintf(buff, 50, "Second from last boot: %f seconds \r\n", seconds);
    strcat(logBuffer, buff);
  }
  counter++;
  preference.putUInt("counter", counter);
  preference.end();
  // Print the counter to Serial Monitor
  Serial.printf("Boot Nr: %u\r\n", counter);
}
void startWifi() {
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
}

void startSDCard() {
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    error(04000);
    stopOrWarning();
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    error(04001);
    stopOrWarning();
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}
