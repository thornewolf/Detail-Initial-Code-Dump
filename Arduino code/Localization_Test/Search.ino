int j = 0;
void sweep() {      
       if (pitchPos != 0) {
        if (pitchPos > 0) {
          digitalWrite(PD,LOW);
          pitchPos--;
        }
        if (pitchPos < 0) {
          digitalWrite(PD,HIGH);
          pitchPos++;
        }
        digitalWrite(PS,HIGH);
        delayMicroseconds(Speed);
        digitalWrite(PS,LOW);
        delayMicroseconds(Speed);
       }

       if (pitchPos == 0) {
        //step motor
        digitalWrite(YD,HIGH)
        digitalWrite(YS,HIGH);
        delayMicroseconds(Speed);
        digitalWrite(YS,LOW);
        delayMicroseconds(Speed);
        yawPos++
      }
}
