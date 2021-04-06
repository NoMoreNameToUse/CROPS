void serial1CommHandler() {
    serial1Reciever();
    if (serial1newData == true) {
        strcpy(tempChars, serial1Recieved);
        parseData();
        proccessParsedData();
        serial1newData = false;
    }
}

void serial1Reciever() {
    static boolean Serial1InProgress = false;
    static byte S1CharIndex = 0;
    char startMarker = '<';
    char endMarker = '>';
    char S1RecievedChar;

    while (Serial.available() > 0 && serial1newData == false) {
        S1RecievedChar = Serial.read();

        if (Serial1InProgress == true) {
            if (S1RecievedChar != endMarker) {
                serial1Recieved[S1CharIndex] = S1RecievedChar;
                S1CharIndex++;
                if (S1CharIndex >= maxCLength) {
                    S1CharIndex = maxCLength - 1;
                }
            }
            else {
                serial1Recieved[S1CharIndex] = '\0'; // terminate the string
                Serial1InProgress = false;
                S1CharIndex = 0;
                serial1newData = true;
            }
        }

        else if (S1RecievedChar == startMarker) {
            Serial1InProgress = true;
        }
    }
}

void parseData() {
    char * strtokIndx; // this is used by strtok() as an index
    
    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(serial1Str, strtokIndx); // copy it to messageFromPC
    Serial.println(serial1Str);

    if(strcmp (serial1Str, "DHT") == 0){
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    floatData1 = atof(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    floatData2 = atof(strtokIndx);     // convert this part to a float
    CurrentTime = micros();
    unsigned long ElapsedTime = CurrentTime - StartTime;
    StartTime = CurrentTime;
    Serial.println(ElapsedTime);
    }
}
void proccessParsedData() {
    Serial.print("Command ");
    Serial.println(serial1Str);
    Serial.print("Temp: ");
    Serial.println(floatData1);
    Serial.print("Humidity: ");
    Serial.println(floatData2);
}
