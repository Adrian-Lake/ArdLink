/*
 * Digital and analog pin functions
 * 
 * Note the pin deffined in this app may be different than
 * the pin used directly by the ARduino IDE, especially for
 * analog pins (A0, A1, ...). see ArdunioCode-avr repo for
 * "real" pin mapping.
 * 
 * https://github.com/arduino/ArduinoCore-avr/tree/master/variants
 * 
*/


void dio_pinMode(unsigned short pin, unsigned short mode) {
  // Update the pin mode of a given pin

  // set pinmode only iff the set mode matches these options
  if (mode == 0) {
    pinMode(pin, OUTPUT);
  } else if (mode == 1) {
    pinMode(pin, INPUT);
  } else if (mode == 2) {
    pinMode(pin, INPUT_PULLUP);
  }
}

void dio_digitalWrite(unsigned short pin, unsigned short state) {
  // Update the pin mode of a given pin

  // set pin state only iff the set mode matches these options
  if (state == 0) {
    digitalWrite(pin, LOW);
  } else if (state == 1) {
    digitalWrite(pin, HIGH);
  }
}

unsigned int dio_digitalRead(unsigned short pin) {
  // Update the pin mode of a given pin

  // result will be save in return val, in interpreter
  return digitalRead(pin);
}

void dio_analogWrite(unsigned short pin, unsigned short value) {
  // Write PWM state to avaiable PWM pin
  int coerced_value = value > 255 ? 255 : value;

  analogWrite(pin, coerced_value);

}


unsigned int dio_analogRead(unsigned short pin) {
  // Read voltage from analog pin

  return analogRead(pin);

}
