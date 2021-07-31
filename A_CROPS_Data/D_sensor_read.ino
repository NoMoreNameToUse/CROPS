void ina219_Read(char ina219_Select) {
  Adafruit_INA219 *INA_Name;
  ina219_Data *INA_Data;
  bool INAConnected;
  switch (ina219_Select) {
    case 'C':
      INA_Name = &ina219_5VHARDWARE;
      INA_Data = &ina219_5VData;
      INAConnected = INA_Data->ifConnected;
      break;
    case 'M':
      INA_Name = &ina219_12VMAIN;
      INA_Data = &ina219_12VMData;
      INAConnected = INA_Data->ifConnected;
      break;
    case 'B':
      INA_Name = &ina219_12VBRIDGE;
      INA_Data = &ina219_12VBData;
      INAConnected = INA_Data->ifConnected;
      break;
    default:
      error(05000);
      break;
  }
  if (INAConnected) {
    INA_Data->shuntvoltage = INA_Name->getShuntVoltage_mV();
    INA_Data->busvoltage = INA_Name->getBusVoltage_V();
    INA_Data->current_mA = INA_Name->getCurrent_mA();
    INA_Data->power_mW = INA_Name->getPower_mW();
    INA_Data->loadvoltage = INA_Data->busvoltage + (INA_Data->shuntvoltage / 1000);
  }
  else {
    Serial.println("read failed");
  }
}
void getDHTData(char select) {
  dht_Data *dhtDataSelect;
  switch (select) {
    case '1':
      dhtDataSelect = &dht1Data;
      // Read temperature as Fahrenheit (isFahrenheit = true)
      dhtDataSelect->humidity = dht1.readHumidity();
      dhtDataSelect->tempC = dht1.readTemperature();
      dhtDataSelect->tempF = dht1.readTemperature(true);

      if (isnan(dhtDataSelect->humidity) || isnan(dhtDataSelect->tempC) || isnan(dhtDataSelect->tempF)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        dhtDataSelect->validReading = false;
        return;
      } else if (!dhtDataSelect->validReading) {
        dhtDataSelect->validReading = true;
      }
      // Compute heat index in Fahrenheit (the default)
      dhtDataSelect->hif = dht1.computeHeatIndex(dhtDataSelect->tempF, dhtDataSelect->humidity);
      // Compute heat index in Celsius (isFahreheit = false)
      dhtDataSelect->hic = dht1.computeHeatIndex(dhtDataSelect->tempC, dhtDataSelect->humidity, false);
      break;
    case '2':
      dhtDataSelect = &dht2Data;
      // Read temperature as Fahrenheit (isFahrenheit = true)
      dhtDataSelect->humidity = dht2.readHumidity();
      dhtDataSelect->tempC = dht2.readTemperature();
      dhtDataSelect->tempF = dht2.readTemperature(true);

      if (isnan(dhtDataSelect->humidity) || isnan(dhtDataSelect->tempC) || isnan(dhtDataSelect->tempF)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        dhtDataSelect->validReading = false;
        return;
      } else if (!dhtDataSelect->validReading) {
        dhtDataSelect->validReading = true;
      }
      // Compute heat index in Fahrenheit (the default)
      dhtDataSelect->hif = dht2.computeHeatIndex(dhtDataSelect->tempF, dhtDataSelect->humidity);
      // Compute heat index in Celsius (isFahreheit = false)
      dhtDataSelect->hic = dht2.computeHeatIndex(dhtDataSelect->tempC, dhtDataSelect->humidity, false);
      break;
    default:
      error(05001);
      break;
  }
}

void getMoistureData(short int CSMSSelect) {
  moistureValue = analogRead(CSMSPin[CSMSSelect]);
  if (moistureValue > MMax[CSMSSelect]  ) {
    error (13200 + CSMSSelect);
    moistureValue = MMax[CSMSSelect];
  } else if (moistureValue < MMin[CSMSSelect]) {
    error (13200 + CSMSSelect);
    moistureValue = MMin[CSMSSelect];
  }
  CSMPercentValue[CSMSSelect] = mapPrecise(moistureValue, MMax[CSMSSelect], MMin[CSMSSelect], 0, 100);
}
void getSensorData(short int sensorPin, short int sensorSelect) {
  sensorValue = analogRead(sensorPin);
  if (sensorValue > SMax[sensorSelect]  ) {
    error (14200 + sensorSelect);
    sensorValue = SMax[sensorSelect];
  } else if (sensorValue < SMin[sensorSelect]) {
    error (14200 + sensorSelect);
    sensorValue = SMin[sensorSelect];
  }
  sensorPercentValue[sensorSelect] = mapPrecise(sensorValue, SMax[sensorSelect], SMin[sensorSelect], 0, 100);
}


float mapPrecise(short int rawValue, short int rawMax, short int rawMin, short int mapMax, short int mapMin ) {
  return ((float) (rawValue - rawMin) * (mapMax - mapMin) / (rawMax - rawMin)) + mapMin;
}
