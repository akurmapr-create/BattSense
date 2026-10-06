#include <Arduino.h>
#include "BatteryMonitor.h"
#include "StateOfChargeEstimator.h"

BatteryMonitor battery;
StateOfChargeEstimator socEstimator;

void setup()
{
    Serial.begin(115200);
    battery.begin();

    Serial.println();
    Serial.println("================================");
    Serial.println("       BattSense Rev A");
    Serial.println("================================");
}

void loop()
{
    const BatteryDiagnostics diagnostics = battery.getDiagnostics();

    const float stateOfCharge =
        socEstimator.estimate(diagnostics.batteryVoltage);

    Serial.println();
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