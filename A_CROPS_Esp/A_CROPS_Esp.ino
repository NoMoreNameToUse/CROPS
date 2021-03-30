
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
int utilityState[2] = {LOW, LOW};
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
