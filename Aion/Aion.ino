#include "config.h"

unsigned long timeNow_ms = 0;
unsigned long timeLast_ms = 0;
uint16_t updatePeriod_ms = 30;

uint8_t currentState = 0;
uint32_t stateLengths[NUM_OF_STATES];
int32_t lastTransitionTime_ms = 0;

uint8_t currentMode = 0;

uint16_t prevBtn = 1;

void setup() {

  //TODO find a way to improve this
  stateLengths[0] = FIRST_STATE_LENGTH;
  stateLengths[1] = SECOND_STATE_LENGTH;
  stateLengths[2] = THIRD_STATE_LENGTH;
  Serial.begin(SERIAL_BAUD_RATE);
   
  initializeEncoder(ENCODER_CLK, ENCODER_DT, ENCODER_BTN);
 
}

void loop() {
  timeNow_ms = millis();
  delay(10);
  uint16_t buttonState = digitalRead(ENCODER_BTN);
  if (buttonState == LOW && prevBtn == HIGH) {
    currentMode++;
    if (currentMode >= NUM_OF_MODES){
      currentMode = 0;
    }
   }
  prevBtn = buttonState;
  
  

  if ((timeNow_ms - timeLast_ms) > updatePeriod_ms){
    timeLast_ms = timeNow_ms;
//    Serial.println(getEncoderValue());

    Serial.println(currentMode);

    int x = getTimeUntilTransition(timeNow_ms, lastTransitionTime_ms, currentState, stateLengths);
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
      lastTransitionTime_ms = timeNow_ms;
      currentState++;
      if (currentState >= NUM_OF_STATES) {
        currentState = 0; 
      }
    }
    
  }

}
