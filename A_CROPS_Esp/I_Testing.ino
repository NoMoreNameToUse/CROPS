void errCheck()
{
  endStopState[0] = digitalRead(endStopPin[0]);
  endStopState[1] = digitalRead(endStopPin[1]);
  if (endStopState[0] == LOW) {
    Serial.println("IM DIEING 1");
  }
  else if (endStopState[1] == LOW) {
    Serial.println("IM DIEING 2");
  }
}

String readDHTTemperature() {
StartTime = micros();
Serial.println("<GETDHT11>");
//return String(floatData1);
String SensorData;
SensorData += String(floatData1) + "," + String(floatData2);
return SensorData;
}
