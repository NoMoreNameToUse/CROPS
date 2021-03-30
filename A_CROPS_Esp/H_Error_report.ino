void errCheck()
  {
   endStopState[0] = digitalRead(endStopPin[0]);
   endStopState[1] = digitalRead(endStopPin[1]); 
   if(endStopState[0] == LOW){
    Serial.println("IM DIEING 1");
   }
   else if(endStopState[1] == LOW){
    Serial.println("IM DIEING 2");
   }
  }
    
    
