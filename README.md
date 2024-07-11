# Arduino_IOclass

This is a library to handle the IO operations on the Arduino. It is designed to be used in a more complex project where you have to handle a lot of IO operations. The libary is also able to prevent unexpected behavior by checking if the pins are already in use or if the pins are correctly defined. If you use the library correctly you don't have to worry about the IO operations anymore. For this reason ask you to read the documentation carefully.
## Usage

### Setup

- i recommend to create a separate file for defining the pins

**Example of defining the pins**

- in this example i stored the pins in a file called `pinMapping.h` and i declared the variables as `PROGMEM` to store them in the flash memory (because the pin Definition will not change during the runtime of the program)

```C++
constexpr static  uint8_t EXT_FWT_FILL_REQUEST PROGMEM = 22; // Digital Input
constexpr static  uint8_t BUFFER_TANK_FILL_START PROGMEM = 23;// Digital Input

// ...

constexpr static  uint8_t LED_PLANT_OPERATIONAL PROGMEM = 10; // Digital Output

// ...

constexpr static  uint8_t P_FRESHWATER PROGMEM = A0; // Analog Input
constexpr static  uint8_t P_CONCENTRATE PROGMEM = A1; // Analog Input

// ...

constexpr static  uint8_t SPI_CS PROGMEM = 53; // SPI Bus Pin | SPI chip select / Slave Select
constexpr static  uint8_t SPI_COPI PROGMEM = 51; // SPI Bus Pin | SPI MOSI
constexpr static  uint8_t SPI_SCK PROGMEM = 52; // SPI Bus Pin
constexpr static  uint8_t SPI_CIPO PROGMEM = 50; // SPI Bus Pin | SPI MISO
```

- Now we have to include the `Arduino_IOclass.h` and the `pinMapping.h` in the main file
- we also have to create an instance of the `IO` class and pass the pinMapping to the setup function

```C++
#include <Arduino.h>
#include <io.hpp>
#include "pinMapping.hpp"

IO _io;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  _io.reservePin(EXT_FWT_FILL_REQUEST, IO::mode::DIGITAL_IN);
  _io.reservePin(BUFFER_TANK_FILL_START, IO::mode::DIGITAL_IN);

  _io.reservePin(LED_PLANT_OPERATIONAL, IO::mode::DIGITAL_OUT);
  
  _io.reservePin(P_FRESHWATER, IO::mode::ANALOG_IN);
  _io.reservePin(P_CONCENTRATE, IO::mode::ANALOG_IN);

  // if you use some pins for bus communication you can reserve them as well
  _io.reservePin(SPI_CS, IO::mode::BUS);
  _io.reservePin(SPI_COPI, IO::mode::BUS);
  _io.reservePin(SPI_SCK, IO::mode::BUS);
  _io.reservePin(SPI_CIPO, IO::mode::BUS);
}

```
### Reading Values 
* basically you only have to call ```readAll()``` in the first line of the loop routine
* the values are stored in the `IO` class and can be accessed directly by using the public member array ```mRawData```

```C++
void loop() {
  // put your main code here, to run repeatedly:
  _io.readAll();
  
  Serial.print("Freshwater Pressure: ");
  Serial.println(_io.mRawData[P_FRESHWATER]);
  
  Serial.print("Concentrate Pressure: ");
  Serial.println(_io.mRawData[P_CONCENTRATE]);
  
  Serial.print("External Freshwater Fill Request: ");
  Serial.println(_io.mRawData[EXT_FWT_FILL_REQUEST]);
  
  Serial.print("Buffer Tank Fill Start: ");
  Serial.println(_io.mRawData[BUFFER_TANK_FILL_START]);
  
  Serial.print("Plant Operational: ");
  Serial.println(_io.mRawData[LED_PLANT_OPERATIONAL]);
  
  delay(1000);
}
``` 

### Writing Values
* to write a value to a pin, it's important to you use the ```setValue(const uint8_t, const uint16_t)``` function which is checking if the pin is reserved as an output 
* in the last line of you ```void loop()``` you call the function ```writeAll()``` which is writing all values to the pins at the same time

```C++

void loop() {

  _io.setValue(LED_PLANT_OPERATIONAL, HIGH);
  _io.setValue(EXT_FWT_FILL_REQUEST, LOW); // this will not be written to the pin because it is reserved as an input
  _io.writeAll();
  
  delay(1000);
}
```	
