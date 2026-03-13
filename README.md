# Arduino_IOclass

This is a library to handle the IO operations on the Arduino. It is designed to be used in a more complex project where you have to handle a lot of IO operations. The libary is also able to prevent unexpected behavior by checking if the pins are already in use or if the pins are correctly defined. If you use the library correctly you don't have to worry about the IO operations anymore. For this reason ask you to read the documentation carefully.
## Usage

### Setup

- i recommend to create a separate file for defining the pins

**Example of defining the pins**

- in this example i declared the variables as `PROGMEM` to store them in the flash memory (because the pin Definition will not change during the runtime of the program)

```C++
constexpr static uint8_t RELAY1 PROGMEM = 2;
constexpr static uint8_t RELAY2 PROGMEM = 3;
constexpr static uint8_t RELAY3 PROGMEM = 4;
```

- Now we have to include the `io.hpp` 
- we also have to create an instance of the `IO` class and pass the pinMapping to the setup function

```C++
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
``` 

### Writing Values
* to write a value to a pin, it's important to you use the ```setValue(const uint8_t, const uint16_t)``` function which is checking if the pin is reserved as an output 
* in the last line of you ```void loop()``` you call the function ```writeAll()``` which is writing all values to the pins at the same time

### Reading Values 
* basically you only have to call ```readAll()``` in the first line of the loop routine
* the values are stored in the `IO` class and can be accessed directly by using the `get()` function

# Special functions
## ActivateValidationError(const bool)
* use this function in the `setup()` routine
* if it set to `true` the function will block the code execution of the programm if a critical configuration error has been detected (e.g. if you try to set Digital Outputs to UART Pins)

## ActivateOutputDelay(const bool, const uint32_t)
* this function is useful if you want to delay the actuation of digitaloutputs
* this function can reduce power spikes if you try to actuate consumer with high power consumption
