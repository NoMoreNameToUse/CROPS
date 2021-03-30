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
        else if (customKey == '7')
        {
            lcd.setCursor(0, 1);
            lcd.print("Watering");
            utilityState[0] = !utilityState[0];
            digitalWrite(driverPin[2], utilityState[0]);
        }
        else if (customKey == '8')
        {
            lcd.setCursor(0, 1);
            lcd.print("LED");
            utilityState[1] = !utilityState[1];
            digitalWrite(driverPin[3], utilityState[1]);
        }
        //send command over serial to arduino
        else
        {
            Serial2.println(customKey);
            Serial.println(customKey);
        }
    }
}
