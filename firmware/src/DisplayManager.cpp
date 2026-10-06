#include "DisplayManager.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace
{

constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;

constexpr uint8_t OLED_ADDRESS = 0x3C;
constexpr int OLED_RESET = -1;

constexpr uint8_t SDA_PIN = 21;
constexpr uint8_t SCL_PIN = 22;

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    OLED_RESET
);

}

bool DisplayManager::begin()
{
    Wire.begin(SDA_PIN, SCL_PIN);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS))
    {
        return false;
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.display();

    return true;
}

void DisplayManager::showBattery(
    float batteryVoltage,
    float stateOfCharge
)
{
    display.clearDisplay();

    // Header
    display.setTextSize(1);
    display.setCursor(38, 2);
    display.print("BattSense");

    // State of charge
    display.setTextSize(3);
    display.setCursor(34, 18);
    display.print(static_cast<int>(stateOfCharge + 0.5f));
    display.print("%");

    // Battery voltage
    display.setTextSize(1);
    display.setCursor(45, 51);
    display.print(batteryVoltage, 2);
    display.print(" V");

    display.display();
}