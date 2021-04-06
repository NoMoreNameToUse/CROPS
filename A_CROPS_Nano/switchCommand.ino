void switchCommand() {
  if (strcmp (serialStr, "PFW") == 0) {
    //Phi axis For-Ward
    switchMotor(0);
  }
  else if (strcmp (serialStr, "PBW") == 0) {
    //Phi axis Back-Ward
    switchMotor(1);
    Serial.print("PBW");
  }
  else if (strcmp (serialStr, "RFW") == 0) {
    //R axis For-Ward
    switchMotor(2);
    preEndStopState[0] = HIGH;
  }
  else if (strcmp (serialStr, "RBW") == 0) {
    //R axis Back-Ward
    switchMotor(3);
    preEndStopState[1] = HIGH;
  }
  else if (strcmp (serialStr, "RST") == 0) {
    // reset driver
    for (int i = 0; i < 4; i++) {
      pinState[i] = LOW;
      digitalWrite(driverPin[i], LOW);
    }
    preEndStopState[0] = HIGH;
    preEndStopState[1] = HIGH;
  }
  else if (strcmp (serialStr, "S1L") == 0) {
    //Servo 1 turn Left
    servoPos[0] = servoPos[0] + 10;
    Servo1.write(servoPos[0]);
  }
  else if (strcmp (serialStr, "S1R") == 0) {
    //Servo 1 turn Right
    servoPos[0] = servoPos[0] - 10;
    Servo1.write(servoPos[0]);
  }
  else if (strcmp (serialStr, "S2L") == 0) {
    //Servo 2 turn Left
    servoPos[1] = servoPos[1] + 10;
    Servo2.write(servoPos[1]);
  }
  else if (strcmp (serialStr, "S2R") == 0) {
    //Servo 2 turn Right
    servoPos[1] = servoPos[1] - 10;
    Servo2.write(servoPos[1]);
  }
  else if (strcmp (serialStr, "S1S") == 0) {
    //Set Servo 1 Angle
    Servo1.write(intData1);
  }
  else if (strcmp (serialStr, "S2S") == 0) {
    //Set Servo 2 Angle
    Servo2.write(intData1);
    Serial.println("S2S");
    Serial.println(intData1);
  }
}
