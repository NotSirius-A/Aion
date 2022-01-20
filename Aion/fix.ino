uint8_t dutyCycleLCDPercent = 100;
bool isStealthMode = false;
int32_t lastDecoderValue = 0;

void loop () {	
  int32_t encoderValue;
  int32_t encoderValueChange = 0;

  encoderValue = getEncoderValue();
  encoderValueChange = abs(encoderValue) - abs(lastEncoderValue);

  dutyCycleLCDPercent += encoderValueChange * 5;
  dutyCycleLCDPercent = constrain(dutyCycle, 0, 100);
  

  if (encoderValueChange > 0) {
    isStealthMode = true;
  } else if (encoderValueChange < 0) {
    isStealthMode = false;
  }
}
