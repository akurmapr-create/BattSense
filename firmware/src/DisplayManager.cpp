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

} // namespace


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
    // Clamp SoC to the valid display range.
    if (stateOfCharge < 0.0f)
    {
        stateOfCharge = 0.0f;
    }
    else if (stateOfCharge > 100.0f)
    {
        stateOfCharge = 100.0f;
    }

    const int newPercent =
        static_cast<int>(stateOfCharge + 0.5f);

    // Initialize the displayed percentage immediately.
    if (displayedPercent < 0)
    {
        displayedPercent = newPercent;
    }
    // Only update the displayed percentage if the new value
    // differs by at least the hysteresis threshold.
    else if (abs(newPercent - displayedPercent) >= SOC_HYSTERESIS)
    {
        displayedPercent = newPercent;
    }

    display.clearDisplay();

    // ------------------------------------------------
    // Header
    // Top portion of this OLED is physically yellow.
    // ------------------------------------------------

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(37, 2);
    display.print("BattSense");

    display.drawLine(
        0,
        14,
        SCREEN_WIDTH - 1,
        14,
        SSD1306_WHITE
    );

    // ------------------------------------------------
    // State-of-charge percentage
    // ------------------------------------------------

    display.setTextSize(2);

    // Adjust horizontal position depending on number
    // of digits so the percentage remains centered.
    if (displayedPercent == 100)
    {
        display.setCursor(43, 19);
    }
    else if (displayedPercent >= 10)
    {
        display.setCursor(49, 19);
    }
    else
    {
        display.setCursor(55, 19);
    }

    display.print(displayedPercent);
    display.print("%");

    // ------------------------------------------------
    // Battery level bar
    // ------------------------------------------------

    constexpr int BAR_X = 14;
    constexpr int BAR_Y = 39;
    constexpr int BAR_WIDTH = 100;
    constexpr int BAR_HEIGHT = 10;

    display.drawRect(
        BAR_X,
        BAR_Y,
        BAR_WIDTH,
        BAR_HEIGHT,
        SSD1306_WHITE
    );

    const int innerWidth = BAR_WIDTH - 4;

    // The bar represents the underlying SoC estimate,
    // not the hysteresis-controlled displayed percentage.
    const int fillWidth =
        static_cast<int>(
            (stateOfCharge / 100.0f) * innerWidth
        );

    if (fillWidth > 0)
    {
        display.fillRect(
            BAR_X + 2,
            BAR_Y + 2,
            fillWidth,
            BAR_HEIGHT - 4,
            SSD1306_WHITE
        );
    }

    // ------------------------------------------------
    // Battery voltage
    // ------------------------------------------------

    display.setTextSize(1);
    display.setCursor(47, 54);

    display.print(batteryVoltage, 2);
    display.print(" V");

    // Send completed frame to OLED.
    display.display();
}