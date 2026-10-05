#pragma once
struct BatteryDiagnostics
{
    int rawAdcCount;
    float adcVoltage
    float batteryVoltage;
}

class BatteryMonitor
{
    public;
    void begin();
    float getVoltage();
    BatteryDiagnostics getDiagnostics();
}