#include <Preferences.h>
#include <HardwareSerial.h>

#include "Led.h"
#include "OLed.h"
#include "App.h"

App::App()
    : led(new Led(LED_R_PIN, LED_G_PIN, LED_B_Pin)),
      oLed(new OLed(OLed_ADDRESS, OLed_SDA_PIN, OLed_SCL_PIN))
{
}

void App::setup()
{
    Serial.begin(SERIAL_RATE);

    led->setup();
    oLed->setup();
}
