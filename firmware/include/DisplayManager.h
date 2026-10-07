#pragma once

class DisplayManager
{
public:
    bool begin();

    void showBattery(
        float batteryVoltage,
        float stateOfCharge
    );

private:
    int displayedPercent = -1;

    static constexpr int SOC_HYSTERESIS = 2;
};