#include "config.h"

unsigned long timeLast = 0;
const uint16_t updatePeriod = 30;

unsigned long debounceTimeLast = 0;

uint8_t currentState = 0;
int32_t lastTransitionTime = 0;

uint8_t currentMode = 0;

uint16_t prevModeButtonState = 0;

int16_t lastEncoderValue = 0;

uint32_t stateLengths[] = STATE_LENGTHS;
size_t numOfStates = 3;
void setup() {

  Serial.begin(SERIAL_BAUD_RATE);
  
  initializeEncoder(ENCODER_CLK, ENCODER_DT, ENCODER_BTN);
  lastEncoderValue = getEncoderValue();
  
}

void loop() {

  unsigned long timeNow = millis();

  int8_t encoderValueChange;
  int16_t encoderValue;  

  if ((timeNow - debounceTimeLast) > DEBOUNCE_PERIOD) {
    uint16_t modeButtonState = digitalRead(MODE_BTN);
    debounceTimeLast = timeNow;
    prevModeButtonState = modeButtonState;
    
    if (modeButtonState == LOW && prevModeButtonState == HIGH) {
      currentMode++;
      if (currentMode >= NUM_OF_MODES) {
        currentMode = 0;
      }
    }
  }
  
  
  switch(currentMode) {
    case 0:  
      int32_t timeUntilTransition = getTimeUntilTransition(timeNow, lastTransitionTime , currentState, stateLengths);
      uint8_t hours, minutes, seconds;
      convertMillisToHumanFormat(timeUntilTransition, hours, minutes, seconds);

      printTimeToLCD(hours, minutes, seconds);
      
      if (timeUntilTransition <= 0) {
        currentState++;
        lastTransitionTime = timeNow;
    
        // Loop back to state 0 when last state ends
        if (currentState >= numOfStates) {
          currentState = 0; 
        }
    
        // Execute some code when transition to another state happens, for example let user know a transition happened with sound etc.
        for (bool i = false; i==false;) {
          i = handleTransition();
        }
      }
      break; 
    case 1:
      //handleStateLengthEditMode(encoderValueChange);
      break;
    case 2:
      //handleDutyCycleEditMode(encoderValueChange);
      break;
    case 3:
      //handleStealthinessEditMode(encoderValueChange);
      break;
    default:
      Serial.println("def");
  }

}

bool handleTransition() {
  return true;
}
