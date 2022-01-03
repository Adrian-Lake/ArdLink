/*
 * Data Management Functions
 */


#include <EEPROM.h>


word data_buffer[MAX_DATA_BUFFER_LEN];
word data_buffer_index;

// Standard instructional unit
// currently 6 bytes per instruction
struct instruction {
  word op; // operation
  word p1; // paramter1
  word p2; // paramter2
};

instruction instr_buffer[MAX_INSTR_BUFFER_LEN];
word instr_buffer_index; // index of currently executed buffer

word return_val = 0; // global return val for last operation
word temp_a = 0; // global return val for last operation
word temp_b = 0; // global return val for last operation
word temp_c = 0; // global return val for last operation
word temp_d = 0; // global return val for last operation

// these should be booleans, but we are using words as the smallest data type
word exec_pgrm = false; // set true to execute prgm once!
word end_pgrm = false; // prevents additional execution if true
word loop_count = 0; // prevents additional execution if true

void * ref_map(word address) {
  // map refrences to common addresses.

  word subaddress = address & 0x3FF;
  word selected_offset = address & 0xC000;

  switch (selected_offset) { // switch for speed

    case (SYSTEM_CONFIG_OFFSET): // system config

      if (subaddress == 0x0001) { // execute program
        return &exec_pgrm;
      } else if (subaddress == 0x0002) { // abort program
        return &end_pgrm;
      } else if (subaddress == 0x0003) { // instruction buffer index
        return &instr_buffer_index;
      } else if (subaddress == 0x0004) { // data buffer index
        return &data_buffer_index;
      } else if (subaddress == 0x0005) { // prgm loop count
        return &loop_count;
      } else if (subaddress == 0x0010) { // return val
        return &return_val;
      } else  if (subaddress == 0x0011) { // temp a
        return &temp_a;
      } else  if (subaddress == 0x0012) { // temp b
        return &temp_b;
      } else  if (subaddress == 0x0013) { // temp c 
        return &temp_c;
      } else  if (subaddress == 0x0014) { // temp d
        return &temp_d;
      } else {
        return 0;
      }

    case (INSTR_BUFFER_OFFSET): // instruction buffer

      return ((byte *)instr_buffer) + subaddress; // important, we write bytes here!

    case (DATA_BUFFER_OFFSET): // data buffer

      return ((word *)data_buffer) + subaddress;

  }
}

void clear_data_buffer() {
  for (int i = 0; i < MAX_DATA_BUFFER_LEN; i++) {
    data_buffer[i] = 0;
  }
}

void write_EEPROM_instr_buffer(){
  // write instructions from buffer to EEPROM
  byte *instr_bytes = (byte *) &instr_buffer;
  word instr_bytes_len = MAX_INSTR_BUFFER_LEN*sizeof(instruction);
  word instr_offset = MAX_INSTR_BUFFER_LEN*sizeof(instruction);
  
  for (int i = 0; i < instr_bytes_len; i++) {
    EEPROM.update(i + instr_offset, instr_bytes[i]);
  }
}


void read_EEPROM_instr_buffer(){
  // read instructions from buffer to EEPROM
  byte *instr_bytes = (byte *) &instr_buffer;
  word instr_bytes_len = MAX_INSTR_BUFFER_LEN*sizeof(instruction);
  word instr_offset = MAX_INSTR_BUFFER_LEN*sizeof(instruction);
  
  for (int i = 0; i < instr_bytes_len; i++) {
    instr_bytes[i] = EEPROM.read(i + instr_offset);
  }
}


void format_EEPROM(){
  // set all EEPROM bytes to 0
  
  // My documentation may be from another IDE version, this does not exist in my env
  // users also report issues with nodeMCU
  //  EEPROM.clear(); 
  
  // ... temporary alternative
  for(int i = 0; i < EEPROM.length(); i++)
    EEPROM.write(i, 0);
}

word read_boot_action(){
  word action;
  return EEPROM.get(EEPROM_BOOT_ACTION, action);
  return action;
}

void write_boot_action(word action){
  EEPROM.put(EEPROM_BOOT_ACTION, action);
}

word read_id(){
  word id;
  return EEPROM.get(EEPROM_ID, id);
  return id;
}

void write_id(word id){
  EEPROM.put(EEPROM_ID, id);
}
