#include <Arduino.h>
#include "BatteryMonitor.h"

BatteryMonitor battery;

void setup()
{
    Serial.begin(115200);
    battery.begin();

    Serial.println();
    Serial.println("BattSense Hardware Validation");
    Serial.println("-----------------------------");
}

void loop()
{
    BatteryDiagnostics diagnostics = battery.getDiagnostics();

    Serial.print("Raw ADC: ");
    Serial.println(diagnostics.rawAdcCount);

    Serial.print("ADC Voltage: ");
    Serial.print(diagnostics.adcVoltage, 3);
    Serial.println(" V");

    Serial.print("Battery Voltage: ");
    Serial.print(diagnostics.batteryVoltage, 3);
    Serial.println(" V");

    Serial.println();

    delay(1000);
}