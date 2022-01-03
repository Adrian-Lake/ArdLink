/*
 * Host communication functions
 */

word parse_timeout = 100; // timeout on reading complete instruction

//instruction* input_instr;
instruction parsed_instr;

void read_instr() {
  //Read instruction binary from serial stream
  

  bool parse_instr =  false;
  unsigned long parse_start;
 
  while (Serial.available()) {

    if (parse_instr) {
      // break if the input command is taking too long
      if (parse_start + parse_timeout < millis()) {
        parse_instr = false;
        break;
      }


    } else {
      parse_instr = true;
      parse_start = millis();
    }

    // when we have enough data, break
    if (Serial.available() >= sizeof(instruction)) {

      // read in operation instructions
        Serial.readBytes((byte *) &parsed_instr, sizeof(instruction));    
 
      break;

    }
  }

  // clear serial buffer
  while (Serial.available())
    Serial.read();

}
