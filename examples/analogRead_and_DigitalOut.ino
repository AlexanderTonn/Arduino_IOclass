#include "io.hpp"

IO _io;

constexpr static uint8_t RELAY1 PROGMEM = 2;
constexpr static uint8_t RELAY2 PROGMEM = 3;
constexpr static uint8_t RELAY3 PROGMEM = 4;

void setup() {
  Serial.begin(9600);

  // You declare here which inputs / outputs you want to read / write
  _io.ActivateValidationError(true);
  _io.ActivateOutputDelay(true, 500);
  _io.reservePin(A0, IO::mode::ANALOG_IN);

  _io.reservePin(RELAY1, IO::mode::DIGITAL_OUT);
  _io.reservePin(RELAY2, IO::mode::DIGITAL_OUT);
  _io.reservePin(RELAY3, IO::mode::DIGITAL_OUT);

}

// Call readAll() first
// Call writeAll() last
// This sequence play safe that you read first all inputs and write all outputs at least
// This is standard approach for reliable and good maintainable software
void loop() {

  _io.readAll();
  
  // Your code here
  auto scaled = map(_io.get(A0), 0, 1023, 0, 5000);
  if (scaled > 3000)
  {
    _io.set(RELAY1, HIGH);
    _io.set(RELAY2, HIGH);
    _io.set(RELAY3, HIGH);
  }
  if (scaled < 1500)
  {
    _io.set(RELAY1, LOW);
    _io.set(RELAY2, LOW);
    _io.set(RELAY3, LOW);
  }

  _io.writeAll();

}
