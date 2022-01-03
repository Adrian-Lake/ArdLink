/*
 * Firmware configuration paramters
 */

// COMMON SPECIFICATIONS
// EEPROM 
//Arduino UNO         ATmega328P          1 KB
//Arduino Nano        ATmega328P          1 KB
//Arduino Mega 2560   ATmega 2560         4 KB
//Arduino Micro       ATmega32U4          1 KB
//Arduino Leonardo    ATmega32U4          1 KB

// RAM 
//Arduino UNO       ATmega328P            2 KB
//Arduino Nano      ATmega328P            2 KB
//Arduino Mega 2560 ATmega 2560           8 KB
//Arduino Micro     ATmega32U4            2.5 KB
//Arduino Leonardo  ATmega32U4            2.5 KB



/*
 * All of these below quantities must sum to less 
 * than both the EEPROM and RAM specifications 
 * above. be aware some of the RAM will be taken 
 * up by other components.
 */

// System EEPROM memory size 
// Each element is 2 bytes
#define EEPROM_SYS_RESERVE 12

// Instruction buffer size
// Each instruction is 6 bytes
#define MAX_INSTR_BUFFER_LEN 100

// Program memory size
// Each element is 2 bytes
#define MAX_DATA_BUFFER_LEN 400


// EEPROM system reserve map
// specified in increments of words 
#define EEPROM_BOOT_ACTION 0
#define EEPROM_ID 1

// Bit offsets for boot action 
#define LOAD_EEPROM_INSTR 0x0001
#define EXEC_INSTR_BUFFER 0x0002


// offsets for mampring memmory
// this could use optimization!
#define SYSTEM_CONFIG_OFFSET 0x0000
#define INSTR_BUFFER_OFFSET 0x4000
#define DATA_BUFFER_OFFSET 0x8000
// Direct EEPROM access is not implmented yet
#define EEPROM_OFFSET 0xC000
