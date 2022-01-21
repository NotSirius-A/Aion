typedef struct HumanTime {
  uint32_t milliseconds;
  uint8_t hours, minutes, seconds;
  char stringFormat[20];
};

int32_t getTimeUntilTransition(unsigned long now, uint32_t lastTransitionTime, uint8_t currentState, uint32_t stateLengths[]) {
  int32_t rv = stateLengths[currentState] - (now - lastTransitionTime);
  return rv;
}

void convertMillisToHumanFormat(int x, uint8_t &hours, uint8_t &minutes, uint8_t &seconds) {
  /* Convert time in miliseconds and put directly into provided variables */
  x = x / 1000;
  hours = x / 3600;
  minutes = (x / 60) - (hours * 60);
  seconds = x - (minutes * 60) - (hours * 3600);
}

void printTimeToLCD(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  Serial.print(hours);
  Serial.print(" ");
  Serial.print(minutes);
  Serial.print(" ");
  Serial.println(seconds);
}
