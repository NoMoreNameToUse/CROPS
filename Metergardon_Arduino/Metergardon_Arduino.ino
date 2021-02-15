//Project CROPS V2
//A noobly approach with procedural programming using c/c++
//Boardtype Arduino Nano with old bootloader
/******************************Lib******************************/
#include <Servo.h> 
/************************Endstop & Control**********************/
String inputString = "";      // a String to hold incoming serial data
bool stringComplete = false;  // whether the string is complete
int pinState[4] = {LOW, LOW, LOW, LOW};  // the current state of LED
int endStopPin[3] = {6, 7, 8};
int driverPin[4] = {2, 3, 4, 5};
int servoPin[2] = {10, 11};
int servoPos[2] = {90, 90};
int endStopState[3];
int preEndStopState[3] = {HIGH, HIGH, HIGH};
int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button
// Create servo object
Servo Servo1;
Servo Servo2;
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

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
void switchMotor(int Number) {
  // toggle state of driver pin
  pinState[Number] = !pinState[Number];
  // control LED arccoding to the toggled state
  digitalWrite(driverPin[Number], pinState[Number]);
  Serial.println(Number);
}
void checkIfAtEnd() {
  // read the state of the pushbutton value:
  endStopState[0] = digitalRead(endStopPin[0]);
  endStopState[1] = digitalRead(endStopPin[1]);
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (endStopState[0] != preEndStopState[0]) {
    if (endStopState[0] == LOW) {
      // turn LED on:
      digitalWrite(driverPin[2], LOW);
      Serial.println("stopped");
      pinState[2] = LOW;
      preEndStopState[0] = endStopState[0] ;
    }
  }
  if (endStopState[1] != preEndStopState[1]) {
    if (endStopState[1] == LOW) {
      // turn LED on:
      digitalWrite(driverPin[3], LOW);
      Serial.println("stopped");
      pinState[3] = LOW;
      preEndStopState[1] = endStopState[1];
    }
  }
}
