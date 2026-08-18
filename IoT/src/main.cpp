#include <Arduino.h>

// Set a unique identifier for your device before importing comms.h
const char *mqttClient = "ESp32_Jordyn"; // EDIT THIS FIELD

const char *mqttTopic;

#include <Arduino.h>
#include "comms.h"
#include <Wire.h>
#include "Adafruit_ADT7410.h"

// Create the ADT7410 temperature sensor object
Adafruit_ADT7410 tempsensor = Adafruit_ADT7410();

void performActionBasedOnPayload(String payload)
{
    Serial.print("Payload: ");
    Serial.println(payload);
    if ((char)payload[0] == '1')
    {
        Serial.println("LED ON");
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    wifiSetup();
    mqttSetup();
    while (!Serial)
    {
        delay(10);
    }
    delay(1000);

    randomSeed(analogRead(A0));
    if (!tempsensor.begin())
    {
        Serial.println("Couldn't find ADT7410!");
        while (1);
    }
}

void loop()
{
    // 1. Handle Connection Persistence
    mqttConnect(); // Ensure we are connected to the MQTT broker. If not, this will attempt to reconnect.

    // 2. Generate and send temperature data periodically
    float tempInC = tempsensor.readTempC();
    Serial.println(tempInC);
    sendPeriodicUpdate("sensorData", String(tempInC));

    client.loop(); // Check for incoming messages and keep the connection alive
    delay(100);
}