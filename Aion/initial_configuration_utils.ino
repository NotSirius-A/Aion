void fillEEPROMwithDefaults() {
  Serial.println("***** LOADING INTO EEPROM *****");

  for (int i = 0; i < NUM_OF_STATES; i++) {
    EEPROM.put(EE_STATES_MAX_ADDRESS - ((i+1)*SIZE_OF_STATE), states[i]);
  }

  for (int i = 0; i < NUM_OF_STATES; i++) {
    State x;
    EEPROM.get(EE_STATES_MAX_ADDRESS - ((i+1)*SIZE_OF_STATE), x);
    Serial.print(x.verboseName);
    Serial.print(" -> ");
    Serial.print(x.period);
    Serial.print(", ");
    Serial.print(x.pausePeriod);
    Serial.print(", ");
    Serial.println(x.type);
  }
  
  EEPROM.put(EE_STEALTHMODE_ADDRESS, isStealthMode);
  EEPROM.put(EE_DUTYCYCLE_ADDRESS, dutyCycleLCDPercent);
  EEPROM.put(EE_VOLUME_ADDRESS, volume);
  EEPROM.put(EE_CURRENTHEME_ADDRESS, currentTheme);

  isStealthMode = 0;
  dutyCycleLCDPercent = 0;
  volume = 0;
  
  EEPROM.get(EE_STEALTHMODE_ADDRESS, isStealthMode);
  EEPROM.get(EE_DUTYCYCLE_ADDRESS, dutyCycleLCDPercent);
  EEPROM.get(EE_VOLUME_ADDRESS, volume);
  EEPROM.get(EE_CURRENTHEME_ADDRESS, currentTheme);

  Serial.print("Stealth Mode = ");
  Serial.println(isStealthMode);
  Serial.print("Duty Cycle = ");
  Serial.println(dutyCycleLCDPercent);
  Serial.print("Volume = ");
  Serial.println(volume);
  Serial.print("Theme = ");
  Serial.println(currentTheme);


  Serial.println("*****************************");
}
