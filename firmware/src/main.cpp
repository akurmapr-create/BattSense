#include <Arduino.h>

#include "BatteryMonitor.h"
#include "StateOfChargeEstimator.h"
#include "DisplayManager.h"

BatteryMonitor battery;
StateOfChargeEstimator socEstimator;
DisplayManager display;

void setup()
{
    Serial.begin(115200);

    battery.begin();

    if (!display.begin())
    {
        Serial.println("OLED initialization failed.");

        while (true)
        {
            delay(1000);
        }
    }

    Serial.println("BattSense initialized.");
}

void loop()
{
    const float batteryVoltage = battery.getVoltage();

    const float stateOfCharge =
        socEstimator.estimate(batteryVoltage);

    display.showBattery(
        batteryVoltage,
        stateOfCharge
    );

    Serial.print("Battery Voltage: ");
    Serial.print(batteryVoltage, 3);
    Serial.println(" V");

    Serial.print("Estimated SoC: ");
    Serial.print(stateOfCharge, 1);
    Serial.println(" %");

    delay(1000);
}