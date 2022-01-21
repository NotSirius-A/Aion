#include "config.h"

unsigned long debounceTimeLast = 0;

uint8_t currentState = 0;
int32_t lastTransitionTime = 0;

uint8_t currentMode = 0;

uint16_t prevModeButtonState = 0;

int16_t lastEncoderValue = 0;

uint8_t dutyCycleLCDPercent = DUTY_CYCLE_LCD_PERCENT_DEFAULT;
bool isStealthMode = STEALTHMODE_DEFAULT;

uint32_t stateLengths[] = STATE_LENGTHS;
size_t numOfStates = sizeof(stateLengths)/sizeof(*stateLengths);

uint8_t volume = VOLUME_DEFAULT;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  pinMode(MODE_BTN, INPUT_PULLUP);
  pinMode(5, OUTPUT);


  initializeEncoder(ENCODER_CLK, ENCODER_DT, ENCODER_BTN);  
}

void loop() {
  unsigned long timeNow = millis();

  int16_t encoderValue = getEncoderValue();
  int8_t encoderValueChange = encoderValue - lastEncoderValue;
  lastEncoderValue = encoderValue;
  delay(10);


  if ((timeNow - debounceTimeLast) > DEBOUNCE_PERIOD) {
    uint16_t modeButtonState = digitalRead(MODE_BTN);
    debounceTimeLast = timeNow;

    if (modeButtonState == LOW && prevModeButtonState == HIGH) {
      currentMode++;
      if (currentMode >= NUM_OF_MODES) {
        currentMode = 0;
      }
    }
    prevModeButtonState = modeButtonState;
  }

  switch(currentMode) {
    case 0:
      int32_t timeUntilTransition;   
      timeUntilTransition = getTimeUntilTransition(timeNow, lastTransitionTime , currentState, stateLengths);
      uint8_t hours, minutes, seconds;
      convertMillisToHumanFormat(timeUntilTransition, hours, minutes, seconds);

      //printTimeToLCD(hours, minutes, seconds);
      
      if (timeUntilTransition <= 0) {
        currentState++;
        lastTransitionTime = timeNow;
    
        // Loop back to state 0 when last state ends
        if (currentState >= numOfStates) {
          currentState = 0; 
        }
    
        // Execute some code when transition to another state happens, for example let user know a transition happened with sound etc.
        for (bool i = false; i==false;) {
          Serial.println("---------");
          i = handleTransition();
        }
      }
      break; 
    case 1:
      //handleStateLengthEditMode(encoderValueChange);
      break;
    case 2:
      //handleDutyCycleEditMode(encoderValueChange);
//        dutyCycleLCDPercent += encoderValueChange * 5;
//  dutyCycleLCDPercent = constrain(dutyCycle, 0, 100);
      break;
    case 3:
      //handleStealthinessEditMode(encoderValueChange);
//              if (encoderValueChange > 0) {
//          isStealthMode = true;
//        } else if (encoderValueChange < 0) {
//          isStealthMode = false;
//        }
      break;
    case 4:
      handleVolumeControlMode(volume, isStealthMode, encoderValueChange);
      analogWrite(5, volume);
      break;
    default:
      Serial.println("def");
  }

}

bool handleTransition() {
  return true;
}
