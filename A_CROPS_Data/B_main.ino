void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
serial1CommHandler();
serialReciever();
}
