void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  for (int i = 0; i < 3; i++) {
    pinMode(endStopPin[i], INPUT);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(driverPin[i], OUTPUT);
  }
  Servo1.attach(servoPin[0]);
  Servo2.attach(servoPin[1]);
}
void loop() {
  //check endstop
  checkIfAtEnd();
  // check if string is complete (newline)
  if (stringComplete) {
    if (inputString == "0\r\n") {
      switchMotor(0);
    }
    else if (inputString == "1\r\n") {
      switchMotor(1);
    }
    else if (inputString == "2\r\n") {
      switchMotor(2);
      preEndStopState[0] = HIGH;
    }
    else if (inputString == "3\r\n") {
      switchMotor(3);
      preEndStopState[1] = HIGH;
    }
    else if (inputString == "8\r\n") {
      // reset driver
      for (int i = 0; i < 4; i++) {
        pinState[i] = LOW;
        digitalWrite(driverPin[i], LOW);
      }
      preEndStopState[0] = HIGH;
      preEndStopState[1] = HIGH;
    }
    else if (inputString == "C\r\n") {
      servoPos[0] = servoPos[0] + 10;
      Servo1.write(servoPos[0]);
    }
    else if (inputString == "D\r\n") {
      servoPos[0] = servoPos[0] - 10;
      Servo1.write(servoPos[0]);
    }
    else if (inputString == "E\r\n") {
      servoPos[1] = servoPos[1] + 10;
      Servo2.write(servoPos[1]);
    }
    else if (inputString == "F\r\n") {
      servoPos[1] = servoPos[1] - 10;
      Servo2.write(servoPos[1]);
    }
    // Serial.println(inputString);
    //Serial.println(preEndStopState[0], preEndStopState[1]);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}
