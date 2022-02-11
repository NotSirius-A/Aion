#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <EEPROM.h>

#include "config.h"
#include "states_config.h"
#include "GUI_config.h"
#include "EEPROM_config.h"

Adafruit_ST7735 tft = Adafruit_ST7735(LCD_TFT_CS, LCD_TFT_DC, LCD_TFT_RST);

word PRIMARY_THEME_COLOR = PRIMARY_THEME_COLOR_DEFAULT;
word SECONDARY_THEME_COLOR = SECONDARY_THEME_COLOR_DEFAULT;
word BACKGROUND_COLOR = BACKGROUND_COLOR_DEFAULT;

uint8_t currentState = 0;
unsigned long lastTransitionTime = 0;
uint8_t currentStateEdit = 0;
uint16_t prevSecondaryButtonState = 1;

unsigned long debounceTimeLast = 0;
unsigned long secondaryDebounceTimeLast = 0; 

unsigned long EEPROMSaveTimeLast = 0;

uint8_t currentMode = START_MODE_DEFAULT;
uint16_t prevModeButtonState = 1;

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
  
  pinMode(PRIMARY_BUTTON, INPUT_PULLUP);
  pinMode(SECONDARY_BUTTON, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);
  digitalWrite(SPEAKER_PIN, LOW);
  pinMode(BLUE_LED_PIN, OUTPUT);
  digitalWrite(BLUE_LED_PIN, LOW);
  pinMode(LCD_BACKLIGHT, OUTPUT);
  digitalWrite(LCD_BACKLIGHT, HIGH);


  //fillEEPROMwithDefaults();
  loadSettingsFromEEPROM();
  
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);



  
  
  initializeEncoder(ENCODER_CLK, ENCODER_DT, ENCODER_BUTTON);  

  
  tft.initR(INITR_GREENTAB);
  tft.setTextWrap(false);
  tft.setRotation(1);
  tft.fillScreen(BACKGROUND_COLOR);
  tft.setTextColor(PRIMARY_THEME_COLOR, ST77XX_BLACK);


    
  /* 
   Display a splash screen and delay for some time
  */
  printSplashScreen();
  delay(SPLASH_SCREEN_DURATION);
  
  tft.fillScreen(BACKGROUND_COLOR);

  unsigned int timeNow = millis();
  debounceTimeLast = timeNow;
  secondaryDebounceTimeLast = timeNow;
  lastTransitionTime = timeNow;
}

void loop() {
  unsigned long timeNow = millis();

  int16_t encoderValue = getEncoderValue();
  int8_t encoderValueChange = encoderValue - lastEncoderValue;
  encoderValueChange = constrain(encoderValueChange, -3, 3);
  lastEncoderValue = encoderValue;

  uint8_t lastMode = currentMode;
  currentMode = getUpdatedCurrentMode(currentMode, debounceTimeLast, prevModeButtonState);
  if (currentMode != lastMode) {
    //Clear the screen when mode changes, to reset GUI
    tft.fillScreen(BACKGROUND_COLOR);

    //Reset the timer when mode changes happens
    lastTransitionTime = timeNow;
  }

  
  if (isStealthMode && currentMode == 0) {
    digitalWrite(LCD_BACKLIGHT, LOW);
  } else {
    digitalWrite(LCD_BACKLIGHT, HIGH);
  }

  switch(currentMode) {
    /*
      switch statement seems to break when in a case there's an inline declaration + initialization, like int32_t t = 0;
      but only when its directly inside a case, nested are fine 
    */
    case 0: 
      int32_t timeUntilTransition;
      timeUntilTransition = getTimeUntilTransition(timeNow, lastTransitionTime , currentState, states);

      HumanTime timeStruct;
      timeStruct = convertMillisToHumanFormat(timeUntilTransition + 600); // +600=0.6s offset, so it doesnt display 0:00:00 for long

      uint8_t coords[2];
      coords[0] = tft.width()/2;
      coords[1] = tft.height()/3;
      adjustTextCoords(coords, timeStruct.stringFormat, 1, 3);
      
      tft.setCursor(coords[0], coords[1]);
      tft.setTextSize(3);
      tft.println(timeStruct.stringFormat);

      coords[0] = tft.width()/2;
      coords[1] = tft.height()-25;
      adjustTextCoords(coords, states[currentState].verboseName, 1, 2);
      tft.setCursor(coords[0], coords[1]);
      tft.setTextSize(2);
      tft.println(states[currentState].verboseName);
      
      if (timeUntilTransition <= 0) {
        uint32_t pausePeriod = states[currentState].pausePeriod;
        currentState++;
        // Loop back to state 0 when last state ends
        if (currentState >= NUM_OF_STATES) {
          currentState = 0; 
        }
        
        // Execute some code when transition to another state happens, for example let user know a transition happened with sound etc.
        tft.fillScreen(ST77XX_BLACK);
        for (unsigned long startTime = timeNow; (timeNow - startTime) < pausePeriod;) {
          handleTransition(timeNow - startTime, pausePeriod, currentState, volume, isStealthMode);
          timeNow = millis();
        }
        analogWrite(SPEAKER_PIN, LOW);
        digitalWrite(BLUE_LED_PIN, LOW);
        tft.fillScreen(BACKGROUND_COLOR);
        
        lastTransitionTime = timeNow;
      }
      break; 
      
    case 1:
      uint8_t lastStateEdit; 
      lastStateEdit = currentStateEdit; 
      currentStateEdit = getUpdatedCurrentStateEdit(currentStateEdit, secondaryDebounceTimeLast, prevSecondaryButtonState);
      if (currentStateEdit != lastStateEdit) {
        tft.fillScreen(BACKGROUND_COLOR);
      }
      lastStateEdit = currentStateEdit; 
      
      states[currentStateEdit] = handleStateLengthEditMode(states[currentStateEdit], encoderValueChange);
      break;
      
    case 2:
      isStealthMode = handleStealthEditMode(isStealthMode, encoderValueChange);
      break;
      
    case 3:
      dutyCycleLCDPercent = handleDutyCycleEditMode(dutyCycleLCDPercent, encoderValueChange);
      break;
      
    case 4:
      volume = handleVolumeControlMode(volume, encoderValueChange);
      break;
      
    default:
      Serial.println("BAD MODE VALUE");
  }

 
  if ((digitalRead(SECONDARY_BUTTON) == LOW) && (currentMode == 4)) {
    analogWrite(SPEAKER_PIN, volume);
  } else {
    analogWrite(SPEAKER_PIN, LOW);
  }

  if ((timeNow - EEPROMSaveTimeLast) > EEPROM_SAVE_PERIOD) {
    bool oldStealthMode;
    EEPROM.get(EE_STEALTHMODE_ADDRESS, oldStealthMode);
    if (oldStealthMode != isStealthMode) {
      EEPROM.put(EE_STEALTHMODE_ADDRESS, isStealthMode);
    }
  
    uint8_t oldDutyCycle;
    EEPROM.get(EE_DUTYCYCLE_ADDRESS, oldDutyCycle);
    if (oldDutyCycle != dutyCycleLCDPercent) {
      EEPROM.put(EE_DUTYCYCLE_ADDRESS, dutyCycleLCDPercent);
    }
  
    uint8_t oldVolume;
    EEPROM.get(EE_VOLUME_ADDRESS, oldVolume);
    if (oldVolume != volume) {
      EEPROM.put(EE_VOLUME_ADDRESS, volume);
    }
  
    State oldState;
    EEPROM.get(EE_STATES_MAX_ADDRESS - ((currentStateEdit+1)*SIZE_OF_STATE), oldState);;
    if (oldState.period != states[currentStateEdit].period) {
      EEPROM.put(EE_STATES_MAX_ADDRESS - ((currentStateEdit+1)*SIZE_OF_STATE), states[currentStateEdit]);
    }
  }
}

void handleTransition(unsigned long timeSinceStarted, uint32_t pausePeriod, uint8_t currentState, uint8_t volume, bool isStealthMode) {
  if (isStealthMode) {
    volume = 0;
  }
  
  uint8_t coords[2] = {tft.width()/2, tft.height()/2};
  adjustTextCoords(coords, states[currentState].verboseName, 1, 2);
  tft.setCursor(coords[0], coords[1]);
  tft.setTextSize(2);
  tft.println(states[currentState].verboseName);

  unsigned long pulseLength = pausePeriod/(2*(currentState+1));
  if (timeSinceStarted < pausePeriod/2) {
    digitalWrite(BLUE_LED_PIN, HIGH);
    analogWrite(SPEAKER_PIN, volume);
  } else if (((timeSinceStarted-pausePeriod/2) % pulseLength) > pulseLength/2){
    digitalWrite(BLUE_LED_PIN, HIGH);
    analogWrite(SPEAKER_PIN, volume);
  } else {
    digitalWrite(BLUE_LED_PIN, LOW);
    analogWrite(SPEAKER_PIN, LOW);
  }

}
