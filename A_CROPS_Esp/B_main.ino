/******************************Setup******************************/
void setup()
{
    Serial.begin(115200);
    //Serial Connection to arduino nano on bridge
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    //pinmode definition
    for (int i = 0; i < 2; i++)
    {
        pinMode(endStopPin[i], INPUT_PULLUP);
    }
    for (int i = 0; i < 4; i++)
    {
        pinMode(driverPin[i], OUTPUT);
    }
    //Start 1602 LCD
    lcd.begin();
    lcd.backlight();
    
    // Basic Debug Information 
    Serial.println("#####CROP System --- Boardtype DOIT ESP32 DEVKIT V1#####");
    Serial.println("/// Software Version v0.2 pre Alpha Test ///");
    Serial.println("/// Serial Configuration:/// \n  Serial 1 -- PC Serial \n  Serial 2 -- Arduino Uno");
    Serial.println("More Info @ https://github.com/NoMoreNameToUse/CROPS");
    
    // Connect to Wi-Fi
    lcd.print("Connecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    
    // Print ESP Local IP Address
    Serial.println("Local network IP Address:");
    Serial.println(WiFi.localIP());
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Local IP Address");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());

    // Route for root / web page (send website to client)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html, processor);
        Serial.println("Recieved new HTML request");
    });

    // Handling incoming GET request  
    server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
        String inputMessage1;
        String inputMessage2;
        String inputMessage3;
        // GET input value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
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
