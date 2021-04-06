void setup() {
  // initialize serial:
  Serial.begin(9600);
  // pinMode definition
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
  serialCommHandler();
  if (stringComplete) {
    switchCommand();
    stringComplete = false;
  }
}
