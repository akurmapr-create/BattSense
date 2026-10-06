#pragma once

#include <Arduino.h>

struct BatteryDiagnostics
{
    int rawAdcCount;
    float adcVoltage;
    float batteryVoltage;
};

class BatteryMonitor
{
public:
    void begin();

    float getVoltage();

    BatteryDiagnostics getDiagnostics();

private:
    static constexpr uint8_t ADC_PIN = 34;
    static constexpr uint8_t ADC_RESOLUTION = 12;
    int readRawADC();
    float readADCVoltage();
    static constexpr float R_UPPER_OHMS = 20700.0f;
    static constexpr float R_LOWER_OHMS = 13000.0f;
    float adcToBatteryVoltage(float adcVoltage);
};