// check if endstop is pressed. Stop the motor when being pressed
void checkIfAtEnd() {
  endStopState[0] = digitalRead(endStopPin[0]);
  endStopState[1] = digitalRead(endStopPin[1]);
  if (endStopState[0] != preEndStopState[0]) {
    if (endStopState[0] == LOW) {
      digitalWrite(driverPin[2], LOW);
      Serial.println("stopped");
      pinState[2] = LOW;
      preEndStopState[0] = endStopState[0] ;
    }
  }
  if (endStopState[1] != preEndStopState[1]) {
    if (endStopState[1] == LOW) {
      digitalWrite(driverPin[3], LOW);
      Serial.println("stopped");
      pinState[3] = LOW;
      preEndStopState[1] = endStopState[1];
    }
  }
}
