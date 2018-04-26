#include "Keyboard.hpp"

Keyboard::Keyboard(BLEServer *bleServer, char *keyboardServiceUuid, char *hidCharacteristicUuid)
    : bleServer(bleServer)
{
    bleServer->setCallbacks(this);

    keyboardService = bleServer->createService(keyboardServiceUuid);

    Hid::init(keyboardService, hidCharacteristicUuid);

    keyboardService->start();
}

void Keyboard::onDisconnect(BLEServer *bleServer)
{
    Serial.println("Keyboard: onDisconnect...");

    connected = false;
}

void Keyboard::onConnect(BLEServer *bleServer)
{
    Serial.println("Keyboard: onConnect...");

    connected = true;
}

Keyboard *Keyboard::getSingleTon(BLEServer *bleServer, char *keyboardServiceUuid, char *hidCharacteristicUuid)
{
    if (singleTon == NULL)
    {
        singleTon = new Keyboard(bleServer, keyboardServiceUuid, hidCharacteristicUuid);
    }

    return (singleTon);
}

void Keyboard::init(BLEServer *bleServer, char *keyboardServiceUuid, char *hidCharacteristicUuid)
{
    getSingleTon(bleServer, keyboardServiceUuid, hidCharacteristicUuid);

    bleServer->getAdvertising()->start();

    Serial.println("Keyboard: Waiting a client connection...");
}

bool Keyboard::isConnected()
{
    return connected;
}

Keyboard *Keyboard::singleTon = NULL;
BLEService *Keyboard::keyboardService;
bool Keyboard::connected = false;
