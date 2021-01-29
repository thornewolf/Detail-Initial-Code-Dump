void track() {
   
   if (yaw == 2) {              //turn right
    digitalWrite(YD,HIGH);
    digitalWrite(YS,HIGH);
    yawPos++;
   }
   if (yaw == 0) {              //turn left
    digitalWrite(YD,LOW);
    digitalWrite(YS,HIGH);
    yawPos--;
   }
   if (pitch == 2) {            //pitch up
    if (pitchPos <= 20) {
    digitalWrite(PD,HIGH);
    digitalWrite(PS,HIGH);
    pitchPos++;
   }
   }
   if (pitch == 0) {            //pitch down
    if (pitchPos >= -20) {
    digitalWrite(PD,LOW);
    digitalWrite(PS,HIGH);
    pitchPos--;
   }
   }
   
  delayMicroseconds(Speed);
  digitalWrite(YS,LOW);
  digitalWrite(PS,LOW);
  delayMicroseconds(Speed);
  yaw = 1;
  pitch = 1;
 }
