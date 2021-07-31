void powerMonitor() {
  unsigned long currentMillis = millis();
  if (currentMillis - monitorMillis >= 500) {
    monitorMillis = currentMillis;

    ina219_Read('C');
    if (ina219_5VData.ifConnected) {
      if (ina219_5VData.busvoltage > mainCon_VMax || ina219_5VData.busvoltage < mainCon_VMin) {
        digitalWrite(RelayPin[0], LOW);
        error(01000);
        buzzStop();
      }
      else if (ina219_5VData.current_mA > mainCon_CMax) {
        digitalWrite(RelayPin[0], LOW);
        error(01001);
        buzzStop();
      }
    }
    ina219_Read('M');
    if (ina219_5VData.ifConnected) {
      if (ina219_12VMData.busvoltage > main12V_VMax || ina219_12VMData.busvoltage < main12V_VMin) {
        digitalWrite(RelayPin[0], LOW);
        error(02000);
        buzzStop();
      }
      else if (ina219_12VMData.current_mA > main12V_CMax) {
        digitalWrite(RelayPin[0], LOW);
        error(02001);
        buzzStop();
      }
    }
  }
}
