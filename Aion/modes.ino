void handleMainMode(unsigned long timeNow, int32_t & lastTransitionTime , uint8_t & currentState) {
  uint32_t stateLengths[] = STATE_LENGTHS;
  size_t numOfStates = sizeof(stateLengths)/sizeof(*stateLengths);

  int32_t timeUntilTransition = getTimeUntilTransition(timeNow, lastTransitionTime , currentState, stateLengths);
  uint8_t hours, minutes, seconds;
  convertMillisToHumanFormat(timeUntilTransition, hours, minutes, seconds);
  
  Serial.print(hours);
  Serial.print(" ");
  Serial.print(minutes);
  Serial.print(" ");
  Serial.println(seconds);

  
  if (timeUntilTransition < 0) {
    lastTransitionTime = timeNow;
    currentState++;
    if (currentState >= numOfStates) {
      currentState = 0; 
    }
  }
}
