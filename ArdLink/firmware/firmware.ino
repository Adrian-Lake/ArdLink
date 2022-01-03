#include "config.h"
#include "interpreter.h"


void setup() {

  // Format EEPROM before anything else
  // Use if the microcontroller EEPROM was used for another project
  // format_EEPROM();

  // Serial comms to host
  Serial.begin(9600);

  // Set seed for RNG instruction
  randomSeed(analogRead(0)); // On real devices this will create a random "enough" seed

  // defines boot routine for microcontroller
  word boot_action = read_boot_action();

  // load eeprom data at boot
  bool load_eeprom_instr = (LOAD_EEPROM_INSTR & boot_action) > 0;

  // execute instr buffer at boot, only useful if EEPROM is read and has a program in it!
  bool exec_instr_buffer = (EXEC_INSTR_BUFFER & boot_action) > 0;

  // Execute boot actions
  if (load_eeprom_instr) {
    // read existing program
    read_EEPROM_instr_buffer();

    // trigger instr buffer execution
    if (exec_instr_buffer) {
      exec_pgrm = true;
    }
  }

}


void loop() {
  
  // execute instr buffer
  if (exec_pgrm) {
    exec_pgrm = false; // prevent loop
    exec_instr_buffer();
  }
}

void serialEvent() { 
  // Event for reading in data from Serial stream, NOT INTERUPT driven
  // This does not work for SAMD (ARD zero), and other common boards.

  // Consider putting this routine in the main loop
  read_instr();
  exec(&parsed_instr);

}
