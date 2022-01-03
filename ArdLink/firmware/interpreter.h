/*
 * Execution and instruction interpretation functions
 */

#include "data.h"
#include "digital_IO.h"
#include "interface.h"

void goto_label(word label) {
  // goto label defined in p1 of instruction in instr_buffer
  for (word i = 0; i < MAX_INSTR_BUFFER_LEN; i++) {

    if (instr_buffer[i].op == 0x0000 & !(instr_buffer[i].p1 ^ label)) {
      instr_buffer_index = i;
      break;
    }
  }
}

void exec(struct instruction *instr) {
  // execute instruction at pointer

  switch (instr->op) {

    // Note the formatting on each op code's comment. This source is used to configure the Python module!
    //section:BEGIN_OP_DEFINITION

    /*
     * 0x00 - 0x14 Basic Operations
    */

    case 0x00:
      // sym:NOP params:0 doc_str: Non operation, do nothing
      // sym:Label params:1 doc_str: set label (p1) in instruction buffer, see Goto
      break;

    case 0x01:
      // sym:Halt params:0 doc_str: end of application, return exec pointer to init instr
      end_pgrm = true;

    case 0x02:
      // sym:ReadInstr params:0 doc_str: Read Instruction if available.
      read_instr();
      break;

    case 0x03:
      // sym:Set params:2 doc_str: Set Constant (p2) to addr (p1)
      *(word *)(ref_map(instr->p1)) = (word)instr->p2; // no need to cast
      break;

    case 0x04:
      // sym:Move params:2 doc_str: Move value at addr2 (p2) to addr1 (p1)
      *(word *)(ref_map(instr->p1)) = *(word *)ref_map(instr->p2);
      break;

    case 0x06:
      // sym:Goto params:1 doc_str: shift execution to label (p1), see Label
      goto_label(instr->p1);
      break;

    case 0x07:
      // sym:IfEq params:2 doc_str: execute next instruction if the value at addr1 (p1) == value at addr2 (p2) otherwise skip an instruction
      if (!(*(word *)ref_map(instr->p1) == *(word *)ref_map(instr->p2)))
        instr_buffer_index++; // skip to next instruction
      break;

    case 0x08:
      // sym:IfLe params:2 doc_str: execute next instruction if the value at addr1 (p1) < value at addr2 (p2) otherwise skip an instruction
      if (!(*(word *)ref_map(instr->p1) < *(word *)ref_map(instr->p2)))
        instr_buffer_index++; // skip to next instruction
      break;

    case 0x09:
      // sym:IfLeEq params:2 doc_str: execute next instruction if the value at addr1 (p1) <= value at addr2 (p2) otherwise skip an instruction
      if (!(*(word *)ref_map(instr->p1) <= *(word *)ref_map(instr->p2)))
        instr_buffer_index++;
      break;

    case 0x0A: // delay
      // sym:Delay params:1 doc_str: delay execution to for a period found at addr (p1).
      delay(*(word *)ref_map(instr->p1));
      break;

    case 0x0B:
      // sym:Millis params:0 doc_str: return relative clock (ms) repeats every 1.09 min
      return_val = millis();
      break;

    case 0x0C:
      // sym:Seconds params:0 doc_str: return relative clock (s)
      return_val = millis() / 1000;
      break;

    case 0x0D:
      // sym:Minutes params:0 doc_str: return relative clock (m)
      return_val = millis() / 60000;
      break;

    case 0x0E:
      // sym:Hours params:0 doc_str: return relative clock (h)
      return_val = millis() / 3600000;
      break;

    case 0x0F:
      // sym:SerialBytesAvailable params:0 doc_str: return number of bytes available in serial buffer
      return_val = Serial.available();
      break;

    case 0x10:
      // sym:SerialRead params:0 doc_str: return word in serial buffer
      Serial.readBytes((byte *) &return_val, sizeof(word));
      break;

    case 0x11:
      // sym:PrintWord params:1 doc_str: print integer value of value at addr (p1)
      Serial.print(*(word *)ref_map(instr->p1));
      break;

    case 0x12:
      // sym:PrintChar params:1 doc_str: print char of value at addr (p1)
      Serial.print(*(char *)ref_map(instr->p1));
      break;

    case 0x13:
      // sym:PrintBin params:1 doc_str: print raw binary value at addr (p1). Prints two chars
      Serial.print(*(char *)ref_map(instr->p1));
      Serial.print(*((char *)ref_map(instr->p1) + 1));
      break;

    case 0x14:
      // sym:NewLine params:0 doc_str: print new line
      Serial.print("\n");
      break;

    /*
     * 0x30 - 0x34 Digital Input and Ouptut
    */


    case 0x30:
      // sym:PinMode params:2 doc_str: set pin mode of pin (p1) to mode (p2). Mode: 0 -> OUTPUT, 1 -> INPUT, 2 -> INPUT_PULLUP
      dio_pinMode(instr->p1, instr->p2);
      break;

    case 0x31:
      // sym:DigitalWrite params:2 doc_str: write state at addr (p2) to pin (p1)
      dio_digitalWrite(instr->p1, *(word *)ref_map(instr->p2));
      break;

    case 0x32:
      // sym:DigitalRead params:1 doc_str: return state of pin (01)
      return_val = dio_digitalRead(instr->p1);
      break;

    case 0x33:
      // sym:AnalogWrite params:1 doc_str: write state at addr (p2) to pin (p1)
      dio_analogWrite(instr->p1, *(word *)ref_map(instr->p2));
      break;

    case 0x34: // Read DIO state
      // sym:AnalogRead params:1 doc_str: return state of pin (01). Only appies for analog pins (A0:14, A1:15...)
      return_val = dio_analogRead(instr->p1);
      break;


    /*
     * 0x40 - 0x46 Memory Operations
    */
    
    case 0x40:
      // sym:ClearDataBuffer params:0 doc_str: clears the data buffer
      clear_data_buffer();
      break;

    case 0x41:
      // sym:ReadBootAction params:0 doc_str: read boot action from EEPROM
      return_val = read_boot_action();
      break;

    case 0x42:
      // sym:WriteBootAction params:1 doc_str: write boot action to EEPROM
      write_boot_action(instr->p1);
      break;

    case 0x43:
      // sym:ReadID params:0 doc_str: read id from EEPROM
      return_val = read_id();
      break;

    case 0x44:
      // sym:WriteID params:1 doc_str: write id to EEPROM
      write_id(instr->p1);
      break;

    case 0x45:
      // sym:ReadInstrBuffer params:0 doc_str: read instruction buffer from EEPROM
      read_EEPROM_instr_buffer();
      break;

    case 0x46:
      // sym:WriteInstrBuffer params:0 doc_str: write instruction buffer to EEPROM
      write_EEPROM_instr_buffer();
      break;

    /*
     * 0x50 - 0x51 Numerics
    */

    case 0x50:
      // sym:GetRandom params:0 doc_str: return random word
      return_val = random(0, 65536);
      break;

      //section:END_OP_DEFINITION
  }
}

void exec_instr_buffer() {
  // execute instruction buffer

  //return retval to default state
  return_val = 0;
  
  // reset abort to false
  end_pgrm = false;
  instr_buffer_index = 0;
  data_buffer_index = 0;

  for (instr_buffer_index = 0; instr_buffer_index < MAX_INSTR_BUFFER_LEN; instr_buffer_index++) {

    // abort if needed
    if (end_pgrm)
      break;

    // execute single instruction
    exec(&instr_buffer[instr_buffer_index]);

    // debugging
//    Serial.print(instr_buffer[instr_buffer_index].op, HEX);
//    Serial.print(" ");
//    Serial.print(instr_buffer[instr_buffer_index].p1, HEX);
//    Serial.print(" ");
//    Serial.println(instr_buffer[instr_buffer_index].p2, HEX);
  }

}
