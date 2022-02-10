State handleStateLengthEditMode(State state, int8_t change) {

  int32_t period = state.period;
 
  period += change * STATE_CHANGE_MULTIPLIER;

  //For unknown reasons period sometimes stops being a multiple of STATE_CHANGE_MULTIPLIER, modulo is a quick fix
  period -= period % STATE_CHANGE_MULTIPLIER;


  if (period < MIN_STATE_PERIOD) {
    state.period = MAX_STATE_PERIOD;
  } else if (period > MAX_STATE_PERIOD) {
    state.period = MIN_STATE_PERIOD;
  } else {
    state.period = period;
  }


  uint8_t coords[2];

  coords[0] = tft.width()/2;
  coords[1] = 20;
  
  adjustTextCoords(coords, state.verboseName, 1, 2);
  tft.setCursor(coords[0], coords[1]);
  tft.setTextSize(2);
  tft.println(state.verboseName);

  HumanTime timeStruct;
  timeStruct = convertMillisToHumanFormat(state.period);
  
  coords[0] = tft.width()/2;
  coords[1] = tft.height()/2;
 
  adjustTextCoords(coords, timeStruct.stringFormat, 1, 3);
  tft.setCursor(coords[0], coords[1]);
  tft.setTextSize(3);
  tft.println(timeStruct.stringFormat);

  return state; 
}

uint8_t handleVolumeControlMode(uint8_t volume, int8_t change) {
  /* This function takes in volume and change in argument and returns new volume
   Firstly, given volume is used to get the original function argument, then
   change in argument is added to the calculated value, so that the new value can be
   plugged into the volume function to get the updated volume
  */ 

  /* There is only natural log natively in Arduino code, here other log base is required, fortunately
    there's a formula that can be used here: logx(z) = logy(z) / logy(x)
  */

  double a = VOLUME_CONTROL_EXPONENT_COEFFICIENT;
  double x = (log(volume+1)/log(2))/a;

  double exponent = double(x+change)*a;
  double result = pow(2.0, exponent)-1;

  /* 
    PWM can only support int 0-255, so `result` has to be rounded up
  */ 

  if (change >= 0) {
    result = round(result+0.47);
  } else if (change < 0) {
    result = round(result-0.47);
  } 
  
  volume = constrain(trunc(result), 0, 255);

  uint8_t coords[2];

  coords[0] = tft.width()/2;
  coords[1] = 20;
  char title[] = "Volume";
  
  adjustTextCoords(coords, title, 1, 2);
  tft.setCursor(coords[0], coords[1]);
  tft.setTextSize(2);
  tft.println(title);
  
  coords[0] = tft.width()/2;
  coords[1] = 60;
  char volumeText[6];
  sprintf(volumeText, " %hu ", volume);
  
  adjustTextCoords(coords, volumeText, 1, 2);
  tft.setCursor(coords[0], coords[1]);
  tft.setTextSize(2);
  tft.println(volumeText);

  double yStart = PROGRESS_BAR_YSTART_DEFAULT;
  double xStart = PROGRESS_BAR_XSTART_DEFAULT + PROGRESS_BAR_PADDINGX_DEFAULT;
  double width = tft.width()-2*PROGRESS_BAR_PADDINGX_DEFAULT;
  double height = PROGRESS_BAR_HEIGHT_DEFAULT; 

  double percentage = x*VOLUME_CONTROL_EXPONENT_COEFFICIENT/8;
  printProgressBar(percentage, xStart, yStart, width, height);
  
  return volume;
}

uint8_t handleDutyCycleEditMode (uint8_t dutyCycleLCDPercent, int8_t change) {
  int8_t rv = dutyCycleLCDPercent + change * DUTY_CYCLE_CHANGE_MULTIPLIER;
  dutyCycleLCDPercent = constrain(rv, 0, 100);

  uint8_t coords[2];

  coords[0] = tft.width()/2;
  coords[1] = 20;
  char title[] = "Duty Cycle";
  
  adjustTextCoords(coords, title, 1, 2);
  tft.setCursor(coords[0], coords[1]);
  tft.setTextSize(2);
  tft.println(title);
  
  coords[0] = tft.width()/2;
  coords[1] = 60;
  char dutyCycleText[6];
  sprintf(dutyCycleText, " %hu ", dutyCycleLCDPercent);
  
  adjustTextCoords(coords, dutyCycleText, 1, 2);
  tft.setCursor(coords[0], coords[1]);
  tft.setTextSize(2);
  tft.println(dutyCycleText);

  double yStart = PROGRESS_BAR_YSTART_DEFAULT;
  double xStart = PROGRESS_BAR_XSTART_DEFAULT + PROGRESS_BAR_PADDINGX_DEFAULT;
  double width = tft.width()-2*PROGRESS_BAR_PADDINGX_DEFAULT;
  double height = PROGRESS_BAR_HEIGHT_DEFAULT; 

  double percentage = dutyCycleLCDPercent/100.0;
  printProgressBar(percentage, xStart, yStart, width, height);

  
  return dutyCycleLCDPercent;
}

bool handleStealthEditMode (bool isStealthMode, int8_t change) {
  if (change > 0) {
    isStealthMode = true;
  } else if (change < 0) {
    isStealthMode = false;
  } 

  uint8_t coords[2];

  coords[0] = tft.width()/2;
  coords[1] = 20;
  char title[] = "Stealth Mode";
  
  adjustTextCoords(coords, title, 1, 2);
  tft.setCursor(coords[0], coords[1]);
  tft.setTextSize(2);
  tft.println(title);
  
  coords[0] = tft.width()/2;
  coords[1] = 60;
  char isStealthModeText[11];
  sprintf(isStealthModeText, " %s ", isStealthMode ? "Enabled" : "Disabled");
  
  adjustTextCoords(coords, isStealthModeText, 1, 2);
  tft.setCursor(coords[0], coords[1]);
  tft.setTextSize(2);
  tft.println(isStealthModeText);

  double yStart = PROGRESS_BAR_YSTART_DEFAULT;
  double xStart = PROGRESS_BAR_XSTART_DEFAULT + PROGRESS_BAR_PADDINGX_DEFAULT;
  double width = tft.width()-2*PROGRESS_BAR_PADDINGX_DEFAULT;
  double height = PROGRESS_BAR_HEIGHT_DEFAULT; 

  double percentage = isStealthMode;
  printProgressBar(percentage, xStart, yStart, width, height);

  
  return isStealthMode;
}
