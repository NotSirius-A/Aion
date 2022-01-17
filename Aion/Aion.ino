#include "config.h"

unsigned long timeNow  = 0;
unsigned long timeLast = 0;
const uint16_t updatePeriod = 30;

unsigned long debounceTimeLast = 0;

uint8_t currentState = 0;
int32_t lastTransitionTime = 0;

uint8_t currentMode = 0;

uint16_t modeButtonState = 0;
uint16_t prevModeButtonState = 0;

int16_t lastEncoderValue = 0;

void setup() {

  Serial.begin(SERIAL_BAUD_RATE);
  initializeEncoder(ENCODER_CLK, ENCODER_DT, ENCODER_BTN);
  lastEncoderValue = getEncoderValue();
  
  timeNow = millis();
  timeLast = timeNow;
}

void loop() {

  timeNow = millis();
  timeLast = timeNow;

  if ((timeNow - debounceTimeLast) > DEBOUNCE_PERIOD) {
    debounceTimeLast = timeNow;
    modeButtonState = digitalRead(MODE_BTN);
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
      handleMainMode(timeNow, lastTransitionTime , currentState);
      break; 
    case 1:
      Serial.println("dfssdfd");
      break;
    default:
      Serial.println("def");
  }
  

}
