void switchMotor(int Number) {
  // toggle state of driver pin
  pinState[Number] = !pinState[Number];
  // control motor arccoding to the toggled state
  digitalWrite(driverPin[Number], pinState[Number]);
  Serial.println(Number);
}
