//Project minigarden V2
//Based on arduino customkeypad Example
//Boardtype Esp32 Doit Devkit v1
//Still using procedural programming mindset
/******************************Lib*******************************/
//I2c Display
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//keypad
#include <Keypad.h>
//Async WebServer
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
/*****************************LCD********************************/
LiquidCrystal_I2C lcd(0x3F, 16, 2);
/****************************Keypad******************************/
//define Size of the keybard
const byte ROWS = 4;
const byte COLS = 4;
//map keys to Input
char hexaKeys[ROWS][COLS] = {
    {'0', '1', '2', '3'},
    {'4', '5', '6', '7'},
    {'8', '9', 'A', 'B'},
    {'C', 'D', 'E', 'F'}};
//Define Keypad connection pins 
byte rowPins[ROWS] = {13, 12, 14, 27}; 
byte colPins[COLS] = {26, 25, 33, 32};
//initialize an instance of class NewKeypad 
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
/*********************Serial Connection**************************/
#define RXD2 16
#define TXD2 17
/******************************Wifi******************************/
// WLAN login
const char *ssid = "WLAN-339828";
const char *password = "36938296529523707325";

//Define HTTP GET update request parameter
const char *PARAM_INPUT_1 = "output";
const char *PARAM_INPUT_2 = "state";
const char *PARAM_INPUT_3 = "switch";

//Web input flag and data
bool newInput = LOW;
int webinput;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
/******************************Endstop******************************/
//current endstop state and initial state. Not the best implementation, but works for now 
bool endStopState[2];
bool preEndStopState[2] = {HIGH, HIGH};
//endstop and l298n driver connections
int endStopPin[2] = {34, 35};
int driverPin[4] = {23, 19, 18, 5};
int pinState[6] = {LOW, LOW, LOW, LOW, LOW, LOW}; // the current state of driver pins  

int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button
/******************************Website******************************/
const char index_html[] PROGMEM = R"=="rawliteral(
<!DOCTYPE html>
<!-- Using CSS Framwork form W3schools -->
<!-- Currently Running on a Esp32 Devkit V1 -->
<!-- tinkered together by NoMoreNameToUse -->
<!-- https://github.com/NoMoreNameToUse -->

<html>
<title>CROPS Control Center V 0.01 pre alpha test in development</title>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
<link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">

<style>
    html {font-family: Arial;text-align: center;}
    .grid-container {
        width: 100vw;
        height: 100vh;
        display: grid;
        grid-template-columns: 0.6fr 0.5fr 0.3fr 1.2fr 0.8fr 0.4fr;
        grid-template-rows: 0.5fr 1.3fr 1.3fr 1.2fr;
        gap: 0px 0px;
        grid-template-areas:
            "head-bar head-bar head-bar head-bar head-bar head-bar"
            "Side-bar Stream-1 Stream-1 Stream-1 Stream-2 option-bar"
            "Side-bar Stream-1 Stream-1 Stream-1 Stream-3 option-bar"
            "Side-bar Control-1 control-2 control-3 control-4 option-bar";}
    .head-bar {
      grid-area: head-bar;
      background-color: rgb(96, 125, 139);
      text-align: left;
      color: rgb(255, 255, 255);
      border: 2px;
      padding: 15px;}
    .Side-bar {
        grid-area: Side-bar;
        background-color: rgb(139, 194, 139);
    }
    .Control-1 {grid-area: Control-1;}
    .control-2 {grid-area: control-2;}
    .control-3 {grid-area: control-3;text-align: left;}
    .control-4 {grid-area: control-4;}
    .control {background-color: grey;padding: 3px;}
    .Stream-1 {grid-area: Stream-1;padding: 3px;}
    .Stream-2 {grid-area: Stream-2;padding: 3px;}
    .Stream-3 {grid-area: Stream-3;padding: 3px;}
    .option-bar {grid-area: option-bar;background-color: rgb(236, 129, 129);}
    .stylebutton {
        background-color: white;
        border: none;
        padding: 9px 24px;
        text-decoration: none;
        display: inline-block;
        border: 8px solid #ccc;
        border-radius: 6px
    }
    .block {
        position: relative;
        display: inline-block;
        width: 58px;
        height: 58px;
        background-color: #ccc;
        border-radius: 6px;
    }
    .switch {
        position: relative;
        display: inline-block;
        width: 58px;
        height: 58px
    }
    .switch input {display: none}
    .switch {
        position: relative;
        display: inline-block;
        width: 58px;
        height: 58px
    }
    .bbar {
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #ccc;
        border-radius: 6px;
    }
    .bbar:before {
        position: absolute;
        line-height: 40px;
        content: attr(name);
        height: 42px;
        width: 42px;
        left: 8px;
        bottom: 8px;
        background-color: #fff;
        border-radius: 3px
    }

    input:checked+.bbar {
        background-color: #a80505
    }
</style>
<body>
    <div class="grid-container">
        <div class="head-bar">
            <h1> CROPS Control Center</h1>
        </div>
        <div class="Side-bar" >
            <div class="w3-panel w3-topbar w3-bottombar w3-border-green w3-pale-green">
                <p>Still Testing</p>
            </div >
            <p style="color: white;padding-left: 20px;text-align: left;">The CROPS Project:</p>
                <div style="color: white; padding-left: 60px; text-align: left; line-height: 3px;">
                <p>C omplicated</p>
                <p>R emote</p>
                <p>O perated</p>
                <p>P lanting</p>
                <p>S ystem</p>
            </div>
        </div>
        <div class="Control-1 control">
        %BUTTONPLACEHOLDER%
        </div>
        <div class="control-3 control">
            <p style="color: white; display:inline; padding-right: 8px;">grabber</p>
            <button class="stylebutton" onclick="toggleButton(this)" id="6">open</button>
            <button class="stylebutton" onclick="toggleButton(this)" id="7">close</button>
            <button class="stylebutton" onclick="toggleButton(this)" id="8">r +</button>
            <button class="stylebutton" onclick="toggleButton(this)" id="9">r -</button>
            <p style="line-height: 41px; color: white;">Programms</p>
            <button class="stylebutton" onclick="toggleButton(this)" id="10">Home</button>
            <button class="stylebutton" onclick="toggleButton(this)" id="11">Water</button>
            <button class="stylebutton" onclick="toggleButton(this)" id="12">light</button>
            <button class="stylebutton" onclick="toggleButton(this)" id="13">heating</button>
        </div>
        <div class="control-4 control">
            <p style="color: white;">Esp32-Cam Stream</p>
            <button onclick="still();">Get Still</button>
            <button onclick="stream();">start Stream</button>
            <button onclick="stop();">stop Stream</button>
            <p style="color: white;">IP camera Stream</p>
            <button onclick="streammain();">start Stream</button>
            <button onclick="stopmain();">stop Stream</button>
        </div>
        <div class="Stream-1">
            <img id="stream1" src="https://i.kym-cdn.com/entries/icons/original/000/013/564/doge.jpg"
                style="height: 100%%; width: 100%%">
        </div>
        <div class="Stream-2">
            <img id="stream2" src="https://i.kym-cdn.com/entries/icons/original/000/013/564/doge.jpg" style="max-height: 100%%; max-width: 100%%" >
        </div>
        <div class="Stream-3">
            <iframe src="http://192.168.2.150" height="100%%" width="100%%" title="camera control"></iframe>
        </div>
        <div class="option-bar">
            <div class="w3-panel w3-topbar w3-bottombar w3-border-red w3-pale-red">
                <p>Sensor & Stats</p>
              </div>
              <div class="w3-panel w3-pale-red">
                <p>comming soon</p>
              </div>
        </div>
</body>
<script>function toggleCheckbox(element) {
        var xHR = new XMLHttpRequest();
        if (element.checked) { xHR.open("GET", "/update?output=" + element.id + "&state=1", true); }
        else { xHR.open("GET", "/update?output=" + element.id + "&state=0", true); }
        xHR.send();
    }
    function toggleButton(element) {
        var xHR = new XMLHttpRequest();
        xHR.open("GET", "/update?switch=" + element.id, true);
        xHR.send();
    }
    function still() { var img = document.getElementById("stream2"); img.src = `http://192.168.2.150/capture?_cb=${Date.now()}`; return false; }
    function stream() { var img = document.getElementById("stream2"); img.src = `http://192.168.2.150:81/stream`; return false; }
    function stop() { var img = document.getElementById("stream2"); img.src = `https://i.kym-cdn.com/entries/icons/original/000/013/564/doge.jpg`; return false; }
    function streammain() { var img = document.getElementById("stream1"); img.src = `http://192.168.2.142:8080/video`; return false; }
    function stopmain() { var img = document.getElementById("stream1"); img.src = `https://i.kym-cdn.com/entries/icons/original/000/013/564/doge.jpg`; return false; }
</script>
</html>
)=="rawliteral";

/******************************Setup******************************/
void setup()
{
  Serial.begin(115200);
  //Serial Connection to arduino nano on bridge 
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  //pinmode definition
  for (int i = 0; i < 2; i++)
  {
    pinMode(endStopPin[i], INPUT);
  }
  for (int i = 0; i < 4; i++)
  {
    pinMode(driverPin[i], OUTPUT);
  }
  //Start 1602 LCD 
  lcd.begin();
  lcd.backlight(); 
  // Connect to Wi-Fi
  lcd.print("Connecting...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  // Route for root / web page (send website to chient)
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // when a GET request communication incoming 
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    String inputMessage3;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2))
    {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      remoteControl(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else if (request->hasParam(PARAM_INPUT_3))
    {
      inputMessage3 = request->getParam(PARAM_INPUT_3)->value();
      webinput = inputMessage3.toInt();
      newInput = HIGH;
    }
    else
    {
      Serial.println("Invalid GET request");
    }
    request->send(200, "text/plain", "OK");
  });
  // Start Async webserver
  server.begin();
}
/******************************Loop*******************************/
void loop()
{
  buttoncontrol();
  webcontrol();
  checkIfAtEnd();
}
/*********************web control handler*************************/
//Inefficiet but should work. Will Sort it out later
void remoteControl(int control, int state)
{
  //quick and dirty if else switch :D 
  //angular coordinate phi +
  pinState[control] = state;
  if (control == 0)
  {
    Serial2.println(0);
  }
  //L Axis backward
  else if (control == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print("Web L Axis back");
    //Motor forward
    pinState[0] = state;
    digitalWrite(driverPin[0], pinState[0]);
    preEndStopState[0] = HIGH;
  }
  //L Axis forward
  else if (control == 2)
  {
    lcd.setCursor(0, 1);
    lcd.print("Web L Axis forw.");
    pinState[1] = state;
    digitalWrite(driverPin[1], pinState[1]);
    preEndStopState[1] = HIGH;
  }
  //angular coordinate phi -
  else if (control == 3)
  {
    Serial2.println(1);
  }
  //polar axis +
  else if (control == 4)
  {
    Serial2.println(2);
  }
  //polar axis -
  else if (control == 5)
  {
    Serial2.println(3);
  }
  else{
    Serial.println("Invalid button state");
  }
}
// Replaces placeholder with current button html 
String processor(const String &var)
{
  //Serial.println(var);
  if (var == "BUTTONPLACEHOLDER")
  {
    String buttons = "";
    buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"0\"" + switchState(0) + "><span class=\"bbar\" name=\"^\"></span></label><br>\n";
    buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"1\"" + switchState(1) + "><span class=\"bbar\" name=\"<\"></span></label>\n";
    buttons += "<label class=\"block\"></label>\n";
    buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\"" + switchState(2) + "><span class=\"bbar\" name=\">\"></span></label><br>\n";
    buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"3\"" + switchState(3) + "><span class=\"bbar\" name=\"v\"></span></label>\n";
    buttons += "</div>\n<div class=\"control-2 control\">\n";
    buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\"><span class=\"bbar\" name=\"^\"></span></label>\n";
    buttons += "<p style=\"line-height: 34px; color: rgb(255, 255, 255);\">r Axis</p>\n";
    buttons += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\"><span class=\"bbar\" name=\"v\"></span></label>";
    return buttons;
  }
  return String();
}
//reutrn current pin state 
String switchState(int switchPin)
{
  if (pinState[switchPin])
  {
    return "checked";
  }
  else
  {
    return "";
  }
}
//Inefficiet but should work. Will Sort it out later
/********************web control processor***********************/
void webcontrol()
{
  //quick and dirty if else switch ;D 
  if (newInput == HIGH)
  {
    Serial.println(webinput);
    //grabber open
    if (webinput == 6)
    {
      Serial2.println('C');
    }
    //grabber close
    else if (webinput == 7)
    {
      Serial2.println('D');
    }
    //grabber turn +
    else if (webinput == 8)
    {
      Serial2.println('E');
    }
    //grabber turn -
    else if (webinput == 9)
    {
      Serial2.println('F');
    }
    //Homing
    else if (webinput == 10)
    {
      Home();
    }
    else
    {
      Serial.println("not yet implemented");
    }
    //reset flag
    newInput = LOW;
  }
}
/*****************keypad control processor***********************/
void buttoncontrol()
{
  //read and execute command recieved from 4x4 button pad
  char customKey = customKeypad.getKey();
  if (customKey)
  {
    //L Axis backward
    if (customKey == '4')
    {
      lcd.setCursor(0, 1);
      lcd.print("L Axis backward");
      switchMotor(0);
      preEndStopState[0] = HIGH;
    }
    //L Axis backward
    else if (customKey == '5')
    {
      lcd.setCursor(0, 1);
      lcd.print("L Axis foreward");
      switchMotor(1);
      preEndStopState[1] = HIGH;
    }
    //Homing
    else if (customKey == '6')
    {
      Home();
    }
    //send command over serial to arduino
    else
    {
      Serial2.println(customKey);
      Serial.println(customKey);
    }
  }
}
void switchMotor(int Number)
{
  // toggle state of LED
  pinState[Number] = !pinState[Number];
  // control LED arccoding to the toggled state
  digitalWrite(driverPin[Number], pinState[Number]);
  Serial.println(Number);
}
void Home()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Homing...");
  Serial2.println("2");
  delay(2000);
  digitalWrite(driverPin[0], HIGH);
  while (endStopState[0] != LOW)
  {
    char customKey = customKeypad.getKey();
    if (customKey)
    {
      //Abord Homing
      if (customKey == '6')
      {
        digitalWrite(driverPin[0], LOW);
        pinState[0] = LOW;
        return;
      }
    }
    endStopState[0] = digitalRead(endStopPin[0]);
    delay(20);
  }
  // turn Motor via L298N driver off:
  digitalWrite(driverPin[0], LOW);
  pinState[0] = LOW;
  Serial.println("Home X stopped");
  Serial2.println("0");
  for (int i = 0; i < 2400; i++)
  {
    char customKey = customKeypad.getKey();
    if (customKey)
    {
      //Abord Homing
      if (customKey == '6')
      {
        Serial2.println("0");
        return;
      }
    }
    delay(20);
  }
  Serial2.println("0");
  delay(100);
  Serial2.println("1");
  for (int i = 0; i < 650; i++)
  {
    char customKey = customKeypad.getKey();
    if (customKey)
    {
      //Abord Homing
      if (customKey == '6')
      {
        Serial2.println("1");
        return;
      }
    }
    delay(20);
  }
  Serial2.println("1");
  lcd.setCursor(0, 1);
  lcd.print("y Axis Homed");
}
void checkIfAtEnd()
{
  //read endstop state
  endStopState[0] = digitalRead(endStopPin[0]);
  endStopState[1] = digitalRead(endStopPin[1]);
  //check if end is reached, then switch off the motor. 
  if (endStopState[0] != preEndStopState[0])
  {
    if (endStopState[0] == LOW)
    {
      // turn LED on:
      digitalWrite(driverPin[0], LOW);
      Serial.println("stopped0");
      pinState[0] = LOW;
      preEndStopState[0] = endStopState[0];
    }
  }
  if (endStopState[1] != preEndStopState[1])
  {
    if (endStopState[1] == LOW)
    {
      // turn LED on:
      digitalWrite(driverPin[1], LOW);
      Serial.println("stopped1");
      pinState[1] = LOW;
      preEndStopState[1] = endStopState[1];
    }
  }
}
