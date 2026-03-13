/**
 * @file io.hpp
 * @author Alexander Tonn
 * @brief IO Class for handling the input and outputs
 * @version 0.1
 * @date 2024-06-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef IO_HPP
#define IO_HPP

#include <Arduino.h>
#include "io_diagStrings.hpp"

class IO
{
private:
  auto getPROGMEM_String(const char *progmemString) -> String { return String((__FlashStringHelper *)progmemString); };
    
    #if defined (ARDUINO_SAMD_MKRWIFI1010)
        constexpr static uint8_t MAX_PIN_COUNT = 21;
    #elif defined (ARDUINO_AVR_MEGA2560)
        constexpr static uint8_t MAX_PIN_COUNT = 70;
    #else
        #error getPROGMEM_String(BOARD_NOT_SUPPORTED);
    #endif

    bool  mPinAssigned [MAX_PIN_COUNT] = {false}; // stores boolean state whether it's assumed or not
    uint8_t mPinMode[MAX_PIN_COUNT]; // stores the mode of the pin
    
    uint16_t mRawData[MAX_PIN_COUNT]; // Array for storing the raw data from the pins or the data which should be written to the pins
    bool mOutputDelayFlag[MAX_PIN_COUNT]; // Flag for delaying the outputs
    public:
    enum class mode
    {
        DIGITAL_IN ,
        DIGITAL_OUT ,
        ANALOG_IN ,
        ANALOG_OUT ,
        ANALOG_IN_PULLUP,
        DIGITAL_IN_PULLUP
    };

    auto reservePin(const uint8_t, mode) -> bool;
    auto set(const uint8_t, const uint16_t) -> void;
    auto get(const byte pin) -> uint16_t ;
    auto ActivateValidationError(const bool activation) -> void {mValidationErrorActivation = activation;};
    auto ActivateOutputDelay(const bool activation, const uint32_t ms) -> void {mOutputDelayActivation = activation; mOutputDelayTime = ms;};
    auto readAll() -> void; 
    auto writeAll() -> void;
    

    auto fmap(const float, const float, const float, const float, const float) -> float;
    

    private:
    bool mValidationErrorActivation; 
    bool mOutputDelayActivation;
    uint32_t mOutputDelayTime;
    auto setPinMode(const uint8_t, const mode) -> void;
    auto getValue(const uint8_t) -> int;
    auto printSerial(String) -> void; 
    auto validate(const byte pin) -> bool;
    auto delayOutputs() -> void;
    



};

    


#endif