#include "BatteryMonitor.h"

void BatteryMonitor::begin()
{
    analogReadResolution(ADC_RESOLUTION);
    analogSetPinAttenuation(ADC_PIN, ADC_6db);
}

float BatteryMonitor::getVoltage()
{
    const float adcVoltage = readADCVoltage();

    return adcToBatteryVoltage(adcVoltage);
}

BatteryDiagnostics BatteryMonitor::getDiagnostics()
{
    BatteryDiagnostics diagnostics{};

    diagnostics.rawAdcCount = readRawADC();
    diagnostics.adcVoltage = readADCVoltage();
    diagnostics.batteryVoltage =
        adcToBatteryVoltage(diagnostics.adcVoltage);

    return diagnostics;
}

int BatteryMonitor::readRawADC()
{
    return analogRead(ADC_PIN);
}

float BatteryMonitor::readADCVoltage()
{
    const uint32_t millivolts = analogReadMilliVolts(ADC_PIN);

    return millivolts / 1000.0f;
}

float BatteryMonitor::adcToBatteryVoltage(float adcVoltage)
{
    return adcVoltage *
           ((R_UPPER_OHMS + R_LOWER_OHMS) / R_LOWER_OHMS);
}