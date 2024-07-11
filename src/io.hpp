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

#include <Array.h>
#include <assert.h>

class IO
{
private:
    #if defined (ARDUINO_SAMD_MKRWIFI1010)
        constexpr static uint8_t MAX_PIN_COUNT = 21;
    #elif defined (ARDUINO_AVR_MEGA2560)
        constexpr static uint8_t MAX_PIN_COUNT = 70;
    #else
        #error "Board not supported - Please open a issue on Github: https://github.com/AlexanderTonn/Arduino_IOclass/issues"
    #endif

    bool  mPinAssigned [MAX_PIN_COUNT] = {false}; // stores boolean state whether it's assumed or not
    uint8_t mPinMode[MAX_PIN_COUNT]; // stores the mode of the pin
    
    public:
    enum class mode
    {
        DIGITAL_IN ,
        DIGITAL_OUT ,
        ANALOG_IN ,
        ANALOG_OUT ,
        BUS
    };

    auto reservePin(const uint8_t, mode) -> bool;
    auto setValue(const uint8_t, const uint16_t) -> void;

    auto readAll() -> void; 
    auto writeAll() -> void;
    uint16_t mRawData[MAX_PIN_COUNT]; // Array for storing the raw data from the pins or the data which should be written to the pins

    private:
    auto setPinMode(const uint8_t, const mode) -> void;
    auto getValue(const uint8_t) -> int32_t;
    auto printSerial(String) -> void; 
};

    


#endif