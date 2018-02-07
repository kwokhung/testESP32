#include <HardwareSerial.h>
#include <Wire.h>

#include "SbrControl.h"

SbrControl::SbrControl(std::string name)
    : name(name)
{
    task = NULL;
}

void SbrControl::setup()
{
    Serial.println("SbrControl::setup");

    setup_serial_control();
}

void SbrControl::setup_serial_control()
{
    SerialControl.begin(9600, SERIAL_8N1, 17, 16);

    //xTaskCreatePinnedToCore(serialControlLoop, "serialControlLoop", 4096, NULL, 2, NULL, xPortGetCoreID());
}

void SbrControl::startUp(void *parameter)
{
    SbrControl *sbrControl = (SbrControl *)parameter;

    sbrControl->setup();

    Serial.println("\nStarting thread dealing with Serial Control requests...");
    uint8_t currChar;

    while (true)
    {
        while (SerialControl.available())
        {
            currChar = SerialControl.read();

            if (startNewMsg(currChar))
            {
                _readingMsg = true;
                _msgPos = 0;
            }
            else if (_readingMsg)
            {
                if (_msgPos >= 6)
                {
                    // data finished, last byte is the CRC
                    uint8_t crc = 0;

                    for (uint8_t i = 0; i < 6; i++)
                    {
                        crc += _msg[i];
                    }

                    if (crc == currChar)
                    {
                        joystickX = _msg[0];
                        joystickY = _msg[1];
                        _validData = true;
                    }
                    else
                    {
                        _validData = false;
                        Serial.print("Wrong CRC: ");
                        Serial.print(currChar);
                        Serial.print(" Expected: ");
                        Serial.println(crc);
                    }

                    _readingMsg = false;
                }
                else
                {
                    // normal data, add it to the message
                    _msg[_msgPos++] = currChar;
                }
            }
        }

        delay(1);
    }

    vTaskDelete(NULL);
}

boolean SbrControl::startNewMsg(uint8_t c)
{
    boolean res = (_prevChar == 0) && (c == 255);
    _prevChar = c;

    return res;
}

boolean SbrControl::isValidJoystickValue(uint8_t joystick)
{
    return joystick > 20 && joystick < 230;
}

HardwareSerial &SbrControl::SerialControl = *new HardwareSerial(1);
uint8_t SbrControl::_prevChar;
boolean SbrControl::_readingMsg = false;
uint8_t SbrControl::_msgPos;
boolean SbrControl::_validData = false;
uint8_t SbrControl::_msg[6];
uint8_t SbrControl::joystickX;
uint8_t SbrControl::joystickY;
