

bool getCurrentTime() {
  if (!getLocalTime(&timeinfo)) {
    error(03001);
    return false;
  }
  else {
    return true;
  }
}

void printLocalTime() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time, tring again...");
    if (!getLocalTime(&timeinfo)) {
      error(03000);
      stopOrWarning();
    }
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}
