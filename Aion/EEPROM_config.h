/* 
  Saving to EEPROM should be done ONLY on update, not with every cycle, however to addtionally protect the user, also
  Allow saving once every EEPROM_SAVE_PERIOD to extend EEPROM lifetime
  This also means any change user makes will be saved after 0-EEPROM_SAVE_PERIOD milliseconds, so set this carefully
  5*60*1000ms = 3000000ms = 5min
*/
#define EEPROM_SAVE_PERIOD 300000

#define EEPROM_START_ADDRESS 5
#define EEPROM_PADDING_SIZE 10
/*
  Adding some padding between variables inside EEPROM does indeed waste space, however:
    - This program won't come close to filling entire EEPROM even assuming 512B (Pro Mini has 1kB), so wasting space wont be a problem
    - Chaning variable datatypes will be fairly easy, as long as the datatype is less or equal than padding size, its fine
    - No need to worry about dynamically allocating EEPROM space based on how much data is being stored. It's possible, but here
      needlessly complex + reading data from EEPROM will be very easy, just use .get(predefined address, variable)
    
*/


#define EE_STEALTHMODE_ADDRESS EEPROM_START_ADDRESS
#define EE_DUTYCYCLE_ADDRESS EE_STEALTHMODE_ADDRESS + EEPROM_PADDING_SIZE
#define EE_VOLUME_ADDRESS EE_DUTYCYCLE_ADDRESS + EEPROM_PADDING_SIZE
#define EE_CURRENTHEME_ADDRESS EE_VOLUME_ADDRESS + EEPROM_PADDING_SIZE



//States will "grow downwards" in the EEPROM, so starting at the top
#define EE_STATES_MAX_ADDRESS EEPROM.length()
