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