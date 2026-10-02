#include <Arduino.h>

void setup()
{
    Serial.begin(115200);

    while (!Serial)
    {
        // Wait for serial port (safe on ESP32; exits immediately on most boards)
    }

    Serial.println();
    Serial.println("================================");
    Serial.println("BattSense Firmware Booting...");
    Serial.println("================================");
}

void loop()
{
    Serial.print("Uptime: ");
    Serial.print(millis());
    Serial.println(" ms");

    delay(1000);
}