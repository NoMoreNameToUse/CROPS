void buzzHandler() {
  if (buzzFlag[0]) {
    unsigned long currentMillis = millis();
    if (currentMillis - buzzMillis >= 1000) {
      buzzMillis = currentMillis;
      digitalWrite(BuzzerPin, !buzzerState);
    }
  }
}

void buzzStop() {
  while (true) {
    for (int i = 0; i < 5; i++) {
      digitalWrite(BuzzerPin, HIGH);
      delay(200);
      digitalWrite(BuzzerPin, LOW);
      delay(200);
    }
    delay(2000);
  }
}


void stopOrWarning() {
  if (stopOrWarningFlag) {
    buzzStop();
  } else {
    buzzFlag[0] = true;
  }
}
