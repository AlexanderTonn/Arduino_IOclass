#include "io.hpp"
/**
 * @brief
 *
 * @param pin
 * @param _pinMode
 * @return true
 * @return false
 */
auto IO::reservePin(const uint8_t pin, mode _pinMode) -> bool
{
    // Already reserved

    if (mPinAssigned[pin])
    {
        printSerial("Digital Pin :" + String(pin) + " already reserved");
        return false;
    }

    setPinMode(pin, _pinMode);
    mPinAssigned[pin] = true;
    mPinMode[pin] = static_cast<uint8_t>(_pinMode);

    return true;
}
/**
 * @brief Read the value from the pin
 *
 * @param pin Pin no. to read from
 * @return int32_t
 */
auto IO::getValue(const uint8_t pin) -> int
{
    if (!mPinAssigned[pin])
    {
        printSerial("Pin " + String(pin) + " is not reserved");
        return -1;
    }

    const auto pinMode = static_cast<uint8_t>(mPinMode[pin]);

    int ret = 0;
    switch (pinMode)
    {
    case static_cast<uint8_t>(mode::DIGITAL_IN_PULLUP):
    case static_cast<uint8_t>(mode::DIGITAL_IN): 
        ret = digitalRead(pin);
        break;
    case static_cast<uint8_t>(mode::DIGITAL_OUT):
        break;
    case static_cast<uint8_t>(mode::ANALOG_IN_PULLUP):
    case static_cast<uint8_t>(mode::ANALOG_IN) : 
        ret = analogRead(pin);
        break;
    case static_cast<uint8_t>(mode::ANALOG_OUT):
        break;
    default:
        return -1;
        break;
    }

    return ret;
}
/**
 * @brief Set the value of the pin
 *
 * @param pin target pin
 * @param setValue value to set
 */
auto IO::setValue(const uint8_t pin, const uint16_t setValue) -> void
{
    if (!mPinAssigned[pin])
    {
        printSerial("Pin " + String(pin) + " is not reserved");
        return;
    }

    const auto pinMode = mPinMode[pin];

    switch (pinMode)
    {
    case static_cast<uint8_t>(mode::DIGITAL_IN):
        break;

    case static_cast<uint8_t>(mode::DIGITAL_OUT):

        if (setValue < 0 || setValue > 1)
        {
            printSerial("Pin " + String(pin) + " defined as Digital Output - value is out of range.");
            return;
        }
        mRawData[pin] = setValue;

        break;
    case static_cast<uint8_t>(mode::ANALOG_IN):
        printSerial("Pin " + String(pin) + " cannot write to a analog input pin.");
        break;
    case static_cast<uint8_t>(mode::ANALOG_OUT):
        mRawData[pin] = setValue;

        break;
    default:
        return;
        break;
    }
}
/**
 * @brief
 *
 * @param pin
 * @param _pinMode
 */
auto IO::setPinMode(const uint8_t pin, const mode _pinMode) -> void
{
    switch (_pinMode)
    {
    case mode::DIGITAL_IN:
        pinMode(pin, INPUT);
        break;
    case mode::DIGITAL_OUT:
        pinMode(pin, OUTPUT);
        break;
    case mode::ANALOG_IN:
        break;
    case mode::ANALOG_OUT:
        break;
    case mode::ANALOG_IN_PULLUP:
    case mode::DIGITAL_IN_PULLUP:
        pinMode(pin, INPUT_PULLUP);
        break;
    default:
        break;
    }
}
/**
 * @brief Call this first in the void loop() to read all the values from the reserved pins
 * @note The values will be stored in class internal array
 */
auto IO::readAll() -> void
{
    for (auto i = 0; i < MAX_PIN_COUNT; i++)
    {
        if(!mPinAssigned[i])
            continue; 
        
        if (mPinAssigned[i] == true && (mPinMode[i] == static_cast<uint8_t>(mode::ANALOG_IN) || mPinMode[i] == static_cast<uint8_t>(mode::DIGITAL_IN)))
            {
                mRawData[i] = getValue(i);
            }
    }
}
/**
 * @brief Call this at the end of the void loop() to write all the values to the reserved pins
 * @note Internal handling is checking if the data of the pins are valid
 */
auto IO::writeAll() -> void
{
    for (auto i = 0; i < MAX_PIN_COUNT; i++)
    {

        if (mPinAssigned[i])
        {
            if (mPinMode[i] == static_cast<uint8_t>(mode::DIGITAL_OUT))
                digitalWrite(i, mRawData[i]);

            else if (mPinMode[i] == static_cast<uint8_t>(mode::ANALOG_OUT))
                analogWrite(i, mRawData[i]);
        }
    }
}

/**
 * @brief
 *
 * @param _String
 */
auto IO::printSerial(String _String) -> void
{
    if (Serial)
        Serial.println(_String);
}
/**
 * @brief scaling a float value from one range to another
 * @note If you have a int value, please use the map() function from official Arduino library (syntax is the same)
 * @param x value to scale
 * @param in_min minimum value of input range
 * @param in_max maximum value of input range
 * @param out_min minimum value of output range
 * @param out_max maximum value of output range
 * @return float
 */
auto IO::fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max) -> float
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}