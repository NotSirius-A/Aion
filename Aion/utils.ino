int32_t getTimeUntilTransition(unsigned long now, uint32_t lastTransitionTime, uint8_t currentState, State states[]) {
  int32_t rv = states[currentState].period - (now - lastTransitionTime);
  return rv;
}

HumanTime convertMillisToHumanFormat(int32_t x) {
  x = x / 1000;
  uint8_t hours = x / 3600;
  uint8_t minutes = (x / 60) - (hours * 60);
  uint8_t seconds = x - (minutes * 60) - (hours * 3600);

  HumanTime rv = {x, seconds, minutes, hours};
  
  /*
    Time should be displayed in a format h*:mm:ss, below code is to convert
    something like 5 seconds to '05', so it adds '0' as padding when seconds/minutes is less than 10.
    Better algorithm is certainly possible, but this is easy and simple.
  */

  char secondsString[3];
  if (seconds < 10) {
    sprintf(secondsString, "0%hu", seconds);
  } else {
    sprintf(secondsString, "%hu", seconds);
  }

  char minutesString[3];
  if (minutes < 10) {
    sprintf(minutesString, "0%hu", minutes);
  } else {
    sprintf(minutesString, "%hu", minutes);
  }

  sprintf(rv.stringFormat, "%hu:%2s:%2s", hours, minutesString, secondsString);

  return rv;
  
}


uint8_t getUpdatedCurrentMode (uint8_t currentMode, unsigned long & debounceTimeLast, uint16_t & prevModeButtonState) {
  unsigned long timeNow = millis();
  
  if ((timeNow - debounceTimeLast) > DEBOUNCE_PERIOD) {
    uint16_t modeButtonState = digitalRead(MODE_BUTTON);
    debounceTimeLast = timeNow;

    if (modeButtonState == HIGH && prevModeButtonState == LOW) {
      currentMode++;
      if (currentMode >= NUM_OF_MODES) {
        currentMode = 0;
      }
    }
    prevModeButtonState = modeButtonState;
  }

  return currentMode;

}


uint8_t getUpdatedCurrentStateEdit (uint8_t currentStateEdit, unsigned long & debounceTimeLast, uint16_t & prevSecondaryButtonState) {
  unsigned long timeNow = millis();

  if ((timeNow - debounceTimeLast) > DEBOUNCE_PERIOD) {
    uint16_t secondaryButtonState = digitalRead(SECONDARY_BUTTON);
    debounceTimeLast = timeNow;

    if (secondaryButtonState == HIGH && prevSecondaryButtonState == LOW) {
      currentStateEdit++;
      if (currentStateEdit >= NUM_OF_STATES) {
        currentStateEdit = 0;
      }
    }
    prevSecondaryButtonState = secondaryButtonState;
  }
  return currentStateEdit;
    
}
