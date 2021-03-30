void switchMotor(int Number)
{    // toggle state of LED
    pinState[Number] = !pinState[Number];
    // control LED arccoding to the toggled state
    digitalWrite(driverPin[Number], pinState[Number]);
    Serial.println("Motor Switched: ");
    Serial.println(Number);
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
            digitalWrite(driverPin[0], LOW);
            Serial.println("ENDSTOP: L Axis forward Stopped");
            Serial.println(endStopState[0]);
            Serial.println(preEndStopState[0]);
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
            Serial.println("ENDSTOP: L Axis backward Stopped");
            Serial.println(endStopState[1]);
            Serial.println(preEndStopState[1]);
            pinState[1] = LOW;
            preEndStopState[1] = endStopState[1];
            
        }
    }
}
