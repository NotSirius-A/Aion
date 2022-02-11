word convertRGB( byte R, byte G, byte B)
{
  /* 
    Credit: HazardsMind
    https://forum.arduino.cc/t/adafruit-library-tft-colours/239777/7
    return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );

    Display used in this project has red and blue flipped, so its BGR,
    therefore I modifed this function a bit to correct for that.
    
  */
  return ( ((B & 0xF8) << 8) | ((G & 0xFC) << 3) | (R >> 3) );
}

void printSplashScreen() {
  uint8_t coords[2] = {tft.width()/2, tft.height()/2-15};
  char text1[] = SPLASH_SCREEN_STR1;
  adjustTextCoords(coords, text1, 1, 5);
  tft.setCursor(coords[0], coords[1]);
  tft.setTextSize(5);
  tft.println(text1);
  
  coords[0] = tft.width()/2;
  coords[1] = tft.height()-20;
  char text2[] = SPLASH_SCREEN_STR2;
  adjustTextCoords(coords, text2, 1, 1);
  tft.setCursor(coords[0], coords[1]);
  tft.setTextSize(1);
  tft.println(text2);


}


void printProgressBar(double percentageFilled, double xStart, double yStart, double x, double y) {
  percentageFilled = constrain(percentageFilled, 0, 1);
   
  double a = y/4;
  double L = x-(4.4*a)+2;

  tft.fillTriangle(xStart, yStart-y/2, xStart+a, yStart, xStart+a, yStart-y, SECONDARY_THEME_COLOR);

  tft.fillTriangle(xStart+x, yStart-y/2, xStart+x-a, yStart, xStart+x-a, yStart-y, SECONDARY_THEME_COLOR);

  tft.drawRect(xStart+(1.6*a), yStart-y, x-(3.2*a)+2, y, PRIMARY_THEME_COLOR);

  tft.fillRect(xStart+(2.2*a)+percentageFilled*L, yStart-(y*0.8), L*(1-percentageFilled)+(a*0.3), (0.6*y)+2, BACKGROUND_COLOR);
  tft.fillRect(xStart+(2.2*a), yStart-(y*0.8), percentageFilled*L, (0.6*y)+2, PRIMARY_THEME_COLOR);
}


int32_t getTimeUntilTransition(unsigned long now, unsigned long lastTransitionTime, uint8_t currentState, State states[]) {
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

void adjustTextCoords(uint8_t coords[2], char str[], uint8_t id, uint8_t fontSize) {
  /*
    In order to print text to LCD with <Adafruit_ST7735.h> programmer must provide cursor coordinates,
    which are used as a starting points (upper-left corner of the text). So if one wants to print
    for example centered text in the middle of a screen coordinates must be calculated based on the length of the string.
    This function adjusts given coords to correctly justify text. Ex. Printing centered text in the middle of the screen looks like this,

    uint8_t coords[2] = {tft.width()/2, tft.height()/2};
    adjustTextCoords(coords, "str", 1, 3);
      
    tft.setCursor(coords[0], coords[1]);
    tft.setTextSize(3);
    tft.println("str");
   
    ID:
    0 - justify: left
    1 - justify: center
    2 - justify: right
    
  */

  uint8_t strLength = strlen(str);
  float halfStrLength = strLength / 2.0;
  
  switch(id) {
    case 0:
      coords[0] = coords[0];
      coords[1] = coords[1]-(FONT_SIZE_1_Y*fontSize/1.8);
      break;  
    case 1:
      coords[0] = coords[0]-(halfStrLength * (FONT_SIZE_1_X*fontSize));
      coords[1] = coords[1]-(FONT_SIZE_1_Y*fontSize/1.8);
      break;
    case 2:
      coords[0] = coords[0]-(strLength * (FONT_SIZE_1_X*fontSize));
      coords[1] = coords[1]-(FONT_SIZE_1_Y*fontSize/1.8);
      break; 
  }

}

void loadSettingsFromEEPROM() {
/*
  Loads settings from EEPROM directly into global variables
*/

  EEPROM.get(EE_STEALTHMODE_ADDRESS, isStealthMode);
  EEPROM.get(EE_DUTYCYCLE_ADDRESS, dutyCycleLCDPercent);
  EEPROM.get(EE_VOLUME_ADDRESS, volume);

  for (int i = 0; i < NUM_OF_STATES; i++) {
    EEPROM.get(EE_STATES_MAX_ADDRESS - ((i+1)*SIZE_OF_STATE), states[i]);
  }
  
}
