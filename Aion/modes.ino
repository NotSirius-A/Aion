State handleStateLengthEditMode(State state, int8_t encoderValueChange) {
  uint32_t period = state.period;

  period += encoderValueChange * STATE_CHANGE_MULTIPLIER;
 
  state.period = constrain(period, MIN_STATE_PERIOD, MAX_STATE_PERIOD);

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

  printProgressBar(x);
  
  return volume;
}

uint8_t handleDutyCycleEditMode (uint8_t dutyCycleLCDPercent, int8_t encoderValueChange) {
  int8_t rv = dutyCycleLCDPercent + encoderValueChange * DUTY_CYCLE_CHANGE_MULTIPLIER;
  dutyCycleLCDPercent = constrain(rv, 0, 100);
  return dutyCycleLCDPercent;
}

bool handleStealthEditMode (bool isStealthMode, int8_t encoderValueChange) {
  if (encoderValueChange > 0) {
    isStealthMode = true;
  } else if (encoderValueChange < 0) {
    isStealthMode = false;
  } 
  return isStealthMode;
}
