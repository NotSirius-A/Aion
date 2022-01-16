#include "config.h"

unsigned long timeNow  = 0;
unsigned long timeLast = 0;
uint16_t updatePeriod = 30;

unsigned long debounceTimeLast = 0;
uint8_t debouncePeriod = 50;

uint8_t currentState = 0;
uint32_t stateLengths[NUM_OF_STATES];
int32_t lastTransitionTime = 0;

uint8_t currentMode = 0;

uint16_t modeButtonState = 0;
uint16_t prevModeButtonState = 0;

void setup() {

  //TODO find a way to improve this
  stateLengths[0] = FIRST_STATE_LENGTH;
  stateLengths[1] = SECOND_STATE_LENGTH;
  stateLengths[2] = THIRD_STATE_LENGTH;
  Serial.begin(SERIAL_BAUD_RATE);
   
  initializeEncoder(ENCODER_CLK, ENCODER_DT, ENCODER_BTN);
 
}

void loop() {
  timeNow = millis();

  if ((timeNow - debounceTimeLast) > debouncePeriod) {
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
  

  if ((timeNow - timeLast) > updatePeriod){
    timeLast = timeNow;
//    Serial.println(getEncoderValue());

//    Serial.println(currentMode);

    int x = getTimeUntilTransition(timeNow, lastTransitionTime , currentState, stateLengths);
    x = x / 1000;
    uint8_t hours = x / 3600;
    uint8_t minutes = (x / 60) - (hours * 60);
    uint8_t seconds = x - (minutes * 60) - (hours * 3600);

//    Serial.print(hours);
//    Serial.print(" ");
//    Serial.print(minutes);
//    Serial.print(" ");
//    Serial.println(seconds);
  
    if (x < 0) {
      Serial.println("======================");
      lastTransitionTime = timeNow ;
      currentState++;
      if (currentState >= NUM_OF_STATES) {
        currentState = 0; 
      }
    }
    
  }

}
