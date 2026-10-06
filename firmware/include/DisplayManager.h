#pragma once

class DisplayManager
{
public:
    bool begin();

    void showBattery(
        float batteryVoltage,
        float stateOfCharge
    );
};