#include "io.hpp"
/**
   @brief

   @param pin
   @param _pinMode
   @return true
   @return false
*/
auto IO::reservePin(const uint8_t pin, mode _pinMode) -> bool
{
  // Already reserved

  if (mPinAssigned[pin])
  {
    printSerial(getPROGMEM_String(DIGITAL_PIN_) + String(pin) + getPROGMEM_String(ALREADY_RESERVED));
    return false;
  }

  validate(pin);

  setPinMode(pin, _pinMode);
  mPinAssigned[pin] = true;
  mPinMode[pin] = static_cast<uint8_t>(_pinMode);

  return true;
}
/**
   @brief Read the value from the pin

   @param pin Pin no. to read from
   @return int32_t
*/
auto IO::getValue(const uint8_t pin) -> int
{
  if (!mPinAssigned[pin])
  {
    printSerial(getPROGMEM_String(PIN_) + String(pin) + getPROGMEM_String(IS_NOT_RESERVED));
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
   @brief Set the value of the pin

   @param pin target pin
   @param setValue value to set
*/
auto IO::set(const uint8_t pin, const uint16_t setValue) -> void
{
  if (!mPinAssigned[pin])
  {
    printSerial(getPROGMEM_String(PIN_) + String(pin) + getPROGMEM_String(IS_NOT_RESERVED));
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
        printSerial(getPROGMEM_String(PIN_) + String(pin) +  getPROGMEM_String(DEFINED_AS_DIG_OUT_OOF));
        return;
      }
      mRawData[pin] = setValue;

      break;
    case static_cast<uint8_t>(mode::ANALOG_IN):
      printSerial(getPROGMEM_String(PIN_) + String(pin) + getPROGMEM_String(CANNOT_WRITE_TO_ANALOG_IN));
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
   @brief

   @param pin
   @param _pinMode
*/
auto IO::setPinMode(const uint8_t pin, const mode _pinMode) -> void
{
  switch (_pinMode)
  {
    case mode::DIGITAL_IN:
      pinMode(pin, INPUT);
      break;
    case mode::ANALOG_OUT:
    case mode::DIGITAL_OUT:
      pinMode(pin, OUTPUT);
      break;
    case mode::ANALOG_IN:
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
   @brief Call this first in the void loop() to read all the values from the reserved pins
   @note The values will be stored in class internal array
*/
auto IO::readAll() -> void
{
  for (auto i = 0; i < MAX_PIN_COUNT; i++)
  {
    if (!mPinAssigned[i])
      continue;

    if (mPinAssigned[i] == true && (mPinMode[i] == static_cast<uint8_t>(mode::ANALOG_IN) || mPinMode[i] == static_cast<uint8_t>(mode::DIGITAL_IN) || (mPinMode[i] == static_cast<uint8_t>(mode::ANALOG_IN_PULLUP) || mPinMode[i] == static_cast<uint8_t>(mode::DIGITAL_IN_PULLUP))))
    {
      mRawData[i] = getValue(i);
    }
  }
}
/**
   @brief Call this at the end of the void loop() to write all the values to the reserved pins
   @note Internal handling is checking if the data of the pins are valid
*/
auto IO::writeAll() -> void
{

  for (auto i = 0; i < MAX_PIN_COUNT; i++)
  {

    if (mPinAssigned[i])
    {
      if (mPinMode[i] == static_cast<uint8_t>(mode::ANALOG_OUT))
        analogWrite(i, mRawData[i]);
    }
  }

  if (mOutputDelayActivation)
  {
    delayOutputs();
    return;
  }

  for (auto i = 0; i < MAX_PIN_COUNT; i++)
  {
    if (mPinAssigned[i])
    {
      if (mPinMode[i] == static_cast<uint8_t>(mode::DIGITAL_OUT))
        digitalWrite(i, mRawData[i]);
    }
  }
}

/**
   @brief

   @param _String
*/
auto IO::printSerial(String _String) -> void
{
  if (Serial)
    Serial.println(_String);
}
/**
   @brief scaling a float value from one range to another
   @note If you have a int value, please use the map() function from official Arduino library (syntax is the same)
   @param x value to scale
   @param in_min minimum value of input range
   @param in_max maximum value of input range
   @param out_min minimum value of output range
   @param out_max maximum value of output range
   @return float
*/
auto IO::fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max) -> float
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

auto IO::get(const byte pin) -> uint16_t
{
  if (!mPinAssigned[pin])
  {
    printSerial(getPROGMEM_String(PIN_) + String(pin) + getPROGMEM_String(IS_NOT_RESERVED));
    return -1;
  }

  return mRawData[pin];
}
/**
  @brief Check whether the reserved pin is set on a pin with a specific functionality
*/
auto IO::validate(const byte pin) -> bool
{
  auto ret = true;
#if defined (ARDUINO_AVR_MEGA2560)

  if (pin == 0 || pin == 1)
  {
    printSerial(getPROGMEM_String(CRIT_ERROR));
    printSerial(getPROGMEM_String(IS_UART));
    ret = false;
  }

#endif

  // if mValidationErrorActivation is set, the code will be blocked
  // this can be used to play safe that the pins are assigned properly
  if (mValidationErrorActivation && !ret)
    while (1);


  return ret;
}
/**
  @brief Delaying the outputs will only affect the Digital Outputs
*/
auto IO::delayOutputs() -> void
{
  static auto timestamp = millis();
  static auto timestampLastActuation = millis();

  for (auto i = 0; i < MAX_PIN_COUNT; i++)
  {

    if (mPinAssigned[i])
    {
      if (mPinMode[i] == static_cast<uint8_t>(mode::DIGITAL_OUT))
      {
          //Switch On delayed
          if (!mOutputDelayFlag[i] && mRawData[i] == HIGH )
          {
            //Check the last Digital_Out Input, whether it's on
            if (millis() >= timestampLastActuation + mOutputDelayTime)
            {
              digitalWrite(i, HIGH);
              mOutputDelayFlag[i] = true;
              timestampLastActuation = millis();
              continue;
            }
        }


        if (mOutputDelayFlag[i] && mRawData[i] == HIGH)
          digitalWrite(i, HIGH);

        if (mRawData[i] == LOW)
        {
          digitalWrite(i, LOW);
          mOutputDelayFlag[i] = false;
        }

      }

    }
  }
}
