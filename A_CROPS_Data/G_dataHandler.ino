void INADataHandler(struct ina219_Data INA_Data) {
  char INABuffer[65];
  if (serialFlag && INA_Data.ifConnected) {
    Serial.print("Bus Voltage:   "); Serial.print(INA_Data.busvoltage); Serial.println(" V");
    Serial.print("Shunt Voltage: "); Serial.print(INA_Data.shuntvoltage); Serial.println(" mV");
    Serial.print("Load Voltage:  "); Serial.print(INA_Data.loadvoltage); Serial.println(" V");
    Serial.print("Current:       "); Serial.print(INA_Data.current_mA); Serial.println(" mA");
    Serial.print("Power:         "); Serial.print(INA_Data.power_mW); Serial.println(" mW");
    Serial.println("");
  }
  snprintf(INABuffer, 65, "%f, %f, %f, %f, %f, ", INA_Data.busvoltage , INA_Data.shuntvoltage, INA_Data.loadvoltage, INA_Data.current_mA, INA_Data.power_mW);
  strncat (sensorDataBuffer, INABuffer, 65);
  if (serialFlag) {
    Serial.print(strlen(INABuffer));
    Serial.print(" INA ");
    Serial.println(INABuffer);
  }
}

void DHTDataHandler(struct dht_Data DHT_Data) {
  char DHTBuffer[65];
  if (DHT_Data.validReading) {
    if (serialFlag) {
      Serial.printf("Temperature %f, Humidity %f, ", DHT_Data.tempC, DHT_Data.humidity);
      Serial.println("");
    }
  } else {
    Serial.println("DHT reading failed");
  }
  snprintf(DHTBuffer, 65, "%f, %f, %f, %f, %f, ", DHT_Data.humidity , DHT_Data.tempC, DHT_Data.tempF, DHT_Data.hif, DHT_Data.hic);
  strncat (sensorDataBuffer, DHTBuffer, 65);
  if (serialFlag) {
    Serial.print(strlen(DHTBuffer));
    Serial.print(" DHT ");
    Serial.println(DHTBuffer);
  }
}
void sensorDataHandler(bool select) {
  char sensorBuffer[65];
  if (select) {
    snprintf(sensorBuffer, 65, "%f, %f, %f, ", CSMPercentValue[0] , CSMPercentValue[1], CSMPercentValue[2]);
  } else {
    snprintf(sensorBuffer, 65, "%f, %f, %f, ", sensorPercentValue[0] , sensorPercentValue[1], sensorPercentValue[2]);
  }
  strncat (sensorDataBuffer, sensorBuffer, 65);
  if (serialFlag) {
    Serial.print(strlen(sensorBuffer));
    Serial.print(" Sensor ");
    Serial.println(sensorBuffer);
  }
}
void getHandleRainSensor() {
  char rainBuffer[10];
  rainValue = digitalRead(RainSenPin);
  snprintf(rainBuffer, 10, "%s, ", rainValue ? "false" : "true" );
  strncat (sensorDataBuffer, rainBuffer, 10);
  if (serialFlag) {
    Serial.printf("Rain Sensor detection State: %s", rainBuffer);
  }
}
