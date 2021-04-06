void serialCommHandler() {
    serialReciever();
    if (serialnewData == true) {
        strcpy(tempChars, serialRecieved);
        parseData();
        serialnewData = false;
    }
}

void serialReciever() {
    static boolean SerialInProgress = false;
    static byte CharIndex = 0;
    char startMarker = '<';
    char endMarker = '>';
    char RecievedChar;

    while (Serial.available() > 0 && serialnewData == false) {
        RecievedChar = Serial.read();

        if (SerialInProgress == true) {
            if (RecievedChar != endMarker) {
                serialRecieved[CharIndex] = RecievedChar;
                CharIndex++;
                if (CharIndex >= maxCLength) {
                    CharIndex = maxCLength - 1;
                }
            }
            else {
                serialRecieved[CharIndex] = '\0'; // terminate the string
                SerialInProgress = false;
                CharIndex = 0;
                serialnewData = true;
            }
        }

        else if (RecievedChar == startMarker) {
            SerialInProgress = true;
        }
    }
}

void parseData() {
    char * strtokIndx; // this is used by strtok() as an index
    
    strtokIndx = strtok(tempChars,",");
    strcpy(serialStr, strtokIndx); 
    if(strcmp (serialStr, "GETDHT") == 0){
      getDHTData();
    }
    else if(strcmp (serialStr, "SYN") == 0){
      Serial.print("<ACK>");
    }
}
