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
    else
    {
        Serial.println("Invalid button state");
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
        else if (webinput == 11)
        {
            lcd.setCursor(0, 1);
            lcd.print("Watering");
            utilityState[0] = !utilityState[0];
            digitalWrite(driverPin[2], utilityState[0]);
        }
        else if (webinput == 12)
        {
            lcd.setCursor(0, 1);
            lcd.print("LED");
            utilityState[1] = !utilityState[1];
            digitalWrite(driverPin[3], utilityState[1]);
        }
        else
        {
            Serial.println("not yet implemented");
        }
        //reset flag
        newInput = LOW;
    }
}
