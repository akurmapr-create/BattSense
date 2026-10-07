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
    const BatteryDiagnostics diagnostics = battery.getDiagnostics();

    const float stateOfCharge =
        socEstimator.estimate(diagnostics.batteryVoltage);

    display.showBattery(
        diagnostics.batteryVoltage,
        stateOfCharge
    );

    Serial.println("----- Battery Diagnostics -----");

    Serial.print("Raw ADC:          ");
    Serial.println(diagnostics.rawAdcCount);

    Serial.print("ADC Voltage:      ");
    Serial.print(diagnostics.adcVoltage, 3);
    Serial.println(" V");

    Serial.print("Battery Voltage:  ");
    Serial.print(diagnostics.batteryVoltage, 3);
    Serial.println(" V");

    Serial.print("Estimated SoC:    ");
    Serial.print(stateOfCharge, 1);
    Serial.println(" %");

    Serial.println("-------------------------------");

    delay(1000);
}