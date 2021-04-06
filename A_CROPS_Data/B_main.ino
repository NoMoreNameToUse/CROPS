void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
serialCommHandler();
serialReciever();
}
