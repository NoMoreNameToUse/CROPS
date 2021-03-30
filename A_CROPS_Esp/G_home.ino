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
    for (int i = 0; i < 2300; i++)
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
    for (int i = 0; i < 640; i++)
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
