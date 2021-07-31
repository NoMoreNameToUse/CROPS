/*
  Error code implementation:
  Range [0, 65,535]
  Int:   [XX]          [X]        [XX]
  Error source  Severity   Error type

  Error source:
  0X: System Error
  1X: Sensor
  2X: Serial
*/

void error(unsigned int errorCode) {

  char Error_Message[16];
  char Error_Description[16];
  char Error_Detailed[120];

  sprintf(Error_Message, "#%d ", errorCode);


  //I tried to do something modular here with struct to get rid of the repitition, but it seems to be overcomplicated and difficult to implement.
  //So ill stick with a gigantic switch statement for now :D

  switch (errorCode) {
    case 01000:
      strncat(Error_Message, "ABN_M5V_V", IndexEM);
      strncpy(Error_Description, "Abnorm. M5v Volt", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #CRIT# Crit: 5v main bus voltage abnormal: %f Volt, not in range of [%f,%f] \r\n", errorCode, ina219_5VData.busvoltage, mainCon_VMax, mainCon_VMin);
      break;
    case 01001:
      strncat(Error_Message, "ABN_M5V_C", IndexEM);
      strncpy(Error_Description, "Abnorm. M5v Cur.", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #CRIT# Crit: 5v main bus current draw abnormal: %f mA, exceed max current of %f mA  \r\n", errorCode, ina219_5VData.current_mA, mainCon_CMax);
      break;
    case 02000:
      strncat(Error_Message, "ABN_M12VV", IndexEM);
      strncpy(Error_Description, "Abnorm M12v Volt", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #CRIT# Crit: 12v main bus voltage abnormal: %f Volt, not in range of [%f,%f] \r\n", errorCode, ina219_12VMData.busvoltage, main12V_VMax, main12V_VMin);
      break;
    case 02001:
      strncat(Error_Message, "ABN_M12VC", IndexEM);
      strncpy(Error_Description, "Abnorm. M12v Cur", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #CRIT# Crit: 12v main bus current draw abnormal: %f mA, exceed max current of %f mA \r\n", errorCode, ina219_12VMData.current_mA, main12V_CMax );
      break;
    case 03000:
      strncat(Error_Message, "ERR_TimeN", IndexEM);
      strncpy(Error_Description, "NTP g. Time Fail", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #CRIT# Crit: failed to get time from ntp server \r\n", errorCode);
      break;
    case 03001:
      strncat(Error_Message, "ERR_TimeG", IndexEM);
      strncpy(Error_Description, "Fail to get Time", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #WARN# Warning: failed to get time \r\n", errorCode);
      break;
    case 04000:
      strncat(Error_Message, "ERR_SDC_M", IndexEM);
      strncpy(Error_Description, "Card Mount Fail.", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #CRIT# Crit: SD Card Mount Failed \r\n", errorCode);
      break;
    case 04001:
      strncat(Error_Message, "ERR_SDC_N", IndexEM);
      strncpy(Error_Description, "No SD Card Insert", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #CRIT# Crit: no SD Card Inserted \r\n", errorCode);
      break;
    case 05000:
      strncat(Error_Message, "WAR_INANS", IndexEM);
      strncpy(Error_Description, "INA read handl n", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #Warn# Warning: INA219 read function recived unknown input \r\n", errorCode);
      break;
    case 05001:
      strncat(Error_Message, "WAR_DHTNS", IndexEM);
      strncpy(Error_Description, "DHT read handl n", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #Warn# Warning: DHT22 read function recived unknown input \r\n", errorCode);
      break;
    // #10500 #11500 #12500: INI219 I2C Initiation failure
    case 10500:
      strncat(Error_Message, "INI_INA5V", IndexEM);
      strncpy(Error_Description, "INA 5v Init Fail", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #CRIT# Failed to Initiate INA219 main 5v power bus chip on I2C Bus \r\n", errorCode);
      break;
    case 11500:
      strncat(Error_Message, "INI_INAMB", IndexEM);
      strncpy(Error_Description, "INA MB Init Fail", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #CRIT# Failed to Initiate INA219 main 12v power bus chip on I2C Bus \r\n", errorCode);
      break;
    case 12500:
      strncat(Error_Message, "INI_INABP", IndexEM);
      strncpy(Error_Description, "INA BP Init Fail", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #CRIT# Failed to Initiate INA219 bridge 12v power bus chip on I2C Bus \r\n", errorCode);
      break;
    case 13200:
      strncat(Error_Message, "VAL_MOIS1", IndexEM);
      strncpy(Error_Description, "M1 read Abnormal", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #WARN# Moisture Sensor nr1 reading Abmormal:%d, not in range of [%d,%d] \r\n", errorCode, moistureValue, MMax[0], MMin[0]);
      break;
    case 13201:
      strncat(Error_Message, "VAL_MOIS2", IndexEM);
      strncpy(Error_Description, "M2 read Abnormal", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #WARN# Moisture Sensor nr2 reading Abmormal:%d, not in range of [%d,%d] \r\n", errorCode, moistureValue, MMax[1], MMin[1]);
      break;
    case 13202:
      strncat(Error_Message, "VAL_MOIS3", IndexEM);
      strncpy(Error_Description, "M3 read Abnormal", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #WARN# Moisture Sensor nr3 reading Abmormal:%d, not in range of [%d,%d] \r\n", errorCode, moistureValue, MMax[2], MMin[2]);
      break;
    case 14200:
      strncat(Error_Message, "VAL_LUXRD", IndexEM);
      strncpy(Error_Description, "Light S. R. Abn.", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #WARN# Light sensor reading Abmormal:%d, not in range of [%d,%d] \r\n", errorCode, sensorValue, SMax[0], SMin[0]);
      break;
    case 14201:
      strncat(Error_Message, "VAL_TEMPR", IndexEM);
      strncpy(Error_Description, "Temp. S. R. Abn.", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #WARN# Resistive tempreature probe reading Abmormal:%d, not in range of [%d,%d] \r\n", errorCode, sensorValue, SMax[1], SMin[1]);
      break;
    case 14202:
      strncat(Error_Message, "VAL_WATLR", IndexEM);
      strncpy(Error_Description, "Watlv read Abn.", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #WARN# Water level Sensor reading Abmormal:%d, not in range of [%d,%d] \r\n", errorCode, sensorValue, SMax[2], SMin[2]);
      break;
    case 14203:
      strncat(Error_Message, "VAL_RainS", IndexEM);
      strncpy(Error_Description, "RainS read Abnormal", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #WARN# Rain Sensor reading Abmormal:%d, not in range of [%d,%d] \r\n", errorCode, sensorValue, SMax[3], SMin[3]);
      break;
    default:
      strncat (Error_Message, "Not Found", IndexEM);
      strncpy (Error_Description, "Error not found ", IndexED);
      snprintf(Error_Detailed, 120, "[ERR:%d] #SYSE# Failed to Identify the Error Code: Not in Registry \r\n", errorCode);
      break;
  }
  Serial.print(Error_Detailed);
  if (sdConnected) {
    appendFile(SD, logDir, Error_Detailed);
  } else {
    strncat(logBuffer, Error_Detailed , 120);
  }
}
