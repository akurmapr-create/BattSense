# Analog Front-End Design

## Overview

The analog front-end is responsible for measuring the voltage of a single-cell (1S) 18650 lithium-ion battery using the ESP32 ADC.

Because the battery voltage exceeds the desired ADC input range, a passive resistive voltage divider is used to scale the battery voltage before it is measured by the microcontroller.

> **Note**
>
> BattSense is a battery telemetry platform and **not** a Battery Management System (BMS). The analog front-end only measures battery voltage and does not perform charging control, battery protection, or cell balancing.

---

# Requirements

| Requirement | Value |
| :---------- | :---- |
| Battery Chemistry | 1S Lithium-Ion (18650) |
| Battery Voltage Range | 2.5 V – 4.2 V |
| Microcontroller | ESP32-WROOM-32 |
| ADC Peripheral | ADC1 |
| ADC Attenuation | 6 dB |

---

# Design Objectives

The analog front-end was designed with the following objectives:

- Safely measure the battery voltage.
- Keep the ADC input within its intended operating range.
- Leave engineering margin below the ADC input limit.
- Maximize utilization of the ADC measurement range.
- Minimize continuous battery drain.
- Maintain a source impedance suitable for reliable ADC conversions.
- Use standard resistor values whenever possible.

---

# Hardware Architecture

```
      18650 Battery
            │
            ▼
    Voltage Divider
            │
            ▼
      ESP32 ADC1 Pin
            │
            ▼
    BatteryMonitor Module
            │
            ▼
      BLE Telemetry
```

---

# ADC Configuration

The ESP32 ADC supports multiple attenuation settings that determine the measurable input voltage range.

For BattSense, **6 dB attenuation** was selected because it provides sufficient input range while generally offering better ADC performance than the highest attenuation setting.

---

# ADC Target Voltage

A fully charged lithium-ion battery reaches approximately **4.2 V**.

Rather than scaling the battery voltage directly to the maximum ADC input range, the design intentionally maps the battery voltage to **2.0 V**.

## Design Decision

| Parameter | Value |
| :-------- | ----: |
| Maximum Battery Voltage | 4.2 V |
| Target ADC Voltage | 2.0 V |

### Reasoning

The target ADC voltage of **2.0 V** was selected because:

- It provides approximately **0.2 V** of engineering margin below the nominal 6 dB input range.
- It accounts for resistor tolerances.
- It accounts for ADC variation between ESP32 devices.
- It avoids operating near the upper end of the ADC range.
- It still utilizes approximately **91%** of the available ADC measurement range.

The slight reduction in effective resolution was considered an acceptable tradeoff for improved robustness.

---

# Voltage Divider

The battery voltage is reduced using a passive resistor voltage divider.

The divider was designed to:

- Scale **4.2 V** to approximately **2.0 V**
- Minimize measurement error
- Maintain acceptable source impedance
- Minimize battery drain

The calculated resistor ratio is:

**R₁ : R₂ = 11 : 10**

Final resistor values will be selected from standard resistor series during component selection.

---

# Source Impedance

The ESP32 ADC contains an internal sample-and-hold capacitor that must charge before every conversion.

Espressif recommends keeping the ADC source impedance below approximately **10 kΩ** for reliable measurements.

Instead of designing directly at this recommendation, a design target of **8 kΩ** was selected.

### Reasoning

- Provides engineering margin below the recommended limit.
- Allows faster charging of the ADC sampling capacitor.
- Improves measurement repeatability.
- Maintains acceptable battery current consumption.

---

# Divider Current

The voltage divider continuously draws current from the battery.

Using the calculated resistor values, the divider current is approximately:

**131 µA**

For the current development platform, this current draw is acceptable.

Future low-power revisions may reduce this current by:

- Increasing divider resistance.
- Switching the divider on only during measurements.
- Adding an ADC input capacitor to support higher resistance values.

---

# Design Tradeoffs

| Decision | Benefit | Tradeoff |
| :------- | :------ | :------- |
| ADC1 | No Wi-Fi resource conflict | Fewer available ADC pins |
| 6 dB attenuation | Better ADC performance | Smaller measurement range |
| 2.0 V target | Additional engineering margin | Slightly reduced effective resolution |
| 8 kΩ source impedance | Reliable ADC sampling | Higher divider current |
| Passive voltage divider | Simple, inexpensive, reliable | Continuous battery drain |

---

# Future Improvements

Potential improvements include:

- ADC calibration using the ESP-IDF calibration driver.
- Averaging multiple ADC samples.
- Switched voltage divider using a MOSFET.
- Higher-value divider with an ADC input capacitor.
- PCB optimization for improved analog signal integrity.

---

# References

- ESP32-WROOM-32 Datasheet
- ESP32 Technical Reference Manual
- ESP-IDF ADC Programming Guide