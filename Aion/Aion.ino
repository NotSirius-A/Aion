#include "config.h"
#include "states_config.h"




//#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//
//#if defined(ARDUINO_FEATHER_ESP32) // Feather Huzzah32
//  #define TFT_CS         14
//  #define TFT_RST        15
//  #define TFT_DC         32
//
//#elif defined(ESP8266)
//  #define TFT_CS         4
//  #define TFT_RST        16                                            
//  #define TFT_DC         5
//
//#else
//  // For the breakout board, you can use any 2 or 3 pins.
//  // These pins will also work for the 1.8" TFT shield.
//  #define TFT_CS        10
//  #define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
//  #define TFT_DC         8
//#endif
//
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);










unsigned long debounceTimeLast = 0;

uint8_t currentState = 0;
int32_t lastTransitionTime = 0;

uint8_t currentMode = START_MODE_DEFAULT;
uint16_t prevModeButtonState = 0;

int16_t lastEncoderValue = 0;

uint8_t dutyCycleLCDPercent = DUTY_CYCLE_LCD_PERCENT_DEFAULT;
bool isStealthMode = STEALTHMODE_DEFAULT;

uint8_t volume = VOLUME_DEFAULT;

typedef struct HumanTime {
  uint32_t milliseconds;
  uint8_t seconds, minutes, hours;
  char stringFormat[20];
};

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  pinMode(MODE_BTN, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  
  initializeEncoder(ENCODER_CLK, ENCODER_DT, ENCODER_BTN);  

  
//  tft.initR(INITR_GREENTAB);
//  tft.setTextWrap(false);
//  tft.setRotation(1);
//  tft.fillScreen(ST77XX_BLACK);
//  // font1 = [6, 7]
//  // font2 = [12,14]
//  // font3 = [18,21]
//  
//  tft.setCursor(tft.width()/2-(3.5*18), tft.height()/2-(21/2+21/5));
//  tft.setTextColor(ST77XX_RED);
//  tft.setTextSize(3);
//  tft.println("NewText");
//  tft.drawPixel(tft.width()/2, tft.height()/2, ST77XX_GREEN);



}

void loop() {
  unsigned long timeNow = millis();

  int16_t encoderValue = getEncoderValue();
  int8_t encoderValueChange = encoderValue - lastEncoderValue;
  lastEncoderValue = encoderValue;
  Serial.println(encoderValueChange);

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

  currentMode = 1;
  switch(currentMode) {
    case 0: 
      int32_t timeUntilTransition;
      timeUntilTransition = getTimeUntilTransition(timeNow, lastTransitionTime , currentState, states);

      HumanTime timeStruct;
      timeStruct = convertMillisToHumanFormat(timeUntilTransition);

      if (timeUntilTransition <= 0) {
        currentState++;
        lastTransitionTime = timeNow;
    
        // Loop back to state 0 when last state ends
        if (currentState >= numOfStates) {
          currentState = 0; 
        }
    
        // Execute some code when transition to another state happens, for example let user know a transition happened with sound etc.
        for (bool i = false; i==false;) {
          Serial.println("-------------------------------------");
          i = handleTransition();
        }
      }
      break; 
    case 1:
      // No return value, function changes `states` directly
      handleStateLengthEditMode(states, numOfStates, encoderValueChange);
      Serial.println(states[0].period);
      break;
    case 2:
      dutyCycleLCDPercent = handleDutyCycleEditMode(dutyCycleLCDPercent, encoderValueChange);
      break;
    case 3:
      isStealthMode = handleStealthEditMode(isStealthMode, encoderValueChange);
      break;
    case 4:
      volume = handleVolumeControlMode(volume, encoderValueChange);
      //analogWrite(SPEAKER_PIN, volume);
      break;
    default:
      Serial.println("BAD MODE VALUE");
  }

}

bool handleTransition() {
  return true;
}
